#include "SIO/SIOTrackerPulseHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/TrackerPulse.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/TrackerPulseIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOTrackerPulseHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	
    // create a new object :
    TrackerPulseIOImpl* hit  = new TrackerPulseIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_cellID0) , 1  ) ;
    
    LCFlagImpl lcFlag(_flag) ;
    if( lcFlag.bitSet( LCIO::TRAWBIT_ID1 ) )
      SIO_DATA( stream ,  &(hit->_cellID1) , 1  ) ;

    SIO_DATA( stream ,  &(hit->_time) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_charge )  , 1  ) ;
    if( _vers > SIO_VERSION_ENCODE( 1, 12 )   ){

        //SIO_DATA( stream ,  &(hit->_timeError) , 1  ) ;
        //SIO_DATA( stream ,  &(hit->_chargeError )  , 1  ) ;

        if( lcFlag.bitSet( LCIO::TRAWBIT_CM ) ){
            float cov[TRKPULSENCOVMATRIX] ;
            SIO_DATA( stream ,  cov  ,  TRKPULSENCOVMATRIX ) ;
            hit->setCovMatrix( cov ) ;
        }
    }
    SIO_DATA( stream ,  &(hit->_quality )  , 1  ) ;
    
    SIO_PNTR( stream , &(hit->_corrData) ) ;
    
    SIO_PTAG( stream , dynamic_cast<const TrackerPulse*>(hit) ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOTrackerPulseHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 

    const TrackerPulse* hit = dynamic_cast<const TrackerPulse*>(obj)  ;

    LCSIO_WRITE( stream, hit->getCellID0()  ) ;
      
    LCFlagImpl lcFlag(_flag) ;
    if( lcFlag.bitSet( LCIO::TRAWBIT_ID1 ) )
      LCSIO_WRITE( stream, hit->getCellID1()  ) ;
    LCSIO_WRITE( stream, hit->getTime()  ) ;
    LCSIO_WRITE( stream, hit->getCharge()  ) ;
    //LCSIO_WRITE( stream, hit->getTimeError()  ) ;
    //LCSIO_WRITE( stream, hit->getChargeError()  ) ;
    if( lcFlag.bitSet( LCIO::TRAWBIT_CM ) ){
        const FloatVec& cov = hit->getCovMatrix() ;
        for(unsigned int i=0; i<cov.size(); i++){
            LCSIO_WRITE( stream, cov[i]  ) ;
        }
    }
    LCSIO_WRITE( stream, hit->getQuality()  ) ;

    TrackerData* corr = hit->getTrackerData() ;
    SIO_PNTR( stream ,  & corr ); 

    
    SIO_PTAG( stream , hit ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
