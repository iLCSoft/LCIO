#include "SIO/SIOReconstructedParticleHandler.h"

// -- lcio headers
#include "EVENT/LCIO.h"
#include "IOIMPL/ReconstructedParticleIOImpl.h"
#include "IOIMPL/ParticleIDIOImpl.h"
#include "IMPL/LCFlagImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

// -- std headers
#include <bitset>

namespace SIO {

  SIOReconstructedParticleHandler::SIOReconstructedParticleHandler() :
    SIOObjectHandler( EVENT::LCIO::RECONSTRUCTEDPARTICLE ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOReconstructedParticleHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    auto recP = dynamic_cast<IOIMPL::ReconstructedParticleIOImpl*>( objP ) ;
    if( vers > SIO_VERSION_ENCODE( 1, 2 ) ) {
      // type
      SIO_DATA( device ,  &(recP->_type)  , 1 ) ;
      // momentum
      float momentum[3] ;
      SIO_DATA( device ,  momentum  , 3 ) ;
      recP->setMomentum( momentum ) ;
      // energy
      float energy ;
      SIO_DATA( device , &energy , 1  ) ;
      recP->setEnergy( energy )  ;
      // covariance matrix
      float errpos[ NCOVARIANCE ] ;
      SIO_DATA( device , errpos   , NCOVARIANCE ) ;
      recP->setCovMatrix( errpos ) ;
      // mass
      float mass ;
      SIO_DATA( device , &mass , 1  ) ;
      recP->setMass( mass )  ;
      // charge
      SIO_DATA( device ,  &(recP->_charge)  , 1 ) ;
      // reference point
      SIO_DATA( device ,  recP->_reference  , 3 ) ;
      // read PIDs
      int nPid ;
      SIO_DATA( device ,  &nPid  , 1 ) ;
      for( int i=0 ; i<nPid ; i++ ) {
        // create new Pid objects
        auto pid = new IOIMPL::ParticleIDIOImpl() ;
        SIO_DATA( device ,  &(pid->_likelihood) , 1  ) ;
        SIO_DATA( device ,  &(pid->_type) , 1  ) ;
        SIO_DATA( device ,  &(pid->_pdg) , 1  ) ;
        SIO_DATA( device ,  &(pid->_algorithmType) , 1  ) ;
        int nPara  ;
        SIO_DATA( device ,  &nPara  , 1 ) ;
        float aParameter ;
        for(int j=0;j<nPara;j++) {
          SIO_DATA( device ,  &aParameter  , 1 ) ;
          pid->addParameter( aParameter ) ;
        }
        SIO_PTAG( device , pid  ) ;
        recP->addParticleID( pid) ;
      }
      SIO_PNTR( device ,   &(recP->_pidUsed) ) ;
      SIO_DATA( device ,  &(recP->_goodnessOfPID) , 1  ) ;
      // read reconstructed particles
      int nRecP ;
      SIO_DATA( device, &nRecP , 1  ) ;
      recP->_particles.resize( nRecP ) ;
      for( int i=0 ; i<nRecP ; i++ ) {
        SIO_PNTR( device , &(recP->_particles[i] ) ) ;
      }
      // read tracks
      int nTrk ;
      SIO_DATA( device, &nTrk , 1  ) ;
      recP->_tracks.resize( nTrk ) ;
      for( int i=0 ; i<nTrk ; i++ ) {
        SIO_PNTR( device , &(recP->_tracks[i] ) ) ;
      }
      // read clusters
      int nClu ;
      SIO_DATA( device, &nClu , 1  ) ;
      recP->_clusters.resize( nClu ) ;
      for( int i=0 ; i<nClu ; i++ ) {
        SIO_PNTR( device , &(recP->_clusters[i] ) ) ;
      }
      if( vers > SIO_VERSION_ENCODE( 1,7 ) ) {
        // read pointers to start/end vertices
        SIO_PNTR( device ,   &(recP->_sv) ) ;
      }
    }
    else {   // old version (1.0-1.2 keep for a while - no official release !! )
      int typeFlag ;
      SIO_DATA( device ,  &typeFlag , 1  ) ;
      recP->setType( 0x0000ffff & typeFlag ) ;
      SIO_DATA( device ,  recP->_momentum  , 3 ) ;
      SIO_DATA( device ,  &(recP->_energy)  , 1 ) ;
      float errpos[ NCOVARIANCE ] ;
      SIO_DATA( device , errpos   , NCOVARIANCE ) ;
      recP->setCovMatrix( errpos ) ;
      SIO_DATA( device ,  &(recP->_mass)  , 1 ) ;
      SIO_DATA( device ,  &(recP->_charge)  , 1 ) ;
      SIO_DATA( device ,  recP->_reference  , 3 ) ;
      // read PIDs
      int nPid ;
      SIO_DATA( device ,  &nPid  , 1 ) ;
      for( int i=0 ; i<nPid ; i++ ) {
        // create new Pid objects
        auto pid = new IOIMPL::ParticleIDIOImpl() ;
        SIO_DATA( device ,  &(pid->_likelihood) , 1  ) ;
        SIO_DATA( device ,  &(pid->_type) , 1  ) ;
        SIO_DATA( device ,  &(pid->_algorithmType) , 1  ) ;
        int nPara  ;
        SIO_DATA( device ,  &nPara  , 1 ) ;
        float aParameter ;
        for(int j=0;j<nPara;j++){
          SIO_DATA( device ,  &aParameter  , 1 ) ;
          pid->addParameter( aParameter ) ;
        }
        recP->addParticleID( pid) ;
      }
      recP->_pidUsed = 0 ;
      // read reconstructed particles
      int nRecP ;
      SIO_DATA( device, &nRecP , 1  ) ;
      for( int i=0 ; i<nRecP ; i++ ) {
        recP->_particles.push_back( 0 ) ;
      }
      float dummyWeights ;
      for( int i=0 ; i<nRecP ; i++ ) {
        SIO_PNTR( device , &(recP->_particles[i] ) ) ;
        SIO_DATA( device ,  &dummyWeights  , 1 ) ;
      }
      // read tracks
      int nTrk ;
      SIO_DATA( device, &nTrk , 1  ) ;
      for( int i=0 ; i<nTrk ; i++ ) {
        recP->_tracks.push_back( 0 ) ;
      }
      for( int i=0 ; i<nTrk ; i++ ) {
        SIO_PNTR( device , &(recP->_tracks[i] ) ) ;
        SIO_DATA( device ,  &dummyWeights  , 1 ) ;
      }
      // read clusters
      int nClu ;
      SIO_DATA( device, &nClu , 1  ) ;
      for( int i=0 ; i<nClu ; i++ ) {
        recP->_clusters.push_back( 0 ) ;
      }
      for( int i=0 ; i<nClu ; i++ ) {
        SIO_PNTR( device , &(recP->_clusters[i] ) ) ;
        SIO_DATA( device ,  &dummyWeights  , 1 ) ;
      }
      // read MCParticles
      int nMCP ;
      SIO_DATA( device, &nMCP , 1  ) ;
      SIO_DATA( device ,  &dummyWeights  , nMCP ) ;
      SIO_DATA( device ,  &dummyWeights  , nMCP ) ;
    } // --- end old version 1.0-1.2
    // read the pointer tag
    SIO_PTAG( device , dynamic_cast<const EVENT::ReconstructedParticle*>(recP) ) ;
  }

  //----------------------------------------------------------------------------

  void SIOReconstructedParticleHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto recP = dynamic_cast<const EVENT::ReconstructedParticle*>( obj ) ;
    SIO_SDATA( device, recP->getType()  ) ;
    SIO_SDATA( device, (float) recP->getMomentum()[0] ) ;
    SIO_SDATA( device, (float) recP->getMomentum()[1] ) ;
    SIO_SDATA( device, (float) recP->getMomentum()[2] ) ;
    SIO_SDATA( device, (float) recP->getEnergy()  ) ;
    auto cov = recP->getCovMatrix() ;
    for( unsigned int i=0 ; i<cov.size() ; i++ ) {
      SIO_SDATA( device, cov[i]  ) ;
    }
    SIO_SDATA( device, (float) recP->getMass()  ) ;
    SIO_SDATA( device, recP->getCharge()  ) ;
    SIO_DATA( device,  recP->getReferencePoint() , 3 ) ;
    // write Pids
    int nPid  = recP->getParticleIDs().size() ;
    SIO_DATA( device ,  &nPid  , 1 ) ;

    for( int i=0 ; i<nPid ; i++ ) {
      auto pid = recP->getParticleIDs()[i]  ;
      SIO_SDATA( device, pid->getLikelihood()  ) ;
      SIO_SDATA( device, pid->getType()  ) ;
      SIO_SDATA( device, pid->getPDG()  ) ;
      SIO_SDATA( device, pid->getAlgorithmType()  ) ;
      int nPara = pid->getParameters().size() ;
      SIO_DATA( device ,  &nPara  , 1 ) ;
      for(int j=0;j<nPara;j++){
	      SIO_SDATA( device, pid->getParameters()[j]  ) ;
      }
      SIO_PTAG( device ,  pid  ) ;
    }
    auto pidUsed = recP->getParticleIDUsed() ;
    SIO_PNTR( device , &pidUsed  ) ;
    SIO_SDATA( device, recP->getGoodnessOfPID()  ) ;
    // write reconstructed particles
    int nReconstructedParticles =  recP->getParticles().size() ;
    SIO_DATA( device, &nReconstructedParticles , 1  ) ;
    for( int i=0 ; i<nReconstructedParticles ; i++ ) {
      SIO_PNTR( device , &(recP->getParticles()[i]) ) ;
    }
    // write tracks
    int nTrk = recP->getTracks().size() ;
    SIO_DATA( device, &nTrk , 1  ) ;
    for( int i=0 ; i<nTrk ; i++ ) {
      SIO_PNTR( device , &(recP->getTracks()[i]) ) ;
    }
    // write clusters
    int nClu = recP->getClusters().size() ;
    SIO_DATA( device, &nClu , 1  ) ;
    for( int i=0 ; i<nClu ; i++ ) {
      SIO_PNTR( device , &(recP->getClusters()[i]) ) ;
    }
    // write pointers to start/end vertices
    auto sv = recP->getStartVertex() ;
    SIO_PNTR( device , &sv  ) ;
    // write a ptag in order to be able to point to recPs
    SIO_PTAG( device , recP ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOReconstructedParticleHandler::create() const {
    return new IOIMPL::ReconstructedParticleIOImpl() ;
  }

} // namespace
