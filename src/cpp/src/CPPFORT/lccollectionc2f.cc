
#include "CPPFORT/lccollectionc2f.h"

#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/MCParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/CalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"
#include "EVENT/TPCHit.h"
#include "EVENT/LCIO.h"

#include <iostream>

using namespace lcio ;
using namespace std ;


#define LCEVENT_PNTR( event ) if(! (event) ) return LCIO::ERROR ; \
LCEvent* lcEvent = reinterpret_cast<LCEvent*>( (event) ) ;

#define LCCOLLECTION_PNTR( collection ) if(! (collection) ) return LCIO::ERROR ; \
LCCollection* lcCollection = reinterpret_cast<LCCollection*>( (collection) ) ;

#define LCOBJECT_PNTR( object ) if(! (object) ) return LCIO::ERROR ; \
LCObject* lcObject = reinterpret_cast<LCObject*>( (object) ) ;

PTRTYPE LCCreateCollectionVec( const char* colname ){
  const char* TYPENAME = colname ;

  if      ( TYPENAME == string( "MCPARTICLE" )       ){
    LCCollectionVec* lcCollection = new LCCollectionVec( LCIO::MCPARTICLE ) ;
    return reinterpret_cast<PTRTYPE>( lcCollection ) ;
  }
  else if ( TYPENAME == string( "SIMCALORIMETERHIT" )){
    LCCollectionVec* lcCollection = new LCCollectionVec( LCIO::SIMCALORIMETERHIT ) ;
    return reinterpret_cast<PTRTYPE>( lcCollection ) ;
  }
  else if ( TYPENAME == string( "CALORIMETERHIT" )   ){
    LCCollectionVec* lcCollection = new LCCollectionVec( LCIO::CALORIMETERHIT ) ;
    return reinterpret_cast<PTRTYPE>( lcCollection ) ;
  }
  else if ( TYPENAME == string( "SIMTRACKERHIT" )    ){
    LCCollectionVec* lcCollection = new LCCollectionVec( LCIO::SIMTRACKERHIT ) ;
    return reinterpret_cast<PTRTYPE>( lcCollection ) ;
  }
  else if ( TYPENAME == string( "TPCHIT" )           ){
    LCCollectionVec* lcCollection = new LCCollectionVec( LCIO::TPCHIT ) ;
    return reinterpret_cast<PTRTYPE>( lcCollection ) ;
  }
  else if ( TYPENAME == string( "LCFLOATVEC" )       ){
    LCCollectionVec* lcCollection = new LCCollectionVec( LCIO::LCFLOATVEC ) ;
    return reinterpret_cast<PTRTYPE>( lcCollection ) ;
  }
  else if ( TYPENAME == string( "LCINTVEC" )         ){
    LCCollectionVec* lcCollection = new LCCollectionVec( LCIO::LCINTVEC  ) ;
    return reinterpret_cast<PTRTYPE>( lcCollection ) ;
  }
  else return LCIO::ERROR ;

}

PTRTYPE LCAddElement( PTRTYPE collection, PTRTYPE object ){
  LCCOLLECTION_PNTR( collection ) ;
  LCOBJECT_PNTR( object ) ;
  lcCollection->addElement( lcObject ) ;
  return LCIO::SUCCESS ;
}

PTRTYPE LCGetElement( PTRTYPE collection, int NElement ){

/*
the following does not give correct results for the old 2.95.3 compiler
but it yields correct values for gcc 3.3
       LCCOLLECTION_PNTR( collection ) ;
       return reinterpret_cast<PTRTYPE>( lcCollection->getElementAt( NElement - 1 ) ) ;
therefore one has to cast to the specific object !
*/

  if(! (collection) ) return LCIO::ERROR ;
  LCCollection* lcCollection = reinterpret_cast<LCCollection*>( (collection) ) ;
  if      ( lcCollection->getTypeName() == LCIO::MCPARTICLE ){
    MCParticle* lcObject = dynamic_cast<MCParticle*>( lcCollection->getElementAt( NElement - 1 ) ) ;
    return reinterpret_cast<PTRTYPE>( lcObject ) ;
  }
  else if ( lcCollection->getTypeName() == LCIO::SIMCALORIMETERHIT ){
    SimCalorimeterHit* lcObject = dynamic_cast<SimCalorimeterHit*>( lcCollection->getElementAt( NElement - 1 ) ) ;
    return reinterpret_cast<PTRTYPE>( lcObject ) ;
  }
  else if ( lcCollection->getTypeName() == LCIO::CALORIMETERHIT ){
    CalorimeterHit* lcObject = dynamic_cast<CalorimeterHit*>( lcCollection->getElementAt( NElement - 1 ) ) ;
    return reinterpret_cast<PTRTYPE>( lcObject ) ;
  }
  else if ( lcCollection->getTypeName() == LCIO::SIMTRACKERHIT ){
    SimTrackerHit* lcObject = dynamic_cast<SimTrackerHit*>( lcCollection->getElementAt( NElement - 1 ) ) ;
    return reinterpret_cast<PTRTYPE>( lcObject ) ;
  }
  else if ( lcCollection->getTypeName() == LCIO::TPCHIT ){
    TPCHit* lcObject = dynamic_cast<TPCHit*>( lcCollection->getElementAt( NElement - 1 ) ) ;
    return reinterpret_cast<PTRTYPE>( lcObject ) ;
  }
  else return LCIO::ERROR ;
}

int LCGetNumberOfElements ( PTRTYPE collection ){
  LCCOLLECTION_PNTR( collection ) ;
  return lcCollection->getNumberOfElements() ;
}

char* LCGetTypeName ( PTRTYPE collection ){
  const LCCollection* lcCollection = reinterpret_cast<LCCollection*>( (collection) ) ;
  return const_cast<char*>( lcCollection->getTypeName().c_str() ) ;
}

char* LCGetName( const char* mytypename ){
  const char* TYPENAME = mytypename ;
  if      ( TYPENAME == string( "MCPARTICLE" )       ) return const_cast<char*>( LCIO::MCPARTICLE ) ;
  else if ( TYPENAME == string( "SIMCALORIMETERHIT" )) return const_cast<char*>( LCIO::SIMCALORIMETERHIT ) ;
  else if ( TYPENAME == string( "CALORIMETERHIT" )   ) return const_cast<char*>( LCIO::CALORIMETERHIT ) ;
  else if ( TYPENAME == string( "SIMTRACKERHIT" )    ) return const_cast<char*>( LCIO::SIMTRACKERHIT ) ;
  else if ( TYPENAME == string( "TPCHIT" )           ) return const_cast<char*>( LCIO::TPCHIT ) ;
  else if ( TYPENAME == string( "LCFLOATVEC" )       ) return const_cast<char*>( LCIO::LCFLOATVEC ) ;
  else if ( TYPENAME == string( "LCINTVEC" )         ) return const_cast<char*>( LCIO::LCINTVEC  ) ;
  else return "undefined" ;
}

