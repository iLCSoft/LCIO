#include "SIO/SIOSimTrackHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/MCParticle.h"
#include "EVENT/SimTrackerHit.h"
#include "IOIMPL/SimTrackerHitIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"

using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOSimTrackHitHandler::read(SIO_stream* stream, 
				      LCObject** objP,
				      unsigned int flag,
				      unsigned int vers ){
    unsigned int status ; 
	
    // create a new object :
    SimTrackerHitIOImpl* hit  = new SimTrackerHitIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_cellID) , 1  ) ;
    SIO_DATA( stream ,    hit->_pos  , 3 ) ;

    SIO_DATA( stream ,  &(hit->_dEdx) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_time) , 1  ) ;

    SIO_PNTR( stream , &(hit->_particle)  ) ;

    // if we want to point at simulated tracker hits we need to add a ptag:
    //  SIO_PTAG( stream , dynamic_cast<const SimTrackerHit*>(hit) ) ;
	
    return ( SIO_BLOCK_SUCCESS ) ;
	
  }
    
    
  unsigned int SIOSimTrackHitHandler::write(SIO_stream* stream, 
				       const LCObject* obj,
				       unsigned int flag ){
    
    unsigned int status ; 
	
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

    //  SIO_PTAG( stream , dynamic_cast<const SimTrackerHit*>(hit) ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
}; // namespace
