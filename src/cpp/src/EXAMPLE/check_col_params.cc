#include "lcio.h"
#include "UTIL/CheckCollections.h"

#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace {

std::vector<std::string> splitComma(const std::string& s) {
  std::vector<std::string> out;
  std::string cur;
  for (char c : s) {
    if (c == ',') { if (!cur.empty()) out.push_back(std::move(cur)); cur.clear(); }
    else          { cur.push_back(c); }
  }
  if (!cur.empty()) out.push_back(std::move(cur));
  return out;
}

void jsonEscape(std::ostream& os, const std::string& s) {
  os << '"';
  for (char c : s) {
    switch (c) {
      case '"':  os << "\\\""; break;
      case '\\': os << "\\\\"; break;
      case '\b': os << "\\b";  break;
      case '\f': os << "\\f";  break;
      case '\n': os << "\\n";  break;
      case '\r': os << "\\r";  break;
      case '\t': os << "\\t";  break;
      default:
        if (static_cast<unsigned char>(c) < 0x20) {
          os << "\\u" << std::hex << std::setw(4) << std::setfill('0')
             << static_cast<int>(c) << std::dec << std::setfill(' ');
        } else {
          os << c;
        }
    }
  }
  os << '"';
}

template <typename T>
void emitNumericArray(std::ostream& os, const std::vector<T>& v) {
  os << '[';
  for (size_t i = 0; i < v.size(); ++i) {
    if (i) os << ", ";
    os << v[i];
  }
  os << ']';
}

void emitFloatArray(std::ostream& os, const std::vector<float>& v) {
  os << '[';
  os << std::setprecision(std::numeric_limits<float>::max_digits10);
  for (size_t i = 0; i < v.size(); ++i) {
    if (i) os << ", ";
    os << v[i];
  }
  os << ']';
}

void emitDoubleArray(std::ostream& os, const std::vector<double>& v) {
  os << '[';
  os << std::setprecision(std::numeric_limits<double>::max_digits10);
  for (size_t i = 0; i < v.size(); ++i) {
    if (i) os << ", ";
    os << v[i];
  }
  os << ']';
}

void emitStringArray(std::ostream& os, const std::vector<std::string>& v) {
  os << '[';
  for (size_t i = 0; i < v.size(); ++i) {
    if (i) os << ", ";
    jsonEscape(os, v[i]);
  }
  os << ']';
}

bool emitGroup(std::ostream& os, const char* label,
               const EVENT::StringVec& keys, bool needsComma,
               const std::function<void(const std::string&)>& emitVal) {
  if (keys.empty()) return needsComma;
  if (needsComma) os << ",\n";
  os << "    \"" << label << "\": {";
  for (size_t i = 0; i < keys.size(); ++i) {
    os << (i ? ",\n      " : "\n      ");
    jsonEscape(os, keys[i]);
    os << ": ";
    emitVal(keys[i]);
  }
  os << "\n    }";
  return true;
}

void usage() {
  std::cout << "usage: check_col_params --output <out.json> --params <p1,p2,...> "
               "<input-file1> [input-file2 ...]\n"
               "       (use '-' as output path to write to stdout)\n";
}

} // namespace

int main(int argc, char** argv) {
  std::string outputPath;
  std::vector<std::string> paramNames;
  std::vector<std::string> inputs;

  for (int i = 1; i < argc; ++i) {
    std::string a = argv[i];
    if ((a == "--output" || a == "-o") && i + 1 < argc) {
      outputPath = argv[++i];
    } else if ((a == "--params" || a == "-p") && i + 1 < argc) {
      paramNames = splitComma(argv[++i]);
    } else if (a == "-h" || a == "--help") {
      usage(); return 0;
    } else {
      inputs.push_back(a);
    }
  }

  if (outputPath.empty() || paramNames.empty() || inputs.empty()) {
    usage();
    return 1;
  }

  UTIL::CheckCollections cc;
  cc.checkParameters(inputs, paramNames);
  const auto collected = cc.getCollectedParameters();

  std::ofstream fileOut;
  std::ostream* out = &std::cout;
  if (outputPath != "-") {
    fileOut.open(outputPath);
    if (!fileOut) {
      std::cerr << "error: cannot open '" << outputPath << "' for writing\n";
      return 1;
    }
    out = &fileOut;
  }

  *out << "{";
  bool firstColl = true;
  for (const auto& [name, vals] : collected) {
    if (!firstColl) *out << ",";
    firstColl = false;
    *out << "\n  ";
    jsonEscape(*out, name);
    *out << ": {";

    EVENT::StringVec intKeys, floatKeys, doubleKeys, stringKeys;
    vals.getIntKeys(intKeys);
    vals.getFloatKeys(floatKeys);
    vals.getDoubleKeys(doubleKeys);
    vals.getStringKeys(stringKeys);

    bool needsComma = false;
    needsComma = emitGroup(*out, "int-params", intKeys, needsComma, [&](const std::string& k) {
      EVENT::IntVec v; vals.getIntVals(k, v); emitNumericArray<int>(*out, v);
    });
    needsComma = emitGroup(*out, "float-params", floatKeys, needsComma, [&](const std::string& k) {
      EVENT::FloatVec v; vals.getFloatVals(k, v); emitFloatArray(*out, v);
    });
    needsComma = emitGroup(*out, "double-params", doubleKeys, needsComma, [&](const std::string& k) {
      EVENT::DoubleVec v; vals.getDoubleVals(k, v); emitDoubleArray(*out, v);
    });
    needsComma = emitGroup(*out, "string-params", stringKeys, needsComma, [&](const std::string& k) {
      EVENT::StringVec v; vals.getStringVals(k, v); emitStringArray(*out, v);
    });

    if (needsComma) *out << "\n  ";
    *out << "}";
  }
  *out << "\n}\n";

  return 0;
}
