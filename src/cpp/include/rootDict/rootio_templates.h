#ifndef rootio_templates_h
#define rootio_templates_h 1


#include "UTIL/LCTypedVector.h"

#include "IMPL/ReconstructedParticleImpl.h"
#include "IMPL/MCParticleImpl.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/SimTrackerHitImpl.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "IMPL/TrackerHitImpl.h"
#include "IMPL/VertexImpl.h"
#include "IMPL/TrackImpl.h"
#include "IMPL/ClusterImpl.h"
#include "IMPL/LCRelationImpl.h"
#include "IMPL/LCCollectionVec.h"
//#include "IMPL/LCRefVec.h"
//#include "TRefArray.h"

#include <string>
#include <vector>

// ---- tell ROOT about the templates instantiations that we are going to use ...

#ifdef __MAKECINT__
#pragma link C++ class std::vector<std::string>+;
#pragma link C++ class std::vector<int>+;
#pragma link C++ class std::vector<float>+;

#pragma link C++ class std::map<std::string,EVENT::LCCollection*>+;
#pragma link C++ class std::pair<std::string,EVENT::LCCollection*>+;

#pragma link C++ class std::vector<IMPL::ReconstructedParticleImpl*>+;
#pragma link C++ class std::vector<IMPL::MCParticleImpl*>+;
#pragma link C++ class std::vector<IMPL::SimTrackerHitImpl*>+;
#pragma link C++ class std::vector<IMPL::SimCalorimeterHitImpl*>+;
#pragma link C++ class std::vector<IMPL::TrackImpl*>+;
#pragma link C++ class std::vector<IMPL::ClusterImpl*>+;
#pragma link C++ class std::vector<IMPL::LCRelationImpl*>+;
#pragma link C++ class std::vector<IMPL::VertexImpl*>+;

#pragma link C++ class std::vector<EVENT::LCObject*>+;
#pragma link C++ class std::vector<EVENT::ReconstructedParticle*>+;
#pragma link C++ class std::vector<EVENT::MCParticle*>+;
#pragma link C++ class std::vector<EVENT::SimTrackerHit*>+;
#pragma link C++ class std::vector<EVENT::SimCalorimeterHit*>+;
#pragma link C++ class std::vector<EVENT::Track*>+;
#pragma link C++ class std::vector<EVENT::Cluster*>+;
#pragma link C++ class std::vector<EVENT::LCRelation*>+;
#pragma link C++ class std::vector<EVENT::Vertex*>+;

// #pragma link C++ class IMPL::LCRefVec<EVENT::LCObject*>+;
// #pragma link C++ class IMPL::LCRefVec<EVENT::ReconstructedParticle*>+;
// #pragma link C++ class IMPL::LCRefVec<EVENT::MCParticle*>+;
// #pragma link C++ class IMPL::LCRefVec<EVENT::SimTrackerHit*>+;
// #pragma link C++ class IMPL::LCRefVec<EVENT::CalorimeterHit*>+;
// #pragma link C++ class IMPL::LCRefVec<EVENT::TrackerHit*>+;
// #pragma link C++ class IMPL::LCRefVec<EVENT::SimCalorimeterHit*>+;
// #pragma link C++ class IMPL::LCRefVec<EVENT::Track*>+;
// #pragma link C++ class IMPL::LCRefVec<EVENT::Cluster*>+;
// #pragma link C++ class IMPL::LCRefVec<EVENT::LCRelation*>+;
// #pragma link C++ class IMPL::LCRefVec<EVENT::Vertex*>+;

// #pragma link C++ class IMPL::LCRef<EVENT::LCObject*>+;
// #pragma link C++ class IMPL::LCRef<EVENT::ReconstructedParticle*>+;
// #pragma link C++ class IMPL::LCRef<EVENT::MCParticle*>+;
// #pragma link C++ class IMPL::LCRef<EVENT::SimTrackerHit*>+;
// #pragma link C++ class IMPL::LCRef<EVENT::SimCalorimeterHit*>+;
// #pragma link C++ class IMPL::LCRef<EVENT::TrackerHit*>+;
// #pragma link C++ class IMPL::LCRef<EVENT::CalorimeterHit*>+;
// #pragma link C++ class IMPL::LCRef<EVENT::Track*>+;
// #pragma link C++ class IMPL::LCRef<EVENT::Cluster*>+;
// #pragma link C++ class IMPL::LCRef<EVENT::LCRelation*>+;
// #pragma link C++ class IMPL::LCRef<EVENT::Vertex*>+;

// //#pragma link C++ class IMPL::LCCollectionTVec<EVENT::LCObject>+;
// #pragma link C++ class IMPL::LCCollectionTVec<EVENT::ReconstructedParticle>+;
// #pragma link C++ class IMPL::LCCollectionTVec<EVENT::MCParticle>+;
// #pragma link C++ class IMPL::LCCollectionTVec<EVENT::SimTrackerHit>+;
// #pragma link C++ class IMPL::LCCollectionTVec<EVENT::SimCalorimeterHit>+;
// #pragma link C++ class IMPL::LCCollectionTVec<EVENT::TrackerHit>+;
// #pragma link C++ class IMPL::LCCollectionTVec<EVENT::CalorimeterHit>+;
// #pragma link C++ class IMPL::LCCollectionTVec<EVENT::Track>+;
// #pragma link C++ class IMPL::LCCollectionTVec<EVENT::Cluster>+;
// #pragma link C++ class IMPL::LCCollectionTVec<EVENT::LCRelation>+;
// #pragma link C++ class IMPL::LCCollectionTVec<EVENT::Vertex>+;


// #pragma link C++ class IMPL::LCCollectionTVec<IMPL::ReconstructedParticleImpl>+;
// #pragma link C++ class IMPL::LCCollectionTVec<IMPL::MCParticleImpl>+;
// #pragma link C++ class IMPL::LCCollectionTVec<IMPL::SimTrackerHitImpl>+;
// #pragma link C++ class IMPL::LCCollectionTVec<IMPL::SimCalorimeterHitImpl>+;
// #pragma link C++ class IMPL::LCCollectionTVec<IMPL::TrackerHitImpl>+;
// #pragma link C++ class IMPL::LCCollectionTVec<IMPL::CalorimeterHitImpl>+;
// #pragma link C++ class IMPL::LCCollectionTVec<IMPL::TrackImpl>+;
// #pragma link C++ class IMPL::LCCollectionTVec<IMPL::ClusterImpl>+;
// #pragma link C++ class IMPL::LCCollectionTVec<IMPL::LCRelationImpl>+;
// #pragma link C++ class IMPL::LCCollectionTVec<IMPL::VertexImpl>+;


#endif

#endif
