
#include "IMPL/MCParticleImpl.h"

#include "EVENT/LCIO.h"
#include <stdexcept>
#include <vector>
#include <math.h> 

using namespace DATA ;
using namespace EVENT ;

namespace IMPL {

  MCParticleImpl::MCParticleImpl() :
    _pdg(0),
    _genstatus(0),
    _simstatus(0),
    _mass(0),
    _charge(0),
    _parentsP(0),
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
  
//   MCParticleData * MCParticleImpl::getParentData() const { return _mother0 ; } 
//   MCParticle * MCParticleImpl::getParent() const { return _mother0 ; }

//   MCParticleData * MCParticleImpl::getSecondParentData() const { return _mother1; }  
//   MCParticle * MCParticleImpl::getSecondParent() const { return _mother1; }

//   //  const MCParticleVec * MCParticleImpl::getDaughters() const { return &_daughters ; }


  int MCParticleImpl::getNumberOfParents() const { return _parentsP.size() ; }

  MCParticle* MCParticleImpl::getParent(int i) const {
    
    try{
      //      return _parents.at(i) ;
      //FIXME gcc 2.95 doesn't know at(i) ??
      
      return *_parentsP[i] ;
    }catch( std::out_of_range ){
      throw Exception(std::string("MCParticleImpl::getParent(): out_of_range :" 
				  + i ) );
    }
    
  }
  // unchecked access
  MCParticleData* MCParticleImpl::getParentData(int i) const {
    return *_parentsP[i] ;
  }


  int MCParticleImpl::getNumberOfDaughters() const { return _daughtersP.size() ; }

  MCParticle* MCParticleImpl::getDaughter(int i) const {
    
    try{
      //      return _daughters.at(i) ;
      //FIXME gcc 2.95 doesn't know at(i) ??
      
      return *_daughtersP[i] ;
    }catch( std::out_of_range ){
      throw Exception(std::string("MCParticleImpl::getDaughter(): out_of_range :" 
				  + i ) );
    }
    
  }
//   // unchecked access
//   MCParticleData* MCParticleImpl::getDaughterData(int i) const {
//     return *_daughtersP[i] ;
//   }

  const double* MCParticleImpl::getEndpoint() const { 

    if( _simstatus & 0x80000000 ) // bit 31 ?
      return _endpoint ;
    return 0 ;

  }

  float MCParticleImpl::getEnergy() const { 
    return sqrt( _p[0]*_p[0] + _p[1]*_p[1] + _p[2]*_p[2] + _mass*_mass ) ;  
  }


  int MCParticleImpl::getPDG() const { return _pdg ;}
  int MCParticleImpl::getGeneratorStatus() const { return _genstatus ;}

  int MCParticleImpl::getSimulatorStatus() const {
    // bit 31 reserved for endpoint
    return ( 0x7fffffff & _simstatus ) ;
  }
  
  const double * MCParticleImpl::getVertex() const { return _vertex ;}
  const float * MCParticleImpl::getMomentum() const { return _p ;}
  float MCParticleImpl::getMass() const { return _mass ;}
  float MCParticleImpl::getCharge() const { return _charge ; }

//   void MCParticleImpl::setParent( MCParticle *mom0 ) { 
//     checkAccess("MCParticleImpl::setParent") ;
//     _mother0 = mom0 ; 
//   }
//   void MCParticleImpl::setSecondParent( MCParticle *mom1 ) {
//     checkAccess("MCParticleImpl::setSecondParent") ;
//     _mother1 = mom1 ; 
//   }

  void MCParticleImpl::addDaughter( MCParticle* daughter) { 
    
    checkAccess("MCParticleImpl::addDaughter") ;
    
    MCParticle** pD = new (MCParticle*)  ;
    *pD = daughter ;
    _daughtersP.push_back( pD ) ;
  }

  void MCParticleImpl::addParent( MCParticle* parent) { 

    checkAccess("MCParticleImpl::addParent") ;

    MCParticle** pD = new (MCParticle*)  ;
    *pD = parent ;
    _parentsP.push_back( pD ) ;

    MCParticleImpl* mom = dynamic_cast<MCParticleImpl*>( parent ) ;
    if( mom ) mom->addDaughter( this ) ;
    
  }

  void MCParticleImpl::setPDG(int pdg ) { 
    checkAccess("MCParticleImpl::setPDG") ;
    _pdg = pdg ; 
  }
  void MCParticleImpl::setGeneratorStatus( int status ) { 
    checkAccess("MCParticleImpl::setGeneratorStatus") ;
    _genstatus = status ;
  } 

  void MCParticleImpl::setSimulatorStatus( int status ) { 
    checkAccess("MCParticleImpl::setSimulatorStatus") ;
    // bit 31 reserved for endpioint
    _simstatus |= ( 0x7fffffff  & status )  ;
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
    _simstatus |= 0x80000000 ; // set bit 31 
    _endpoint[0] = endpoint[0] ;
    _endpoint[1] = endpoint[1] ;
    _endpoint[2] = endpoint[2] ;
  }


} ; /// namespace IMPl
