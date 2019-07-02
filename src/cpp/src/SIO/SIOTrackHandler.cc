#include "SIO/SIOTrackHandler.h"

// -- lcio headers
#include "EVENT/LCIO.h"
#include "IOIMPL/TrackIOImpl.h"
#include "IOIMPL/TrackStateIOImpl.h"
#include "IMPL/LCFlagImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOTrackHandler::SIOTrackHandler() :
    SIOObjectHandler( EVENT::LCIO::TRACK ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOTrackHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    auto trk = dynamic_cast<IOIMPL::TrackIOImpl*>( objP ) ;
    int type ;
    SIO_DATA( device ,  &type  , 1 ) ;
    trk->setType( type ) ;  // type is bitset<32> - can't be set directly
    // read TrackStates
    int nTrackStates = 1 ; // set to 1 per default for backwards compatibility
    if( vers >= SIO_VERSION_ENCODE( 2, 0 )   ) {
      SIO_DATA( device ,  &nTrackStates  , 1 ) ;
    }
    for( int i=0 ; i<nTrackStates ; i++ ) {
      // create new TrackState object
      auto trackstate = new IOIMPL::TrackStateIOImpl() ;
      if( vers >= SIO_VERSION_ENCODE( 2, 0 ) ) {
        SIO_DATA( device ,  &(trackstate->_location)  , 1 ) ;
      }
      SIO_DATA( device ,  &(trackstate->_d0)  , 1 ) ;
      SIO_DATA( device ,  &(trackstate->_phi)  , 1 ) ;
      SIO_DATA( device ,  &(trackstate->_omega)  , 1 ) ;
      SIO_DATA( device ,  &(trackstate->_z0)  , 1 ) ;
      SIO_DATA( device ,  &(trackstate->_tanLambda)  , 1 ) ;
      float cov[15] ;
      SIO_DATA( device ,  cov  ,  15 ) ;
      trackstate->setCovMatrix( cov ) ;
      SIO_DATA( device ,  trackstate->_reference  , 3 ) ;
      trk->addTrackState( trackstate );
    }
    SIO_DATA( device ,  &(trk->_chi2)  , 1 ) ;
    SIO_DATA( device ,  &(trk->_ndf)  , 1 ) ;
    SIO_DATA( device ,  &(trk->_dEdx) , 1  ) ;
    SIO_DATA( device ,  &(trk->_dEdxError) , 1  ) ;
    SIO_DATA( device ,  &( trk->_radiusOfInnermostHit ) , 1  ) ;
    int nHitNumbers ;
    SIO_DATA( device, &nHitNumbers , 1  ) ;
    trk->subdetectorHitNumbers().resize( nHitNumbers ) ;
    for( int i=0 ; i<nHitNumbers ; i++ ) {
      SIO_DATA( device , &(trk->_subdetectorHitNumbers[i] ), 1 ) ;
    }
    int nTracks ;
    SIO_DATA( device, &nTracks , 1  ) ;
    trk->_tracks.resize( nTracks ) ;
    for( int i=0 ; i<nTracks ; i++ ) {
      SIO_PNTR( device , &(trk->_tracks[i] ) ) ;
    }
    if( IMPL::LCFlagImpl(_flag).bitSet( EVENT::LCIO::TRBIT_HITS ) ) {
      int nHits ;
      SIO_DATA( device, &nHits , 1  ) ;
      trk->_hits.resize( nHits) ;
      for( int i=0 ; i<nHits ; i++ ) {
	       SIO_PNTR( device , &(trk->_hits[i] ) ) ;
      }
    }
    // read the pointer tag
    SIO_PTAG( device , dynamic_cast<const EVENT::Track*>(trk) ) ;
  }

  //----------------------------------------------------------------------------

  void SIOTrackHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto trk = dynamic_cast<const EVENT::Track*>(obj)  ;
    SIO_SDATA( device, trk->getType()  ) ;
    auto trackstates = trk->getTrackStates() ;
    int nTrackStates = trackstates.size() ;
    SIO_DATA( device, &nTrackStates , 1  ) ;
    for( unsigned int i=0; i<trackstates.size() ; i++ ) {
      SIO_SDATA( device, trk->getTrackStates()[i]->getLocation()  ) ;
      SIO_SDATA( device, trk->getTrackStates()[i]->getD0()  ) ;
      SIO_SDATA( device, trk->getTrackStates()[i]->getPhi()  ) ;
      SIO_SDATA( device, trk->getTrackStates()[i]->getOmega()  ) ;
      SIO_SDATA( device, trk->getTrackStates()[i]->getZ0()  ) ;
      SIO_SDATA( device, trk->getTrackStates()[i]->getTanLambda()  ) ;
      auto cov = trk->getTrackStates()[i]->getCovMatrix() ;
      for(unsigned int j=0; j<cov.size() ; j++ ) {
        SIO_SDATA( device, cov[j]  ) ;
      }
      SIO_DATA( device,  trk->getTrackStates()[i]->getReferencePoint() , 3 ) ;
    }
    SIO_SDATA( device, trk->getChi2()  ) ;
    SIO_SDATA( device, trk->getNdf()  ) ;
    SIO_SDATA( device, trk->getdEdx()  ) ;
    SIO_SDATA( device, trk->getdEdxError()  ) ;
    SIO_SDATA( device , trk->getRadiusOfInnermostHit()  ) ;
    auto hitNums = trk->getSubdetectorHitNumbers() ;
    int nHitNumbers = hitNums.size() ;
    SIO_DATA( device, &nHitNumbers , 1  ) ;
    for( int i=0 ; i<nHitNumbers ; i++ ) {
      SIO_SDATA( device , hitNums[i]  ) ;
    }
    auto tracks = trk->getTracks() ;
    int nTracks = tracks.size() ;
    SIO_DATA( device, &nTracks , 1  ) ;
    for( int i=0 ; i<nTracks ; i++ ) {
      SIO_PNTR( device , &(tracks[i]) ) ;
    }
    if( IMPL::LCFlagImpl(_flag).bitSet( EVENT::LCIO::TRBIT_HITS ) ) {
      auto hits = trk->getTrackerHits() ;
      int nHits = hits.size() ;
      SIO_DATA( device, &nHits , 1  ) ;
      for( int i=0 ; i<nHits ; i++ ) {
	      SIO_PNTR( device , &(hits[i]) ) ;
      }
    }
    // write a ptag in order to be able to point to tracks
    SIO_PTAG( device , trk ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOTrackHandler::create() const {
    return new IOIMPL::TrackIOImpl() ;
  }

} // namespace
