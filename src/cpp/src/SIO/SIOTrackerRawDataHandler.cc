#include "SIO/SIOTrackerRawDataHandler.h"

#include "EVENT/TrackerRawData.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/TrackerRawDataIOImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOTrackerRawDataHandler::SIOTrackerRawDataHandler() :
    SIOObjectHandler( EVENT::LCIO::TRACKERRAWDATA ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOTrackerRawDataHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type /*vers*/ ) {
    auto hit  = dynamic_cast<IOIMPL::TrackerRawDataIOImpl*>(objP) ;
    SIO_DATA( device ,  &(hit->_cellID0) , 1  ) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    if( lcFlag.bitSet( EVENT::LCIO::TRAWBIT_ID1 ) ) {
      SIO_DATA( device ,  &(hit->_cellID1) , 1  ) ;
    }
    SIO_DATA( device ,  &(hit->_time) , 1  ) ;
    SIO_SDATA( device ,  hit->_adc ) ;
    SIO_PTAG( device , dynamic_cast<const EVENT::TrackerRawData*>(hit) ) ;
  }

  //----------------------------------------------------------------------------

  void SIOTrackerRawDataHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto hit = dynamic_cast<const EVENT::TrackerRawData*>(obj)  ;
    SIO_SDATA( device, hit->getCellID0()  ) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    if( lcFlag.bitSet( EVENT::LCIO::TRAWBIT_ID1 ) ) {
      SIO_SDATA( device, hit->getCellID1()  ) ;
    }
    SIO_SDATA( device, hit->getTime()  ) ;
    SIO_SDATA( device, hit->getADCValues()  ) ;
    SIO_PTAG( device , hit ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOTrackerRawDataHandler::create() const {
    return new IOIMPL::TrackerRawDataIOImpl() ;
  }

} // namespace
