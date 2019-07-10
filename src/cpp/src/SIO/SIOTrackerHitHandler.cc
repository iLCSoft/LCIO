#include "SIO/SIOTrackerHitHandler.h"

// -- lcio headers
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "EVENT/MCParticle.h"
#include "EVENT/TrackerHit.h"
#include "IOIMPL/TrackerHitIOImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOTrackerHitHandler::SIOTrackerHitHandler() :
    SIOObjectHandler( EVENT::LCIO::TRACKERHIT ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOTrackerHitHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    auto hit  = dynamic_cast<IOIMPL::TrackerHitIOImpl*>(objP) ;
    IMPL::LCFlagImpl lcFlag(_flag) ;

    if( vers > SIO_VERSION_ENCODE( 1, 51) ) {
      SIO_DATA( device ,  &(hit->_cellID0) , 1  ) ;
      if( lcFlag.bitSet( EVENT::LCIO::RTHBIT_ID1 ) ){
        SIO_DATA( device ,  &(hit->_cellID1) , 1  ) ;
      }
    }
    if( vers > SIO_VERSION_ENCODE( 1, 2)   ){
      SIO_DATA( device ,  &(hit->_type) , 1  ) ;
    }
    SIO_DATA( device ,    hit->_pos  , 3 ) ;
    float cov[TRKHITNCOVMATRIX] ;
    SIO_DATA( device ,  &cov[0]  ,  TRKHITNCOVMATRIX ) ;
    hit->setCovMatrix( cov ) ;
    SIO_DATA( device ,  &(hit->_EDep) , 1  ) ;
    if( vers > SIO_VERSION_ENCODE( 1, 12 )   ) {
      SIO_DATA( device ,  &(hit->_EDepError) , 1  ) ;
    }
    SIO_DATA( device ,  &(hit->_time) , 1  ) ;
    if( vers > SIO_VERSION_ENCODE( 1, 11 )   ) {
      SIO_DATA( device ,  &(hit->_quality) , 1  ) ;
    }
    int numberOfRawHits = 1 ;
    if( vers > SIO_VERSION_ENCODE( 1, 2 ) ) {
      SIO_DATA( device ,  &numberOfRawHits , 1  ) ;
    }
    hit->_rawHits.resize( numberOfRawHits ) ;

    for( int i=0 ; i<numberOfRawHits ; i++ ) {
      SIO_PNTR( device , &(hit->_rawHits[i] ) ) ;
    }
    SIO_PTAG( device , dynamic_cast<const EVENT::TrackerHit*>(hit) ) ;
  }

  //----------------------------------------------------------------------------

  void SIOTrackerHitHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto hit = dynamic_cast<const EVENT::TrackerHit*>(obj)  ;
    IMPL::LCFlagImpl lcFlag(_flag) ;
    SIO_SDATA( device, hit->getCellID0()  ) ;
    if( lcFlag.bitSet( EVENT::LCIO::RTHBIT_ID1 ) ){
      SIO_SDATA( device, hit->getCellID1()  ) ;
    }
    SIO_SDATA( device ,  hit->getType() ) ;
    SIO_DATA( device,  hit->getPosition() , 3 ) ;
    auto cov = hit->getCovMatrix() ;
    for( unsigned int i=0 ; i<cov.size() ; i++ ) {
      SIO_SDATA( device, cov[i]  ) ;
    }
    SIO_SDATA( device, hit->getEDep()  ) ;
    SIO_SDATA( device, hit->getEDepError()  ) ;
    SIO_SDATA( device, hit->getTime()  ) ;
    SIO_SDATA( device, hit->getQuality()  ) ;
    const EVENT::LCObjectVec& rawHits = hit->getRawHits() ;
    int nrawhits = rawHits.size() ;
    SIO_SDATA( device, nrawhits  ) ;
    for(unsigned int i=0; i < rawHits.size() ; i++){
        SIO_PNTR( device , &(rawHits[i]) ) ;
    }
    // write a ptag in order to be able to point to tracker hits in the future
    SIO_PTAG( device , hit ) ;
  }


  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOTrackerHitHandler::create() const {
    return new IOIMPL::TrackerHitIOImpl() ;
  }

} // namespace
