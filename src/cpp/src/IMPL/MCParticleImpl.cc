
#include "IMPL/MCParticleImpl.h"

#include "EVENT/LCIO.h"
#include <stdexcept>

using namespace DATA ;
using namespace EVENT ;

namespace IMPL {

  MCParticleImpl::MCParticleImpl() :
    _mother0(0),
    _mother1(0),
    _pdg(0),
    _status(0),
    _energy(0),
    _charge(0){
  }
  
  MCParticleImpl::~MCParticleImpl(){
    // no dynamic variables
  }
  
  const MCParticleData * MCParticleImpl::getParentData() const { return _mother0 ; } 
  const MCParticle * MCParticleImpl::getParent() const throw (DataNotAvailableException) 
  { 
    if( _mother0 == 0 ) throw DataNotAvailableException("MCParticleImpl::getParent() : no parent ! ") ;
    return _mother0 ; 
  }

  const MCParticleData * MCParticleImpl::getSecondParentData() const { return _mother1 ;  }  
  const MCParticle * MCParticleImpl::getSecondParent() const throw (DataNotAvailableException){ 
    if( _mother1 == 0 ) throw DataNotAvailableException("MCParticleImpl::getParent() : no second parent ! ");
    return _mother1 ; 
  }

  //  const MCParticleVec * MCParticleImpl::getDaughters() const { return &_daughters ; }


  int MCParticleImpl::getNumberOfDaughters() const { return _daughters.size() ; }

  const MCParticle* MCParticleImpl::getDaughter(int i) const throw (DataNotAvailableException) { 
    // checked access
    try{
      return _daughters.at(i) ;
    }catch( std::out_of_range ){
      throw DataNotAvailableException(std::string("MCParticleImpl::getDaughter(): out_of_range :" + i ) );
    }

  }
  // unchecked access
  const MCParticleData* MCParticleImpl::getDaughterData(int i) const {
    return _daughters[i] ;
  }

  const double* MCParticleImpl::getEndpoint() const { 

    if( _daughters.size() > 0 )
      return _daughters[0]->getVertex() ;

    return _endpoint ;
  }


  int MCParticleImpl::getPDG() const { return _pdg ;}
  int MCParticleImpl::getHepEvtStatus() const { return _status ;}
  const double * MCParticleImpl::getVertex() const { return _vertex ;}
  const float * MCParticleImpl::getMomentum() const { return _p ;}
  float MCParticleImpl::getEnergy() const { return _energy ;}
  float MCParticleImpl::getCharge() const { return _charge ; }

  void MCParticleImpl::setParent(  const  MCParticle *mom0 ) { _mother0 = mom0 ; }
  void MCParticleImpl::setSecondParent(  const  MCParticle *mom1 ) { _mother1 = mom1 ; }
  void MCParticleImpl::addDaughter( const MCParticle* daughter) { 
    _daughters.push_back( daughter ) ;
  }
  void MCParticleImpl::setPDG(int pdg ) { _pdg = pdg ; }
  void MCParticleImpl::setHepEvtStatus( int status ) { _status = status ;} 
  void MCParticleImpl::setVertex( double vtx[3] ){
    _vertex[0] = vtx[0] ;
    _vertex[1] = vtx[1] ;
    _vertex[2] = vtx[2] ;
  }
  void MCParticleImpl::setMomentum( float p[3] ){
    _p[0] = p[0] ;
    _p[1] = p[1] ;
    _p[2] = p[2] ;
  }
  void MCParticleImpl::setEnergy( float en ) { _energy = en ; } 
  void MCParticleImpl::setCharge( float c ) { _charge = c ;  } 





} ; /// namespace IMPl
