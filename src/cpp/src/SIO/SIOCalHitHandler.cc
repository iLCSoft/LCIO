#include "SIO/SIOCalHitHandler.h"

// -- lcio headers
#include "EVENT/LCIO.h"
#include "EVENT/CalorimeterHit.h"
#include "IOIMPL/CalorimeterHitIOImpl.h"
#include "IMPL/LCFlagImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOCalHitHandler::SIOCalHitHandler() :
    SIOObjectHandler( EVENT::LCIO::CALORIMETERHIT ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOCalHitHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    IMPL::LCFlagImpl lcFlag( _flag ) ;
    IOIMPL::CalorimeterHitIOImpl *hit = dynamic_cast<IOIMPL::CalorimeterHitIOImpl *>(objP) ;
    SIO_DATA( device ,  &(hit->_cellID0) , 1  ) ;
    // in v00-08 cellid1 has been stored by default
    if( lcFlag.bitSet( EVENT::LCIO::RCHBIT_ID1 ) || ( SIO_VERSION_MAJOR(vers)==0 && SIO_VERSION_MINOR(vers)==8) ) {
      SIO_DATA( device ,  &(hit->_cellID1) , 1  ) ;
    }
    SIO_DATA( device ,  &(hit->_energy) , 1  ) ;
    if( vers > SIO_VERSION_ENCODE( 1, 9 ) && lcFlag.bitSet( EVENT::LCIO::RCHBIT_ENERGY_ERROR )) {
        SIO_DATA( device ,  &(hit->_energyError) , 1  ) ;
    }
    if( vers > SIO_VERSION_ENCODE( 1, 2) && lcFlag.bitSet( EVENT::LCIO::RCHBIT_TIME ) ) {
      SIO_DATA( device ,  &(hit->_time) , 1  ) ;
    }
    if( lcFlag.bitSet( EVENT::LCIO::RCHBIT_LONG ) ) {
      SIO_DATA( device ,  &hit->_position[0]  , 3 ) ;
    }
    if( vers > SIO_VERSION_ENCODE( 1, 2)   ) {
      SIO_DATA( device ,  &(hit->_type) , 1  ) ;
      SIO_PNTR( device , &(hit->_rawHit)  ) ;
    }
    // read a pointer tag for future reference to calorimeter hits
    if( vers > SIO_VERSION_ENCODE( 1, 2) ) {    // the logic of the pointer bit has been inverted in v1.3
      if( ! lcFlag.bitSet( EVENT::LCIO::RCHBIT_NO_PTR )  ) {
	      SIO_PTAG( device , dynamic_cast<const EVENT::CalorimeterHit*>(hit) ) ;
      }
    }
    else {
      if( lcFlag.bitSet( EVENT::LCIO::RCHBIT_NO_PTR )  ) {
	      SIO_PTAG( device , dynamic_cast<const EVENT::CalorimeterHit*>(hit) ) ;
      }
    }
  }

  //----------------------------------------------------------------------------

  void SIOCalHitHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    const EVENT::CalorimeterHit* hit = dynamic_cast<const EVENT::CalorimeterHit*>(obj)  ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    SIO_SDATA( device, hit->getCellID0() ) ;
    if( lcFlag.bitSet( EVENT::LCIO::RCHBIT_ID1 ) ) {
      SIO_SDATA( device, hit->getCellID1()  ) ;
    }
    SIO_SDATA( device, hit->getEnergy()  ) ;
    if( lcFlag.bitSet( EVENT::LCIO::RCHBIT_ENERGY_ERROR ) ) {
        SIO_SDATA( device, hit->getEnergyError()  ) ;
    }
    if( lcFlag.bitSet( EVENT::LCIO::RCHBIT_TIME ) ) {
      SIO_SDATA( device ,  hit->getTime() ) ;
    }
    if( lcFlag.bitSet( EVENT::LCIO::RCHBIT_LONG ) ) {
      SIO_DATA( device,  hit->getPosition() , 3 ) ;
    }
    SIO_SDATA( device ,  hit->getType() ) ;
    EVENT::LCObject *rawHit = hit->getRawHit() ;
    SIO_PNTR( device , &rawHit ) ;
    if( !lcFlag.bitSet( EVENT::LCIO::RCHBIT_NO_PTR ) ) {
      SIO_PTAG( device , hit  ) ;
    }
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOCalHitHandler::create() const {
    return new IOIMPL::CalorimeterHitIOImpl() ;
  }

} // namespace
