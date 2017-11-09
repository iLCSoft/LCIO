#include "IMPL/SimCalorimeterHitImpl.h"
#include "UTIL/LCWarning.h"
#include <iostream>
#include <sstream>

namespace IMPL{
  



  SimCalorimeterHitImpl::SimCalorimeterHitImpl() :
    _cellID0(0),
    _cellID1(0),
    _energy(0){
    _position[0] = 0. ;
    _position[1] = 0. ;
    _position[2] = 0. ; 
  }

  //   // copy of the SimCalorimeterHit hit
  SimCalorimeterHitImpl::SimCalorimeterHitImpl(const SimCalorimeterHit& hit) :

    _cellID0( hit.getCellID0() ),
    _cellID1( hit.getCellID1() ),
    _energy( hit.getEnergy() ) {

    const float* p = hit.getPosition() ;
    _position[0] = p[0] ;
    _position[1] = p[1] ;
    _position[2] = p[2] ;

    int nMC = hit.getNMCContributions() ;
    _vec.reserve( nMC ) ; 

    // now copy all the MCParticle contributions
    for(int i=0; i<nMC ;i++){

      MCParticleCont* con = new  MCParticleCont( hit.getParticleCont(i),
						 hit.getEnergyCont(i),
						 hit.getTimeCont(i),
						 hit.getLengthCont(i),
						 hit.getPDGCont(i),
						 hit.getStepPosition(i) ) ;
      _vec.push_back( con ) ;
    }  
  }
  
  //   EVENT::LCObject* SimCalorimeterHitImpl::clone() const{
  //     // shallow copy ... more thought needed
  //     return new SimCalorimeterHitImpl( *this ) ;
  //   }


  SimCalorimeterHitImpl::~SimCalorimeterHitImpl(){
    // delete all MCParticle contributions
    MCParticleContVec::iterator iter = _vec.begin() ;

    while( iter != _vec.end() ){
      delete *iter++ ;
    }

  }

  int SimCalorimeterHitImpl::getCellID0() const {
    return _cellID0 ;
  }

  int SimCalorimeterHitImpl::getCellID1() const {
    return _cellID1 ;
  }

  float SimCalorimeterHitImpl::getEnergy() const {
    return _energy ;
  }

  const float* SimCalorimeterHitImpl::getPosition() const {
    return _position ;
  }

  const float* SimCalorimeterHitImpl::getStepPosition( int i ) const {
    return _vec[i]->StepPosition ;
  }

  int SimCalorimeterHitImpl::getNMCParticles() const {
    //static bool first = true ;
    //if( first ){
    //  std::cout << " WARNING >>>>>>>  SimCalorimeterHitImpl::getNMCParticles() is deprecated "
    //            << " - please use  SimCalorimeterHitImpl::getNMCContributions() ! " << std::endl ;
    //  first = false ;
    //}

    UTIL::LCWarning::getInstance().printWarning( "SIMCALORIMETERHIT_DEPRECATED_GETNMCPARTICLES" ) ;

    return getNMCContributions() ;
  }

  int SimCalorimeterHitImpl::getNMCContributions() const {
    return _vec.size() ;
  }

  EVENT::MCParticle * SimCalorimeterHitImpl::getParticleCont(int i) const {
    try{
      return _vec.at(i)->Particle ;
    }catch( std::out_of_range ){
      std::stringstream err ; err << "SimCalorimeterHitImpl::getParticleCont(): out_of_range :"  << i  ;
      throw EVENT::Exception( err.str() );
    }
  }

  float SimCalorimeterHitImpl::getEnergyCont(int i) const {
    return _vec[i]->Energy ;
  }

  float SimCalorimeterHitImpl::getTimeCont(int i) const {
    return _vec[i]->Time ;
  }
  float SimCalorimeterHitImpl::getLengthCont(int i) const {
    return _vec[i]->Length ;
  }

  int SimCalorimeterHitImpl::getPDGCont(int i) const{
    return _vec[i]->PDG ;
  }

  void SimCalorimeterHitImpl::setCellID0(int id0){
    checkAccess("SimCalorimeterHitImpl::setCellID0") ;
    _cellID0 = id0 ;
  }

  void SimCalorimeterHitImpl::setCellID1(int id1){
    checkAccess("SimCalorimeterHitImpl::setCellID1") ;
    _cellID1 = id1 ;
  }

  void SimCalorimeterHitImpl::setEnergy(float en){
    checkAccess("SimCalorimeterHitImpl::setEnergy") ;
    _energy = en ;
  }

  void SimCalorimeterHitImpl::setPosition(const float pos[3]){
    checkAccess("SimCalorimeterHitImpl::setPosition") ;
    _position[0] = pos[0] ;
    _position[1] = pos[1] ;
    _position[2] = pos[2] ;
  }


  void SimCalorimeterHitImpl::addMCParticleContribution( EVENT::MCParticle *p,
							 float en,
							 float t ) {

    checkAccess("SimCalorimeterHitImpl::addMCParticleContribution") ;

    _energy += en ;

    static const float nullStep[3] = { 0.,0.,0. } ;

    // if we already have the particle, just add the energy
    for( std::vector<MCParticleCont*>::iterator it=_vec.begin(), End = _vec.end() ; it != End ; ++it ) {


    if( (*it)->Particle == p ) { 
	
    	(*it)->Energy += en ;
	return ;
      }
    }
    // else create a new contribution
    _vec.push_back( new MCParticleCont( p , en , t , 0 , 0 , nullStep )  )  ;

  }
  
  

  void SimCalorimeterHitImpl::addMCParticleContribution( EVENT::MCParticle *p,
							 float en,
							 float t,
							 float l,
							 int pdg, 
							 float* stepPos
							 )  {
  
    checkAccess("SimCalorimeterHitImpl::addMCParticleContribution") ;

   _energy += en ;

    static const float nullStep[3] = { 0.,0.,0. } ;
    
     // add a new contribution :

    _vec.push_back( new MCParticleCont( p , en , t , l , pdg ,  ( stepPos ? stepPos : nullStep )  )  )  ;

    }
} // namespace IMPL
