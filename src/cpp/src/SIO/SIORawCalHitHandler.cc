#include "SIO/SIORawCalHitHandler.h"

// -- lcio headers
#include "EVENT/LCIO.h"
#include "EVENT/RawCalorimeterHit.h"
#include "IOIMPL/RawCalorimeterHitIOImpl.h"
#include "IMPL/LCFlagImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIORawCalHitHandler::SIORawCalHitHandler() :
    SIOObjectHandler( EVENT::LCIO::RAWCALORIMETERHIT ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIORawCalHitHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    auto hit = dynamic_cast<IOIMPL::RawCalorimeterHitIOImpl*>( objP ) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    SIO_DATA( device ,  &(hit->_cellID0) , 1  ) ;
    if( lcFlag.bitSet( EVENT::LCIO::RCHBIT_ID1 ) || vers == SIO_VERSION_ENCODE( 0, 8 ) ) {  // in v00-08 cellid1 has been stored by default
      SIO_DATA( device ,  &(hit->_cellID1) , 1  ) ;
    }
    SIO_DATA( device ,  &(hit->_amplitude) , 1  ) ;
    if( lcFlag.bitSet( EVENT::LCIO::RCHBIT_TIME ) ){
      SIO_DATA( device ,  &(hit->_timeStamp)  , 1 ) ;
    }
    if( ! lcFlag.bitSet( EVENT::LCIO::RCHBIT_NO_PTR )  ) {
      SIO_PTAG( device , dynamic_cast<const EVENT::RawCalorimeterHit*>(hit) ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIORawCalHitHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto hit = dynamic_cast<const EVENT::RawCalorimeterHit*>(obj)  ;
    SIO_SDATA( device, hit->getCellID0()  ) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    if( lcFlag.bitSet( EVENT::LCIO::RCHBIT_ID1 ) ){
      SIO_SDATA( device, hit->getCellID1()  ) ;
    }
    SIO_SDATA( device, hit->getAmplitude()  ) ;
    if( lcFlag.bitSet( EVENT::LCIO::RCHBIT_TIME ) ){
      SIO_SDATA( device, hit->getTimeStamp()  ) ;
    }
    if( ! lcFlag.bitSet( EVENT::LCIO::RCHBIT_NO_PTR ) ){
      SIO_PTAG( device , hit  ) ;
    }
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIORawCalHitHandler::create() const {
    return new IOIMPL::RawCalorimeterHitIOImpl() ;
  }

} // namespace
