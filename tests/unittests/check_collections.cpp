#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/MCParticleImpl.h"
#include "IMPL/ReconstructedParticleImpl.h"
#include "UTIL/CheckCollections.h"
#include "UTIL/PIDHandler.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

TEST_CASE("CheckCollections patching basic collections", "[CheckCollections]") {
  // Create an event that already contains a collection that we want to appear
  // in the patched event
  auto event = std::make_unique<IMPL::LCEventImpl>();
  event->setRunNumber(1);
  event->setEventNumber(1);
  auto mcParticles =
      std::make_unique<IMPL::LCCollectionVec>(EVENT::LCIO::MCPARTICLE);
  auto mcp = new IMPL::MCParticleImpl();
  mcParticles->addElement(mcp);
  event->addCollection(mcParticles.release(), "TestMCParticles");

  UTIL::CheckCollections checker;
  checker.addPatchCollections({{"TestMCParticles", "MCParticle"},
                               {"TestTracks", "Track"},
                               {"TestSubsetCol", "CalorimeterHit*"}});
  checker.patchCollections(event.get());

  auto mcCol = event->getCollection("TestMCParticles");
  REQUIRE(mcCol != nullptr);
  REQUIRE(mcCol->getTypeName() == "MCParticle");
  REQUIRE(mcCol->getNumberOfElements() == 1);
  REQUIRE_FALSE(mcCol->isSubset());

  auto trackCol = event->getCollection("TestTracks");
  REQUIRE(trackCol != nullptr);
  REQUIRE(trackCol->getTypeName() == "Track");
  REQUIRE(trackCol->getNumberOfElements() == 0);
  REQUIRE_FALSE(trackCol->isSubset());

  auto subsetCol = event->getCollection("TestSubsetCol");
  REQUIRE(subsetCol != nullptr);
  REQUIRE(subsetCol->getTypeName() == "CalorimeterHit");
  REQUIRE(subsetCol->getNumberOfElements() == 0);
  REQUIRE(subsetCol->isSubset());
}

TEST_CASE("CheckCollections patching LCRelation collections",
          "[CheckCollections]") {
  auto event = std::make_unique<IMPL::LCEventImpl>();

  UTIL::CheckCollections checker;
  checker.addPatchCollection("MCTruthRelation",
                             "LCRelation[MCParticle,ReconstructedParticle]");
  checker.patchCollections(event.get());

  auto relCol = event->getCollection("MCTruthRelation");
  REQUIRE(relCol != nullptr);
  REQUIRE(relCol->getTypeName() == "LCRelation");
  REQUIRE(relCol->getNumberOfElements() == 0);

  const auto &params = relCol->getParameters();
  REQUIRE(params.getStringVal("FromType") == "MCParticle");
  REQUIRE(params.getStringVal("ToType") == "ReconstructedParticle");
}

TEST_CASE("CheckCollections patching existing LCRelation without parameters",
          "[CheckCollections]") {
  // Create an event with the existing collection but missing From and To type
  // parameters
  auto event = std::make_unique<IMPL::LCEventImpl>();
  auto existingRel = new IMPL::LCCollectionVec("LCRelation");
  event->addCollection(existingRel, "ExistingRelation");

  UTIL::CheckCollections checker;
  checker.addPatchCollection("ExistingRelation", "LCRelation[Track,Cluster]");
  checker.patchCollections(event.get());

  auto relCol = event->getCollection("ExistingRelation");
  const auto &params = relCol->getParameters();
  REQUIRE(params.getStringVal("FromType") == "Track");
  REQUIRE(params.getStringVal("ToType") == "Cluster");
}

TEST_CASE("CheckCollections patching ParticleID algorithms",
          "[CheckCollections]") {
  auto event = std::make_unique<IMPL::LCEventImpl>();
  event->setRunNumber(1);
  event->setEventNumber(1);

  auto recoCol = new IMPL::LCCollectionVec("ReconstructedParticle");
  auto recoPart = new IMPL::ReconstructedParticleImpl();
  recoCol->addElement(recoPart);
  event->addCollection(recoCol, "ReconstructedParticles");

  UTIL::CheckCollections checker;
  checker.addPatchCollections(
      {{"LikelihoodPID", "ReconstructedParticles|dEdx,momentum"},
       {"BDT_PID", "ReconstructedParticles|score"},
       {"FancyPID", "ReconstructedParticles|param1,param2,param3"}});
  checker.patchCollections(event.get());

  auto pidHandler =
      UTIL::PIDHandler(event->getCollection("ReconstructedParticles"));

  int likelihoodID = -1;
  int bdtID = -1;
  int fancyID = -1;
  REQUIRE_NOTHROW(likelihoodID = pidHandler.getAlgorithmID("LikelihoodPID"));
  REQUIRE_NOTHROW(bdtID = pidHandler.getAlgorithmID("BDT_PID"));
  REQUIRE_NOTHROW(fancyID = pidHandler.getAlgorithmID("FancyPID"));

  const auto likelihoodParams = pidHandler.getParameterNames(likelihoodID);
  REQUIRE_THAT(likelihoodParams,
               Catch::Matchers::Equals<std::string>({"dEdx", "momentum"}));

  const auto bdtParams = pidHandler.getParameterNames(bdtID);
  REQUIRE_THAT(bdtParams, Catch::Matchers::Equals<std::string>({"score"}));

  const auto fancyParams = pidHandler.getParameterNames(fancyID);
  REQUIRE_THAT(fancyParams, Catch::Matchers::Equals<std::string>(
                                {"param1", "param2", "param3"}));
}
