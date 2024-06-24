#include "UTIL/CheckCollections.h"

#include "Exceptions.h"
#include "lcio.h"

#include "IMPL/LCCollectionVec.h"
#include "MT/LCReader.h"

#include <algorithm>
#include <iomanip>

namespace UTIL {

void CheckCollections::checkFiles(const std::vector<std::string> &fileNames,
                                  bool quiet) {

  for (auto n : fileNames)
    checkFile(n, quiet);
}

void CheckCollections::checkFile(const std::string &fileName, bool quiet) {

  MT::LCReader lcReader(MT::LCReader::directAccess);
  lcReader.open(fileName);
  //----------- the event loop -----------
  while (const auto evt = lcReader.readNextEventHeader()) {
    const auto *colNames = evt->getCollectionNames();
    std::vector<std::string> recoCollections{};

    for (const auto &name : *colNames) {
      const auto col = evt->getCollection(name);
      auto typeString = col->getTypeName();

      // For ReconstructedParticle we also have to check the ParticleIDs for
      // consistency. We have to do this regardless of whether the
      // ReconstructedParticle collection is already in the map or not
      if (typeString == "ReconstructedParticle") {
        recoCollections.emplace_back(name);
      }

      auto it = _map.find(name);
      if (it == _map.end()) {
        // If the type of a collection is LCRelation we want to read the entire
        // collections instead of just the header to get the 'ToType' and
        // 'FromType'. setReadCollectionNames({name}) allows reading of only
        // certain collections by name instead of an entire event. This flag has
        // to be unset after reading in order for the reading of the headers to
        // function properly.
        if (col->getTypeName() == "LCRelation") {
          lcReader.setReadCollectionNames({name});
          auto fullEvt =
              lcReader.readEvent(evt->getRunNumber(), evt->getEventNumber());
          lcReader.setReadCollectionNames({});

          auto fullcol = fullEvt->getCollection(name);
          const auto &params = fullcol->getParameters();
          const auto &fromType = params.getStringVal("FromType");
          const auto &toType = params.getStringVal("ToType");
          if (quiet == false) {
            if (fromType == "" || toType == "") {
              std::cout << "WARNING! : Relation " << name
                        << " does not have the 'FromType' and 'ToType' set."
                        << std::endl;
            }
          }
          typeString = "LCRelation[" + fromType + "," + toType + "]";
        }
        std::tie(it, std::ignore) =
            _map.emplace(name, std::make_pair(std::move(typeString), 0));
      }

      it->second.second++;
    }

    lcReader.setReadCollectionNames(recoCollections);
    auto fullEvt =
        lcReader.readEvent(evt->getRunNumber(), evt->getEventNumber());
    lcReader.setReadCollectionNames({});

    for (const auto &name : recoCollections) {
      auto handler = PIDHandler(fullEvt->getCollection(name));
      insertParticleIDMetas(handler, name);
    }

    _nEvents++;
  }

  lcReader.close();
}

void CheckCollections::insertParticleIDMetas(const UTIL::PIDHandler &pidHandler,
                                             const std::string &recoName) {
  const auto &algoIds = pidHandler.getAlgorithmIDs();
  auto mapIt = _particleIDMetas.find(recoName);
  if (mapIt == _particleIDMetas.end()) {
    std::tie(mapIt, std::ignore) =
        _particleIDMetas.emplace(recoName, std::vector<PIDMeta>{});
  }

  auto &pidMetas = mapIt->second;
  for (const auto id : algoIds) {
    const auto &name = pidHandler.getAlgorithmName(id);

    if (auto it = std::find_if(
            pidMetas.begin(), pidMetas.end(),
            [&name](const auto &pidMeta) { return pidMeta.name == name; });
        it == pidMetas.end()) {
      pidMetas.emplace_back(name, pidHandler.getParameterNames(id), 1);
    } else {
      it->count++;
    }
  }
}

CheckCollections::Vector CheckCollections::getMissingCollections() const {
  Vector s;
  for (const auto &e : _map) {
    if (e.second.second != _nEvents)
      s.push_back({e.first, e.second.first});
  }
  return s;
}

CheckCollections::Vector CheckCollections::getConsistentCollections() const {
  Vector s;
  for (auto e : _map) {
    if (e.second.second == _nEvents)
      s.push_back({e.first, e.second.first});
  }
  return s;
}

// Obtain the name of the recontructed particle collection as well as the
// parameter names from an encoded "RecoColl|[name1[,names...]]"
std::tuple<std::string, std::vector<std::string>>
getRecoCollAndParamNames(const std::string_view fullType) {
  auto delim = fullType.find('|');
  auto recoName = std::string(fullType.substr(0, delim));

  std::vector<std::string> paramNames{};
  while (delim != std::string_view::npos) {
    auto oldDelim = delim + 1;
    delim = fullType.find(',', oldDelim);
    paramNames.emplace_back(fullType.substr(oldDelim, delim));
  }

  return {recoName, paramNames};
}

void CheckCollections::addPatchCollection(std::string name, std::string type) {
  if (type.find('|') != std::string::npos) {
    auto [recoName, paramNames] = getRecoCollAndParamNames(name);
    _particleIDMetas[recoName].emplace_back(name, std::move(paramNames));
  } else {
    _patchCols.emplace_back(std::move(name), std::move(type));
  }
}

void CheckCollections::addPatchCollections(Vector cols) {
  for (auto &&[name, type] : cols) {
    if (type.find('|') != std::string::npos) {
      auto [recoName, paramNames] = getRecoCollAndParamNames(type);
      _particleIDMetas[recoName].emplace_back(name, std::move(paramNames));
    } else {
      _patchCols.emplace_back(std::move(name), std::move(type));
    }
  }
}

// Obtain the from and to type from the encoded "LCRelation[From,To]"
std::tuple<std::string_view, std::string_view>
getToFromType(const std::string_view fullType) {
  auto delim = fullType.find(',');
  constexpr auto prefixLen = 11u; // length of "LCRelation["

  return {fullType.substr(prefixLen, delim - prefixLen),
          fullType.substr(delim + 1, fullType.size() - delim -
                                         2)}; // need to strip final "]" as well
}

// Add all algorithms that are specified in the pidMetas to the PIDHandler, such
// that the necessary metadata is present
void patchParticleIDs(UTIL::PIDHandler &pidHandler,
                      const std::vector<CheckCollections::PIDMeta> &pidMetas) {
  for (const auto &[name, paramNames, _] : pidMetas) {
    try {
      // simply assume that param names are OK if we find the algorithm
      pidHandler.getAlgorithmID(name);
    } catch (UnknownAlgorithm &) {
      pidHandler.addAlgorithm(name, paramNames);
    }
  }
}

void CheckCollections::patchCollections(EVENT::LCEvent *evt) const {
  for (const auto &[name, typeName] : _patchCols) {
    try {
      auto *coll = evt->getCollection(name);
      const auto collType = coll->getTypeName();
      if (collType == "LCRelation") {
        // For LCRelations we still have to check whether the FromType and
        // ToType are set and correct in case they are not
        auto &params = coll->parameters();
        if (params.getStringVal("FromType").empty() ||
            params.getStringVal("ToType").empty()) {
          const auto [from, to] = getToFromType(typeName);
          params.setValue("FromType", std::string(from));
          params.setValue("ToType", std::string(to));
        }
      }
    } catch (EVENT::DataNotAvailableException &e) {
      // 10 is the length of the String LCRelation after which the bracket is
      // and the "ToType" and "FromType" start.
      if (typeName.size() > 10 && typeName[10] == '[') {
        auto relationColl = new IMPL::LCCollectionVec("LCRelation");
        auto &params = relationColl->parameters();

        const auto [from, to] = getToFromType(typeName);
        params.setValue("FromType", std::string(from));
        params.setValue("ToType", std::string(to));
        evt->addCollection(relationColl, name);
      } else {
        evt->addCollection(new IMPL::LCCollectionVec(typeName), name);
      }
    }
  }

  for (const auto &[recoName, pidMeta] : _particleIDMetas) {
    // Let the exception propagate. This is not something that we can easily
    // handle in any meaningful way, so make users aware as early as possible
    auto pidHandler = UTIL::PIDHandler(evt->getCollection(recoName));
    patchParticleIDs(pidHandler, pidMeta);
  }
}

void CheckCollections::print(std::ostream &os, bool minimal) const {

  unsigned width = 50;
  if (minimal == false) {
    os << " ================================================================ "
       << std::endl;
    os << std::endl << "  " << _nEvents << " events read " << std::endl;
    os << "     collections that are not in all events :  [# events where col "
          "is present]"
       << std::endl;
    os << " ================================================================ "
       << std::endl;
  }
  if (minimal == false) {
    for (auto e : _map) {

      if (e.second.second != _nEvents)
        os << "     " << std::setw(width) << std::left << e.first << " "
           << std::setw(width) << e.second.first << " [" << e.second.second
           << "]" << std::endl;
    }
  }

  if (minimal == false) {
    os << " ================================================================ "
       << std::endl;
    os << "     collections that are in all events : " << std::endl;
    os << " ================================================================ "
       << std::endl;
  }
  if (minimal == false) {
    for (auto e : _map) {

      if (e.second.second == _nEvents)
        os << "     " << std::setw(width) << std::left << e.first << " "
           << std::setw(width) << e.second.first << "  [" << e.second.second
           << "]" << std::endl;
    }
  } else {
    for (auto e : _map) {

      os << "     " << std::setw(width) << std::left << e.first << " "
         << std::setw(width) << e.second.first << std::endl;
    }
  }
  if (minimal == false) {
    os << " ================================================================ "
       << std::endl;
  }

  if (!minimal) {
    os << " ================================================================\n"
       << "Known ParticleID algorithms for ReconstructedParticle collections\n";
    for (const auto &[name, pidMetas] : _particleIDMetas) {
      if (pidMetas.empty()) {
        continue;
      }
      os << name << ":\n";
      for (const auto &meta : pidMetas) {
        os << "     " << std::setw(width) << std::left << meta.name << "   ["
           << meta.count << "]\n";
      }
      os << " -----------------------------------------------------------------"
            " \n";
    }
    os << " ================================================================ "
       << std::endl;
  } else {
    for (const auto &[name, pidMetas] : _particleIDMetas) {
      for (const auto &meta : pidMetas) {
        os << meta.name << std::setw(width) << " " << name << "|";
        if (!meta.paramNames.empty()) {
          os << meta.paramNames[0];
          for (size_t i = 1; i < meta.paramNames.size(); ++i) {
            os << "," << meta.paramNames[i];
          }
        }
        os << '\n';
      }
    }
  }
  os << std::flush;
}

} // namespace UTIL
