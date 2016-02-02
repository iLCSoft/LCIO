
#include "IMPL/MCParticleImpl.h"
#include "UTIL/LCWarning.h"

#include "EVENT/LCIO.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <math.h> 
#include <algorithm>

namespace EVENT{

  // the standard requires static const ints to be defined aoutside the class declaration
  // so we do this here :
  const int MCParticle::BITEndpoint ;
  const int MCParticle::BITCreatedInSimulation ;
  const int MCParticle::BITBackscatter  ;
  const int MCParticle::BITVertexIsNotEndpointOfParent  ;
  const int MCParticle::BITDecayedInTracker  ;
  const int MCParticle::BITDecayedInCalorimeter  ;
  const int MCParticle::BITLeftDetector  ;
  const int MCParticle::BITStopped  ;
  const int MCParticle::BITOverlay  ;
  
}

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
    _daughters(0),
    _endpointSet(false)
  {
    _vertex[0] = 0.0 ;
    _vertex[1] = 0.0 ;
    _vertex[2] = 0.0 ;
    _pEndpoint[0] = 0.0 ;
    _pEndpoint[1] = 0.0 ;
    _pEndpoint[2] = 0.0 ;
    _p[0] = 0.0 ;
    _p[1] = 0.0 ;
    _p[2] = 0.0 ;
    _endpoint[0] = 0.0 ;
    _endpoint[1] = 0.0 ;
    _endpoint[2] = 0.0 ;

    _spin[0] = 0.0 ;
    _spin[1] = 0.0 ;
    _spin[2] = 0.0 ;

    _colorFlow[0] = 0 ;
    _colorFlow[1] = 0 ;
  }

  MCParticleImpl::~MCParticleImpl(){
  }
  
  const MCParticleVec & MCParticleImpl::getParents() const {
    return _parents ;
  }
  
  const MCParticleVec & MCParticleImpl::getDaughters() const {
    return _daughters ;
  }

//  int MCParticleImpl::getNumberOfParents() const { 
//    //static bool first = true ;
//    //if( first ){
//    //  std::cout << " WARNING >>>>>>>  MCParticleImpl::getNumberOfParents() is deprecated "
//	//	<< " - please use  MCParticleImpl::getParents().size() ! " << std::endl ;
//    //  first = false ;
//    //}
//    
//    UTIL::LCWarning::getInstance().printWarning( "MCPARTICLE_DEPRECATED_GETNUMBEROFPARENTS" ) ;
//
//    return _parents.size() ; 
//  }
//
//  MCParticle* MCParticleImpl::getParent(int i) const {
//    
//    //static bool first = true ;
//    //if( first ){
//    //  std::cout << " WARNING >>>>>>>  MCParticleImpl::getParent(i) is deprecated "
//	//	<< " - please use  MCParticleImpl::getParents()[i] ! " << std::endl ;
//    //  first = false ;
//    //}
//
//    UTIL::LCWarning::getInstance().printWarning( "MCPARTICLE_DEPRECATED_GETPARENT" ) ;
//
//    try{
//      return _parents.at(i) ;
//    }catch( std::out_of_range ){
//      std::stringstream err ; err << "MCParticleImpl::getParent(): out_of_range :"  << i  ;
//      throw Exception( err.str() );
//    }
//  }
//  
//  //   // unchecked access
//  //   MCParticle* MCParticleImpl::getParent(int i) const {
//  //     return _parents[i] ;
//  //   }
//
//
//  int MCParticleImpl::getNumberOfDaughters() const { 
//    //static bool first = true ;
//    //if( first ){
//    //  std::cout << " WARNING >>>>>>>  MCParticleImpl::getNumberOfDaughters() is deprecated "
//	//	<< " - please use  MCParticleImpl::getDaughters().size() ! " << std::endl ;
//    //  first = false ;
//    //}
//
//    UTIL::LCWarning::getInstance().printWarning( "MCPARTICLE_DEPRECATED_GETNUMBEROFDAUGHTERS" ) ;
//
//    return _daughters.size() ; 
//  }
//
//  MCParticle* MCParticleImpl::getDaughter(int i) const {
//    
//    UTIL::LCWarning::getInstance().printWarning( "MCPARTICLE_DEPRECATED_GETDAUGHTER" ) ;
//    
//    try{
//      return _daughters.at(i) ;
//    }catch( std::out_of_range ){
//      std::stringstream err ; err << "MCParticleImpl::getDaughter(): out_of_range :"  << i  ;
//      throw Exception( err.str() );
//    }
//    
//  }

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

  const float* MCParticleImpl::getSpin() const { 
    return _spin ;
  }

  const int* MCParticleImpl::getColorFlow() const { 
    return _colorFlow ;
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
  bool MCParticleImpl::isOverlay() const                  { return   _simstatus[ BITOverlay ] ;              }
  


  const double * MCParticleImpl::getVertex() const { return _vertex ;}
  float MCParticleImpl::getTime() const { return _time ; }
  const double * MCParticleImpl::getMomentum() const { return _p ;}
  const double * MCParticleImpl::getMomentumAtEndpoint() const { return _pEndpoint ;}
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

  void MCParticleImpl::setVertex( const double vtx[3] ){
    checkAccess("MCParticleImpl::setVertex") ;
    _vertex[0] = vtx[0] ;
    _vertex[1] = vtx[1] ;
    _vertex[2] = vtx[2] ;
  }
  void MCParticleImpl::setTime(float time ) { 
    checkAccess("MCParticleImpl::setTime") ;
    _time = time ; 
  }
  
  void MCParticleImpl::setMomentum( const float p[3] ){
    checkAccess("MCParticleImpl::setMomentum") ;
    _p[0] = p[0] ;
    _p[1] = p[1] ;
    _p[2] = p[2] ;
  }
  void MCParticleImpl::setMomentum( const double p[3] ){
    checkAccess("MCParticleImpl::setMomentum") ;
    _p[0] = p[0] ;
    _p[1] = p[1] ;
    _p[2] = p[2] ;
  }

  void MCParticleImpl::setMomentumAtEndpoint( const float p[3] ){
    checkAccess("MCParticleImpl::setMomentumAtEndpoint") ;
    _pEndpoint[0] = p[0] ;
    _pEndpoint[1] = p[1] ;
    _pEndpoint[2] = p[2] ;
  }
  void MCParticleImpl::setMomentumAtEndpoint( const double p[3] ){
    checkAccess("MCParticleImpl::setMomentumAtEndpoint") ;
    _pEndpoint[0] = p[0] ;
    _pEndpoint[1] = p[1] ;
    _pEndpoint[2] = p[2] ;
  }

  void MCParticleImpl::setMass( float m ) { 
    checkAccess("MCParticleImpl::setMass") ;
    _mass = m ; 
  } 
  void MCParticleImpl::setCharge( float c ) { 
    checkAccess("MCParticleImpl::setCharge") ;
    _charge = c ;  
  } 

  void MCParticleImpl::setEndpoint( const double endpoint[3] ){
    checkAccess("MCParticleImpl::setEndpoint") ;

    _simstatus.set( BITEndpoint ) ; 

    _endpoint[0] = endpoint[0] ;
    _endpoint[1] = endpoint[1] ;
    _endpoint[2] = endpoint[2] ;
  }

  void MCParticleImpl::setSpin( const float spin[3] ){
    checkAccess("MCParticleImpl::setSpin") ;
    _spin[0] = spin[0] ;
    _spin[1] = spin[1] ;
    _spin[2] = spin[2] ;
  }

  void MCParticleImpl::setColorFlow( const int cflow[2] ){
    checkAccess("MCParticleImpl::setColorFlow") ;
    _colorFlow[0] = cflow[0] ;
    _colorFlow[1] = cflow[1] ;
  }
  
  void MCParticleImpl::setCreatedInSimulation(bool val)         {   _simstatus[ BITCreatedInSimulation ] = val ;  }
  void MCParticleImpl::setBackscatter(bool val)                 {   _simstatus[ BITBackscatter ]  = val;          }
  void MCParticleImpl::setVertexIsNotEndpointOfParent(bool val) {   _simstatus[ BITVertexIsNotEndpointOfParent ]  = val; }
  void MCParticleImpl::setDecayedInTracker(bool val)            {   _simstatus[ BITDecayedInTracker ]  = val;     }
  void MCParticleImpl::setDecayedInCalorimeter(bool val)        {   _simstatus[ BITDecayedInCalorimeter ]  = val; }
  void MCParticleImpl::setHasLeftDetector(bool val)             {   _simstatus[ BITLeftDetector ]  = val ;        }
  void MCParticleImpl::setStopped(bool val)                     {   _simstatus[ BITStopped ]  = val;              }
  void MCParticleImpl::setOverlay(bool val)                     {   _simstatus[ BITOverlay ]  = val;              }


} /// namespace IMPl
