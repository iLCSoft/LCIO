#include "SIO/SIOTrackerHitZCylinderHandler.h"

// -- lcio headers
#include "EVENT/LCIO.h"
#include "EVENT/TrackerHitZCylinder.h"
#include "IOIMPL/TrackerHitZCylinderIOImpl.h"
#include "IMPL/LCFlagImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOTrackerHitZCylinderHandler::SIOTrackerHitZCylinderHandler() :
    SIOObjectHandler( EVENT::LCIO::TRACKERHITZCYLINDER ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOTrackerHitZCylinderHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    auto hit = dynamic_cast<IOIMPL::TrackerHitZCylinderIOImpl*>(objP) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    if(vers > SIO_VERSION_ENCODE( 1, 51) ) {
      SIO_DATA( device ,  &(hit->_cellID0) , 1  ) ;
      if( lcFlag.bitSet( EVENT::LCIO::RTHZBIT_ID1 ) ){
        SIO_DATA( device ,  &(hit->_cellID1) , 1  ) ;
      }
    }
    SIO_DATA( device, &(hit->_type) , 1 ) ;
    SIO_DATA( device, hit->_pos     , 3 ) ;
    SIO_DATA( device, hit->_center  , 2 ) ;
    SIO_DATA( device, &(hit->_drphi), 1 ) ;
    SIO_DATA( device, &(hit->_dz)   , 1 ) ;
    SIO_DATA( device, &(hit->_EDep) , 1 ) ;
    SIO_DATA( device, &(hit->_EDepError) , 1 ) ;
    SIO_DATA( device, &(hit->_time) , 1 ) ;
    SIO_DATA( device, &(hit->_quality) , 1 ) ;
    int numberOfRawHits = 1 ;
    SIO_DATA( device ,  &numberOfRawHits , 1  ) ;
    hit->_rawHits.resize( numberOfRawHits ) ;
    for( int i=0 ; i<numberOfRawHits ; i++ ) {
      SIO_PNTR( device , &(hit->_rawHits[i] ) ) ;
    }
    SIO_PTAG( device , dynamic_cast<const EVENT::TrackerHitZCylinder*>(hit) ) ;
  }

  //----------------------------------------------------------------------------

  void SIOTrackerHitZCylinderHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto hit = dynamic_cast<const EVENT::TrackerHitZCylinder*>(obj)  ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    SIO_SDATA( device, hit->getCellID0()  ) ;
    if( lcFlag.bitSet( EVENT::LCIO::RTHZBIT_ID1 ) ) {
      SIO_SDATA( device, hit->getCellID1()  ) ;
    }
    SIO_SDATA( device ,  hit->getType() ) ;
    SIO_DATA( device,  hit->getPosition() , 3 ) ;
    SIO_DATA( device,  hit->getCenter() , 2 ) ;
    SIO_SDATA( device, hit->getdRPhi()  ) ;
    SIO_SDATA( device, hit->getdZ()  ) ;
    SIO_SDATA( device, hit->getEDep()  ) ;
    SIO_SDATA( device, hit->getEDepError()  ) ;
    SIO_SDATA( device, hit->getTime()  ) ;
    SIO_SDATA( device, hit->getQuality()  ) ;
    const EVENT::LCObjectVec& rawHits = hit->getRawHits() ;
    int nrawhits = rawHits.size() ;
    SIO_SDATA( device, nrawhits ) ;
    for(unsigned int i=0; i < rawHits.size() ; i++){
      SIO_PNTR( device , &(rawHits[i]) ) ;
    }
    SIO_PTAG( device , hit ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOTrackerHitZCylinderHandler::create() const {
    return new IOIMPL::TrackerHitZCylinderIOImpl() ;
  }

} // namespace
