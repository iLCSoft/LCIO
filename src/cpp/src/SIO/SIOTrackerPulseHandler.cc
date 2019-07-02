#include "SIO/SIOTrackerPulseHandler.h"

// -- lcio headers
#include "EVENT/TrackerPulse.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/TrackerPulseIOImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOTrackerPulseHandler::SIOTrackerPulseHandler() :
    SIOObjectHandler( EVENT::LCIO::TRACKERPULSE ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOTrackerPulseHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    auto hit  = dynamic_cast<IOIMPL::TrackerPulseIOImpl*>(objP) ;
    SIO_DATA( device ,  &(hit->_cellID0) , 1  ) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    if( lcFlag.bitSet( EVENT::LCIO::TRAWBIT_ID1 ) ) {
      SIO_DATA( device ,  &(hit->_cellID1) , 1  ) ;
    }
    SIO_DATA( device ,  &(hit->_time) , 1  ) ;
    SIO_DATA( device ,  &(hit->_charge )  , 1  ) ;
    if( vers > SIO_VERSION_ENCODE( 1, 12 )   ) {
      if( lcFlag.bitSet( EVENT::LCIO::TRAWBIT_CM ) ) {
        float cov[TRKPULSENCOVMATRIX] ;
        SIO_DATA( device ,  cov  ,  TRKPULSENCOVMATRIX ) ;
        hit->setCovMatrix( cov ) ;
      }
    }
    SIO_DATA( device ,  &(hit->_quality )  , 1  ) ;
    SIO_PNTR( device , &(hit->_corrData) ) ;
    SIO_PTAG( device , dynamic_cast<const EVENT::TrackerPulse*>(hit) ) ;
  }

  //----------------------------------------------------------------------------

  void SIOTrackerPulseHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto hit = dynamic_cast<const EVENT::TrackerPulse*>(obj)  ;
    SIO_SDATA( device, hit->getCellID0()  ) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    if( lcFlag.bitSet( EVENT::LCIO::TRAWBIT_ID1 ) ) {
      SIO_SDATA( device, hit->getCellID1()  ) ;
    }
    SIO_SDATA( device, hit->getTime()  ) ;
    SIO_SDATA( device, hit->getCharge()  ) ;
    if( lcFlag.bitSet( EVENT::LCIO::TRAWBIT_CM ) ) {
      auto cov = hit->getCovMatrix() ;
      for(unsigned int i=0; i<cov.size(); i++){
        SIO_SDATA( device, cov[i]  ) ;
      }
    }
    SIO_SDATA( device, hit->getQuality()  ) ;
    auto corr = hit->getTrackerData() ;
    SIO_PNTR( device ,  & corr );
    SIO_PTAG( device , hit ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOTrackerPulseHandler::create() const {
    return new IOIMPL::TrackerPulseIOImpl() ;
  }

} // namespace
