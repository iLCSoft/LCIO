#include "SIO/SIOTrackerDataHandler.h"

// -- lcio headers
#include "EVENT/TrackerData.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/TrackerDataIOImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOTrackerDataHandler::SIOTrackerDataHandler() :
    SIOObjectHandler( EVENT::LCIO::TRACKERDATA ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOTrackerDataHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type /*vers*/ ) {
    auto hit = dynamic_cast<IOIMPL::TrackerDataIOImpl*>( objP ) ;
    SIO_DATA( device ,  &(hit->_cellID0) , 1  ) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    if( lcFlag.bitSet( EVENT::LCIO::TRAWBIT_ID1 ) ) {
      SIO_DATA( device ,  &(hit->_cellID1) , 1  ) ;
    }
    SIO_DATA( device ,  &(hit->_time) , 1  ) ;
    SIO_SDATA( device , hit->_charge  ) ;
    SIO_PTAG( device , dynamic_cast<const EVENT::TrackerData*>(hit) ) ;
  }

  //----------------------------------------------------------------------------

  void SIOTrackerDataHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto hit = dynamic_cast<const EVENT::TrackerData*>(obj)  ;
    SIO_SDATA( device, hit->getCellID0()  ) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    if( lcFlag.bitSet( EVENT::LCIO::TRAWBIT_ID1 ) ) {
      SIO_SDATA( device, hit->getCellID1()  ) ;
    }
    SIO_SDATA( device, hit->getTime()  ) ;
    SIO_SDATA( device,  hit->getChargeValues() ) ;
    SIO_PTAG( device , hit ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOTrackerDataHandler::create() const {
    return new IOIMPL::TrackerDataIOImpl() ;
  }

} // namespace
