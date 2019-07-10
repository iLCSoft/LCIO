#include "SIO/SIOTrackerHitPlaneHandler.h"

// -- lcio headers
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "EVENT/TrackerHitPlane.h"
#include "IOIMPL/TrackerHitPlaneIOImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOTrackerHitPlaneHandler::SIOTrackerHitPlaneHandler() :
    SIOObjectHandler( EVENT::LCIO::TRACKERHITPLANE ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOTrackerHitPlaneHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    auto hit = dynamic_cast<IOIMPL::TrackerHitPlaneIOImpl*>(objP) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    if( vers > SIO_VERSION_ENCODE( 1, 51) ) {
      SIO_DATA( device ,  &(hit->_cellID0) , 1  ) ;
      if( lcFlag.bitSet( EVENT::LCIO::RTHPBIT_ID1 ) ){
        SIO_DATA( device ,  &(hit->_cellID1) , 1  ) ;
      }
    }
    SIO_DATA( device, &(hit->_type) , 1 ) ;
    SIO_DATA( device, hit->_pos     , 3 ) ;
    SIO_DATA( device, hit->_u       , 2 ) ;
    SIO_DATA( device, hit->_v       , 2 ) ;
    SIO_DATA( device, &(hit->_du)   , 1 ) ;
    SIO_DATA( device, &(hit->_dv)   , 1 ) ;
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
    SIO_PTAG( device , dynamic_cast<const EVENT::TrackerHitPlane*>(hit) ) ;
  }

  //----------------------------------------------------------------------------

  void SIOTrackerHitPlaneHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto hit = dynamic_cast<const EVENT::TrackerHitPlane*>(obj)  ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    SIO_SDATA( device, hit->getCellID0()  ) ;
    if( lcFlag.bitSet( EVENT::LCIO::RTHPBIT_ID1 ) ){
      SIO_SDATA( device, hit->getCellID1()  ) ;
    }
    SIO_SDATA( device ,  hit->getType() ) ;
    SIO_DATA( device,  hit->getPosition() , 3 ) ;
    SIO_DATA( device,  hit->getU() , 2 ) ;
    SIO_DATA( device,  hit->getV() , 2 ) ;
    SIO_SDATA( device, hit->getdU()  ) ;
    SIO_SDATA( device, hit->getdV()  ) ;
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

  EVENT::LCObject *SIOTrackerHitPlaneHandler::create() const {
    return new IOIMPL::TrackerHitPlaneIOImpl() ;
  }

} // namespace
