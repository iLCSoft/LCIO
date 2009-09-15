#ifndef rootio_templates_h
#define rootio_templates_h 1


#include "UTIL/LCTypedVector.h"

#include "IMPL/ReconstructedParticleImpl.h"
#include "IMPL/MCParticleImpl.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/SimTrackerHitImpl.h"
#include "IMPL/VertexImpl.h"
#include "IMPL/TrackImpl.h"
#include "IMPL/ClusterImpl.h"
#include "IMPL/LCRelationImpl.h"

#include "TRefArray.h"
#include <string>

// ---- tell ROOT about the templates instantiations that we are going to use ...

#ifdef __MAKECINT__
#pragma link C++ class std::vector<std::string>+;
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
#endif

#endif
