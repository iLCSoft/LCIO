
#include "IMPL/MCParticleImpl.h"

#include "EVENT/LCIO.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <math.h> 
#include <algorithm>

using namespace EVENT ;

namespace IMPL {

  MCParticleImpl::MCParticleImpl() :
    _pdg(0),
    _genstatus(0),
    _simstatus(0),
    _mass(0),
    _charge(0),
    _time(0),
    _parents(0),
    _daughters(0)
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
  
//   MCParticleImpl::MCParticleImpl(const EVENT::MCParticle& p) :
//     _pdg( p.getPDG() ),
//     _genstatus( p.getGeneratorStatus() ),
//     _simstatus( p.getSimulatorStatus() ),
//     _mass( p.getMass() ),
//     _charge( p.getCharge() ),
//     _time( p.getTime() ),
//     _parents( p.getParents() ),
//     _daughters( p.getDaughters() )
//   {
//     _vertex[0] = p.getVertex()[0] ;
//     _vertex[1] = p.getVertex()[1] ;
//     _vertex[2] = p.getVertex()[2] ;
//     _p[0] = p.getMomentum()[0] ;
//     _p[1] = p.getMomentum()[1] ;
//     _p[2] = p.getMomentum()[2] ;
//     _endpoint[0] = p.getEndpoint()[0] ;
//     _endpoint[1] = p.getEndpoint()[1] ;
//     _endpoint[2] = p.getEndpoint()[2] ;
//   }
    


  MCParticleImpl::~MCParticleImpl(){
    // no dynamic variables
    //    delete [] _readDaughters ;
//     for(MCParticlePVec::iterator iter = _daughters.begin();iter != _daughters.end() ;iter++){
//       delete (*iter) ;
//     }

  }
  
//   MCParticle * MCParticleImpl::getParent() const { return _mother0 ; } 
//   MCParticle * MCParticleImpl::getParent() const { return _mother0 ; }

//   MCParticle * MCParticleImpl::getSecondParent() const { return _mother1; }  
//   MCParticle * MCParticleImpl::getSecondParent() const { return _mother1; }

//   //  const MCParticleVec * MCParticleImpl::getDaughters() const { return &_daughters ; }


  const MCParticleVec & MCParticleImpl::getParents() const {
    return _parents ;
  }
  
  const MCParticleVec & MCParticleImpl::getDaughters() const {
    return _daughters ;
  }

  int MCParticleImpl::getNumberOfParents() const { 
    static bool first = true ;
    if( first ){
      std::cout << " WARNING >>>>>>>  MCParticleImpl::getNumberOfParents() is deprecated "
		<< " - please use  MCParticleImpl::getParents().size() ! " << std::endl ;
      first = false ;
    }
    return _parents.size() ; 
  }

  MCParticle* MCParticleImpl::getParent(int i) const {
    
    static bool first = true ;
    if( first ){
      std::cout << " WARNING >>>>>>>  MCParticleImpl::getParent(i) is deprecated "
		<< " - please use  MCParticleImpl::getParents()[i] ! " << std::endl ;
      first = false ;
    }
    try{
      //      return _parents.at(i) ;
      //FIXME gcc 2.95 doesn't know at(i) ??
      
      return _parents[i] ;
    }catch( std::out_of_range ){
      throw Exception(std::string("MCParticleImpl::getParent(): out_of_range :" 
				  + i ) );
    }
  }
  
  //   // unchecked access
  //   MCParticle* MCParticleImpl::getParent(int i) const {
  //     return _parents[i] ;
  //   }


  int MCParticleImpl::getNumberOfDaughters() const { 
    static bool first = true ;
    if( first ){
      std::cout << " WARNING >>>>>>>  MCParticleImpl::getNumberOfDaughters() is deprecated "
		<< " - please use  MCParticleImpl::getDaughters().size() ! " << std::endl ;
      first = false ;
    }
    return _daughters.size() ; 
  }

  MCParticle* MCParticleImpl::getDaughter(int i) const {
    
    static bool first = true ;
    if( first ){
      std::cout << " WARNING >>>>>>>  MCParticleImpl::getDaughter(i) is deprecated "
		<< " - please use  MCParticleImpl::getDaughters()[i] ! " << std::endl ;
      first = false ;
    }
    try{
      //      return _daughters.at(i) ;
      //FIXME gcc 2.95 doesn't know at(i) ??
      
      return _daughters[i] ;
    }catch( std::out_of_range ){
      throw Exception(std::string("MCParticleImpl::getDaughter(): out_of_range :" 
				  + i ) );
    }
    
  }
//   // unchecked access
//   MCParticle* MCParticleImpl::getDaughter(int i) const {
//     return *_daughters[i] ;
//   }

  const double* MCParticleImpl::getEndpoint() const { 
    
    if( ! _simstatus.test( BITEndpoint  ) ){
      
      if( _daughters.size() == 0 ) return  _endpoint ;
      
      for(unsigned int i=0;i<_daughters.size();i++)
      {
          if(!_daughters[i]->vertexIsNotEndpointOfParent())
          return _daughters[i]->getVertex();
      }


      return _endpoint ;

    } else 
      return _endpoint ;
  }


  double MCParticleImpl::getEnergy() const { 
    return sqrt( _p[0]*_p[0] + _p[1]*_p[1] + _p[2]*_p[2] + _mass*_mass ) ;  
  }


  int MCParticleImpl::getPDG() const { return _pdg ;}
  int MCParticleImpl::getGeneratorStatus() const { return _genstatus ;}

  int MCParticleImpl::getSimulatorStatus() const {
    // bit 31 reserved for endpoint
    //    return ( 0x7fffffff & _simstatus ) ;
    return _simstatus.to_ulong() ;
  }


  bool MCParticleImpl::isCreatedInSimulation() const      { return   _simstatus[ BITCreatedInSimulation ] ;  }
  bool MCParticleImpl::isBackscatter() const              { return   _simstatus[ BITBackscatter ] ;          }
  bool MCParticleImpl::vertexIsNotEndpointOfParent() const { return  _simstatus[ BITVertexIsNotEndpointOfParent ] ;             }
  bool MCParticleImpl::isDecayedInTracker() const         { return   _simstatus[ BITDecayedInTracker ] ;     }
  bool MCParticleImpl::isDecayedInCalorimeter() const     { return   _simstatus[ BITDecayedInCalorimeter ] ; }
  bool MCParticleImpl::hasLeftDetector() const            { return   _simstatus[ BITLeftDetector ]  ;        }
  bool MCParticleImpl::isStopped() const                  { return   _simstatus[ BITStopped ] ;              }
  


  const double * MCParticleImpl::getVertex() const { return _vertex ;}
  float MCParticleImpl::getTime() const { return _time ; }
  const double * MCParticleImpl::getMomentum() const { return _p ;}
  double MCParticleImpl::getMass() const { return _mass ;}
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
    
//     MCParticle** pD = new (MCParticle*)  ;
//     *pD = daughter ;
    _daughters.push_back( daughter ) ;
  }

  void MCParticleImpl::addParent( MCParticle* parent) { 

    checkAccess("MCParticleImpl::addParent") ;


    if(  std::find(  _parents.begin(), _parents.end(),  parent ) != _parents.end() )
      return ; // parent already exists in list


    _parents.push_back( parent ) ;

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
    // bit 31 reserved for endpoint
    //    _simstatus |= ( 0x7fffffff  & status )  ;
    _simstatus = status ;
  } 

  void MCParticleImpl::setVertex( double vtx[3] ){
    checkAccess("MCParticleImpl::setVertex") ;
    _vertex[0] = vtx[0] ;
    _vertex[1] = vtx[1] ;
    _vertex[2] = vtx[2] ;
  }
  void MCParticleImpl::setTime(float time ) { 
    checkAccess("MCParticleImpl::setTime") ;
    _time = time ; 
  }
  
  void MCParticleImpl::setMomentum( float p[3] ){
    checkAccess("MCParticleImpl::setMomentum") ;
    _p[0] = p[0] ;
    _p[1] = p[1] ;
    _p[2] = p[2] ;
  }
  void MCParticleImpl::setMomentum( double p[3] ){
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

    _simstatus.set( BITEndpoint ) ; 

    _endpoint[0] = endpoint[0] ;
    _endpoint[1] = endpoint[1] ;
    _endpoint[2] = endpoint[2] ;
  }

  void MCParticleImpl::setCreatedInSimulation(bool val)         {   _simstatus[ BITCreatedInSimulation ] = val ;  }
  void MCParticleImpl::setBackscatter(bool val)                 {   _simstatus[ BITBackscatter ]  = val;          }
  void MCParticleImpl::setVertexIsNotEndpointOfParent(bool val) {   _simstatus[ BITVertexIsNotEndpointOfParent ]  = val; }
  void MCParticleImpl::setDecayedInTracker(bool val)            {   _simstatus[ BITDecayedInTracker ]  = val;     }
  void MCParticleImpl::setDecayedInCalorimeter(bool val)        {   _simstatus[ BITDecayedInCalorimeter ]  = val; }
  void MCParticleImpl::setHasLeftDetector(bool val)             {   _simstatus[ BITLeftDetector ]  = val ;        }
  void MCParticleImpl::setStopped(bool val)                     {   _simstatus[ BITStopped ]  = val;              }


} /// namespace IMPl
