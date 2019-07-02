#include "SIO/SIOTPCHitHandler.h"

// -- lcio headers
#include "EVENT/TPCHit.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/TPCHitIOImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOTPCHitHandler::SIOTPCHitHandler() :
    SIOObjectHandler( EVENT::LCIO::TPCHIT ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOTPCHitHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    auto hit = dynamic_cast<IOIMPL::TPCHitIOImpl*>( objP ) ;
    SIO_DATA( device ,  &(hit->_cellID) , 1  ) ;
    SIO_DATA( device ,  &(hit->_time) , 1  ) ;
    SIO_DATA( device ,  &(hit->_charge) , 1  ) ;
    SIO_DATA( device ,  &(hit->_quality) , 1  ) ;
    IMPL::LCFlagImpl lcflag(_flag) ;
    if( lcflag.bitSet( EVENT::LCIO::TPCBIT_RAW ) ){
      SIO_DATA( device ,  &(hit->_rawSize)  , 1 ) ;
      hit->initRawArray( hit->_rawSize ) ; // reserve enough space for raw data
      SIO_DATA( device ,  hit->_rawArray  , hit->_rawSize  ) ;
    }
    if( vers > SIO_VERSION_ENCODE( 1, 2 ) ) {    // the logic of the pointer bit has been inverted in v1.3
      if( ! lcflag.bitSet( EVENT::LCIO::TPCBIT_NO_PTR )  ) {
	     SIO_PTAG( device , dynamic_cast<const EVENT::TPCHit*>(hit) ) ;
     }
    }
    else {
      if( lcflag.bitSet( EVENT::LCIO::TPCBIT_NO_PTR )  ) {
	      SIO_PTAG( device , dynamic_cast<const EVENT::TPCHit*>(hit) ) ;
      }
    }
  }

  //----------------------------------------------------------------------------

  void SIOTPCHitHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto hit = dynamic_cast<const EVENT::TPCHit*>(obj)  ;
    SIO_SDATA( device, hit->getCellID()  ) ;
    SIO_SDATA( device, hit->getTime()  ) ;
    SIO_SDATA( device, hit->getCharge()  ) ;
    SIO_SDATA( device, hit->getQuality()  ) ;
    IMPL::LCFlagImpl lcflag(_flag) ;
    if( lcflag.bitSet( EVENT::LCIO::TPCBIT_RAW ) ) {
      SIO_SDATA( device, hit->getNRawDataWords()  ) ;
      for(int i=0;i<hit->getNRawDataWords();i++){
	       SIO_SDATA( device, hit->getRawDataWord(i)  ) ;
      }
    }
    //  add a pointer tag for reference to TPC hits from generic hits
    if( ! lcflag.bitSet( EVENT::LCIO::TPCBIT_NO_PTR ) ) {
      SIO_PTAG( device , hit ) ;
    }
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOTPCHitHandler::create() const {
    return new IOIMPL::TPCHitIOImpl() ;
  }

} // namespace
