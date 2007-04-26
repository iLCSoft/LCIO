#include "IMPL/SimCalorimeterHitImpl.h"
#include <iostream>

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
    
    // now copy all the MCParticle contributions
    for(int i=0; i<nMC ;i++){
      
      MCParticleCont* con = new  MCParticleCont ;
      
      con->Particle = hit.getParticleCont(i) ;
      con->Energy = hit.getEnergyCont(i) ;
      con->Time = hit.getTimeCont(i) ;
      con->PDG = hit.getPDGCont(i) ;
      
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
  
  int SimCalorimeterHitImpl::getNMCParticles() const {
    static bool first = true ;
    if( first ){
      std::cout << " WARNING >>>>>>>  SimCalorimeterHitImpl::getNMCParticles() is deprecated "
                << " - please use  SimCalorimeterHitImpl::getNMCContributions() ! " << std::endl ;
      first = false ;
    }

    return getNMCContributions() ;
  }

  int SimCalorimeterHitImpl::getNMCContributions() const {
    return _vec.size() ;
  }
  
  EVENT::MCParticle * SimCalorimeterHitImpl::getParticleCont(int i) const {
    try{
      //return _vec.at(i)->Particle ;
      //FIXME gcc 2.95 doesn't know at(i) ??
      return _vec[i]->Particle ;      
    }
    catch( ... ){
      throw EVENT::Exception(std::string("SimCalorimeterHitImpl::getParticleCont: no particle at " + i ) ) ;
    }
  }

  
  float SimCalorimeterHitImpl::getEnergyCont(int i) const {
    return _vec[i]->Energy ;
  }
  
  float SimCalorimeterHitImpl::getTimeCont(int i) const {
    return _vec[i]->Time ;
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
  
  void SimCalorimeterHitImpl::setPosition(float pos[3]){
    checkAccess("SimCalorimeterHitImpl::setPosition") ;
    _position[0] = pos[0] ;
    _position[1] = pos[1] ;
    _position[2] = pos[2] ;
  }
  
  void SimCalorimeterHitImpl::addMCParticleContribution( EVENT::MCParticle *p,
						      float en,
						      float t,
						      int pdg ){

    checkAccess("SimCalorimeterHitImpl::addMCParticleContribution") ;
    if( p==0){
      // just add the energy - no MC contribution  !!
      _energy += en ;
      return ;
    }
    else if( pdg == 0 ) { // not in extended mode - only one contribution per primary particle



      for( std::vector<MCParticleCont*>::iterator iter=_vec.begin() ;
	   iter != _vec.end() ; iter++ ){

	if( (*iter)->Particle == p ) {  //  && (*iter)->PDG == pdg ){
	  
	  (*iter)->Energy += en ;

	  _energy += en ;
	  return ;
	}

      }
    }
      
    _energy += en ;
    MCParticleCont* con = new  MCParticleCont ;
    
    con->Particle = p ;
    con->Energy = en ;
    con->Time = t ;
    con->PDG = pdg ;
    _vec.push_back( con ) ;
    
  }
} // namespace IMPL
