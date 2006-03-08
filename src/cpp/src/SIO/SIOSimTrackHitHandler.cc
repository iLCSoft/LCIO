#include "SIO/SIOSimTrackHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/MCParticle.h"
#include "EVENT/SimTrackerHit.h"
#include "IOIMPL/SimTrackerHitIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"

#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"

using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOSimTrackHitHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	
    // create a new object :
    SimTrackerHitIOImpl* hit  = new SimTrackerHitIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_cellID) , 1  ) ;
    SIO_DATA( stream ,    hit->_pos  , 3 ) ;

    SIO_DATA( stream ,  &(hit->_dEdx) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_time) , 1  ) ;

    SIO_PNTR( stream , &(hit->_particle)  ) ;

    if( LCFlagImpl(_flag).bitSet( LCIO::THBIT_MOMENTUM ) ){
      SIO_DATA( stream ,    hit->_p  , 3 ) ;
      if( _vers  > SIO_VERSION_ENCODE( 1 , 6 ) ) 
        SIO_DATA( stream ,    &(hit->_pathLength)  , 1 ) ;
    }

    // read the pointer tag in case we want to point to hits
    if( _vers > SIO_VERSION_ENCODE( 1, 0) ) {
      SIO_PTAG( stream , dynamic_cast<const SimTrackerHit*>(hit) ) ;
    }
	
    return ( SIO_BLOCK_SUCCESS ) ;
	
  }
    
    
  unsigned int SIOSimTrackHitHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 
    // fg 20030609 changed to use SimTrackerHit
    
    // this is where we gave up type safety in order to
    // simplify the API and the implementation
    // by having a common collection of objects
    const SimTrackerHit* hit = dynamic_cast<const SimTrackerHit*>(obj)  ;

    LCSIO_WRITE( stream, hit->getCellID()  ) ;
    // as SIO doesn't provide a write function with const arguments
    // we have to cast away the constness 
    double* pos = const_cast<double*> ( hit->getPosition() ) ; 
    SIO_DATA( stream,  pos , 3 ) ;
    LCSIO_WRITE( stream, hit->getdEdx()  ) ;
    LCSIO_WRITE( stream, hit->getTime()  ) ;
    
    const MCParticle* part = hit->getMCParticle()  ;
    SIO_PNTR( stream , &part ) ;

    if( LCFlagImpl(_flag).bitSet( LCIO::THBIT_MOMENTUM ) ){
      float* p = const_cast<float*> ( hit->getMomentum() ) ; 
      SIO_DATA( stream , p  , 3 ) ;
      LCSIO_WRITE( stream , hit->getPathLength() ) ;
    }

    // write a ptag in order to be able to point to tracker hits - from v1.1
    SIO_PTAG( stream , hit ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
} // namespace
