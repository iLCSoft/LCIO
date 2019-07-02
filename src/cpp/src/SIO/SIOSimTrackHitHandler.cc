#include "SIO/SIOSimTrackHitHandler.h"


#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/SimTrackerHit.h"
#include "IOIMPL/SimTrackerHitIOImpl.h"
#include "IMPL/LCFlagImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOSimTrackHitHandler::SIOSimTrackHitHandler() :
    SIOObjectHandler( EVENT::LCIO::SIMTRACKERHIT ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOSimTrackHitHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    auto hit = dynamic_cast<IOIMPL::SimTrackerHitIOImpl*>( objP ) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;

    SIO_DATA( device ,  &(hit->_cellID0) , 1  ) ;
    if( vers > SIO_VERSION_ENCODE( 1, 51) ) {
      if( lcFlag.bitSet( EVENT::LCIO::THBIT_ID1 ) ) {
        SIO_DATA( device ,  &(hit->_cellID1) , 1  ) ;
      }
    }
    SIO_DATA( device ,    hit->_pos  , 3 ) ;
    SIO_DATA( device ,  &(hit->_EDep) , 1  ) ;
    SIO_DATA( device ,  &(hit->_time) , 1  ) ;
    SIO_PNTR( device , &(hit->_particle)  ) ;
    if( lcFlag.bitSet( EVENT::LCIO::THBIT_MOMENTUM ) ){
      SIO_DATA( device ,    hit->_p  , 3 ) ;
      if( vers  > SIO_VERSION_ENCODE( 1 , 6 ) ) {
        SIO_DATA( device ,    &(hit->_pathLength)  , 1 ) ;
      }
    }
    if( vers > SIO_VERSION_ENCODE( 2, 7 )   ) {
      SIO_DATA( device ,  &(hit->_quality) , 1  ) ;
    }
    // read the pointer tag in case we want to point to hits
    if( vers > SIO_VERSION_ENCODE( 1, 0) ) {
      SIO_PTAG( device , dynamic_cast<const EVENT::SimTrackerHit*>(hit) ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOSimTrackHitHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto hit = dynamic_cast<const EVENT::SimTrackerHit*>(obj) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    SIO_SDATA( device, hit->getCellID0()  ) ;
    if( lcFlag.bitSet( EVENT::LCIO::THBIT_ID1 ) ) {
      SIO_SDATA( device, hit->getCellID1()  ) ;
    }
    SIO_DATA( device,  hit->getPosition() , 3 ) ;
    SIO_SDATA( device, hit->getEDep()  ) ;
    SIO_SDATA( device, hit->getTime()  ) ;
    auto part = hit->getMCParticle()  ;
    SIO_PNTR( device , &part ) ;
    if( lcFlag.bitSet( EVENT::LCIO::THBIT_MOMENTUM ) ){
      SIO_DATA( device , hit->getMomentum()  , 3 ) ;
      SIO_SDATA( device , hit->getPathLength() ) ;
    }
    SIO_SDATA( device, hit->getQuality()  ) ;
    // write a ptag in order to be able to point to tracker hits - from v1.1
    SIO_PTAG( device , hit ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOSimTrackHitHandler::create() const {
    return new IOIMPL::SimTrackerHitIOImpl() ;
  }

} // namespace
