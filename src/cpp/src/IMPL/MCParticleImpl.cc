
#include "IMPL/MCParticleImpl.h"

#include "EVENT/LCIO.h"
#include <stdexcept>
#include <vector>
#include <math.h> 

using namespace DATA ;
using namespace EVENT ;

namespace IMPL {

  MCParticleImpl::MCParticleImpl() :
    _mother0(0),
    _mother1(0),
    _pdg(0),
    _status(0),
    _mass(0),
    _charge(0),
    _daughtersP(0)
  {
    _vertex[0] = 0.0 ;
    _vertex[1] = 0.0 ;
    _vertex[2] = 0.0 ;
    _p[0] = 0.0 ;
    _p[1] = 0.0 ;
    _p[2] = 0.0 ;
    _endpoint[0] = 0.0 ;
    _endpoint[1] = 0.0 ;
    _endpoint[2] = 0.0 ;
  }
  
  MCParticleImpl::~MCParticleImpl(){
    // no dynamic variables
    //    delete [] _readDaughters ;
    for(MCParticlePVec::iterator iter = _daughtersP.begin();iter != _daughtersP.end() ;iter++){
      delete (*iter) ;
    }

  }
  
  MCParticleData * MCParticleImpl::getParentData() const { return _mother0 ; } 
  MCParticle * MCParticleImpl::getParent() const throw (DataNotAvailableException) 
  { 
    if( _mother0 == 0 ) throw DataNotAvailableException("MCParticleImpl::getParent() : no parent ! ") ;
    return _mother0 ; 
  }

  MCParticleData * MCParticleImpl::getSecondParentData() const { return _mother1 ;  }  
  MCParticle * MCParticleImpl::getSecondParent() const throw (DataNotAvailableException){ 
    if( _mother1 == 0 ) throw DataNotAvailableException("MCParticleImpl::getParent() : no second parent ! ");
    return _mother1 ; 
  }

  //  const MCParticleVec * MCParticleImpl::getDaughters() const { return &_daughters ; }


  int MCParticleImpl::getNumberOfDaughters() const { return _daughtersP.size() ; }

  MCParticle* MCParticleImpl::getDaughter(int i) const throw (DataNotAvailableException) { 

    try{
      //      return _daughters.at(i) ;
      //FIXME gcc 2.95 doesn't know at(i) ??

      return *_daughtersP[i] ;
    }catch( std::out_of_range ){
      throw DataNotAvailableException(std::string("MCParticleImpl::getDaughter(): out_of_range :" + i ) );
    }

  }
  // unchecked access
  MCParticleData* MCParticleImpl::getDaughterData(int i) const {
    return *_daughtersP[i] ;
  }

  const double* MCParticleImpl::getEndpoint() const { 

    if( _daughtersP.size() > 0 )
      return (*_daughtersP[0])->getVertex() ;

    return _endpoint ;
  }

  float MCParticleImpl::getEnergy() const { 
    return sqrt( _p[0]*_p[0] + _p[1]*_p[1] + _p[2]*_p[2] + _mass*_mass ) ;  
  }


  int MCParticleImpl::getPDG() const { return _pdg ;}
  int MCParticleImpl::getHepEvtStatus() const { return _status ;}
  const double * MCParticleImpl::getVertex() const { return _vertex ;}
  const float * MCParticleImpl::getMomentum() const { return _p ;}
  float MCParticleImpl::getMass() const { return _mass ;}
  float MCParticleImpl::getCharge() const { return _charge ; }

  void MCParticleImpl::setParent( MCParticle *mom0 ) { 
    checkAccess("MCParticleImpl::setParent") ;
    _mother0 = mom0 ; 
  }
  void MCParticleImpl::setSecondParent( MCParticle *mom1 ) {
    checkAccess("MCParticleImpl::setSecondParent") ;
    _mother1 = mom1 ; 
  }
  void MCParticleImpl::addDaughter( MCParticle* daughter) { 

    checkAccess("MCParticleImpl::addDaughter") ;

    MCParticle** pD = new (MCParticle*)  ;
    *pD = daughter ;
    _daughtersP.push_back( pD ) ;
  }
  void MCParticleImpl::setPDG(int pdg ) { 
    checkAccess("MCParticleImpl::setPDG") ;
    _pdg = pdg ; 
  }
  void MCParticleImpl::setHepEvtStatus( int status ) { 
    checkAccess("MCParticleImpl::setHepEvtStatus") ;
    _status = status ;
  } 
  void MCParticleImpl::setVertex( double vtx[3] ){
    checkAccess("MCParticleImpl::setVertex") ;
    _vertex[0] = vtx[0] ;
    _vertex[1] = vtx[1] ;
    _vertex[2] = vtx[2] ;
  }
  void MCParticleImpl::setMomentum( float p[3] ){
    checkAccess("MCParticleImpl::setMomentum") ;
    _p[0] = p[0] ;
    _p[1] = p[1] ;
    _p[2] = p[2] ;
  }
  void MCParticleImpl::setMass( float m ) { 
    checkAccess("MCParticleImpl::setMass") ;
    _mass = m ; 
  } 
  void MCParticleImpl::setCharge( float c ) { 
    checkAccess("MCParticleImpl::setCharge") ;
    _charge = c ;  
  } 

  void MCParticleImpl::setEndpoint( double endpoint[3] ){
    checkAccess("MCParticleImpl::setEndpoint") ;
    _endpoint[0] = endpoint[0] ;
    _endpoint[1] = endpoint[1] ;
    _endpoint[2] = endpoint[2] ;
  }


} ; /// namespace IMPl
