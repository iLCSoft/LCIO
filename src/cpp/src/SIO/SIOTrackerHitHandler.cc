#include "SIO/SIOTrackerHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/MCParticle.h"
#include "EVENT/TrackerHit.h"
#include "IOIMPL/TrackerHitIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOTrackerHitHandler::read(SIO_stream* stream, 
				      LCObject** objP,
				      unsigned int flag,
				      unsigned int vers ){
    unsigned int status ; 
	
    // create a new object :
    TrackerHitIOImpl* hit  = new TrackerHitIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,    hit->_pos  , 3 ) ;

    float cov[NCOVMATRIX] ;
    SIO_DATA( stream ,  cov  ,  NCOVMATRIX ) ;
    hit->setCovMatrix( cov ) ;

    SIO_DATA( stream ,  &(hit->_dEdx) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_time) , 1  ) ;

    SIO_PNTR( stream , &(hit->_rawHit)  ) ;

//     // read the pointer tag in case we want to point to hits in the future
//     SIO_PTAG( stream , dynamic_cast<const TrackerHit*>(hit) ) ;
	
    return ( SIO_BLOCK_SUCCESS ) ;
	
  }
    
    
  unsigned int SIOTrackerHitHandler::write(SIO_stream* stream, 
				       const LCObject* obj,
				       unsigned int flag ){
    
    unsigned int status ; 

    const TrackerHit* hit = dynamic_cast<const TrackerHit*>(obj)  ;

    // as SIO doesn't provide a write function with const arguments
    // we have to cast away the constness 
    double* pos = const_cast<double*> ( hit->getPosition() ) ; 
    SIO_DATA( stream,  pos , 3 ) ;

    const FloatVec& cov = hit->getCovMatrix() ;
    for(unsigned int i=0;i<cov.size();i++){
      LCSIO_WRITE( stream, cov[i]  ) ;
    }

    LCSIO_WRITE( stream, hit->getdEdx()  ) ;
    LCSIO_WRITE( stream, hit->getTime()  ) ;
    
    const LCObject* raw = hit->getRawDataHit()  ;
    SIO_PNTR( stream , &raw ) ;

//     // write a ptag in order to be able to point to tracker hits in the future
//     SIO_PTAG( stream , hit ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
}; // namespace
