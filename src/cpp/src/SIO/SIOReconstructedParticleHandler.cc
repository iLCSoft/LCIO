#include "SIO/SIOReconstructedParticleHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "IOIMPL/ReconstructedParticleIOImpl.h"
#include "IOIMPL/ParticleIDIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"

using namespace DATA ;
using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOReconstructedParticleHandler::read(SIO_stream* stream, 
				      LCObject** objP,
				      unsigned int flag,
				      unsigned int vers ){
    unsigned int status ; 
	

    // create a new object :
    ReconstructedParticleIOImpl* recP  = new ReconstructedParticleIOImpl ;
    *objP = recP ;
	
    SIO_DATA( stream ,  &(recP->_typeFlag) , 1  ) ;
    SIO_DATA( stream ,  recP->_momentum  , 3 ) ;
    SIO_DATA( stream ,  &(recP->_energy)  , 1 ) ;

    float errpos[ NCOVARIANCE ] ;
    SIO_DATA( stream , errpos   , NCOVARIANCE ) ;
    recP->setCovMatrix( errpos ) ;
    
    SIO_DATA( stream ,  &(recP->_mass)  , 1 ) ;
    SIO_DATA( stream ,  &(recP->_charge)  , 1 ) ;
    
    SIO_DATA( stream ,  recP->_reference  , 3 ) ;

    // read PIDs
    int nPid ;
    SIO_DATA( stream ,  &nPid  , 1 ) ;
    for(int i=0;i<nPid;i++){
      // create new Pid objects
      ParticleIDIOImpl* pid = new ParticleIDIOImpl ;
      SIO_DATA( stream ,  &(pid->_probability) , 1  ) ;
      SIO_DATA( stream ,  &(pid->_typeID) , 1  ) ;
      char* dummy ; 
      LCSIO_READ( stream,  &dummy ) ; 
      pid->setIdentifier( dummy ) ;
      int nPara  ;
      SIO_DATA( stream ,  &nPara  , 1 ) ;
      float aParameter ;
      for(int j=0;j<nPara;j++){
	SIO_DATA( stream ,  &aParameter  , 1 ) ;
	pid->addParameter( aParameter ) ;
      }
      recP->addParticleID( pid) ;
    }

    // read reconstructed particles
    int nRecP ;
    SIO_DATA( stream, &nRecP , 1  ) ;

    for(int i=0;i<nRecP;i++){
      recP->_particles.push_back( 0 ) ;
      recP->_particleWeights.push_back( 0 ) ;
    }
    for(int i=0;i<nRecP;i++){
      SIO_PNTR( stream , &(recP->_particles[i] ) ) ;
      SIO_PNTR( stream , &(recP->_particleWeights[i] ) ) ;
    }

    // read tracks
    int nTrk ;
    SIO_DATA( stream, &nTrk , 1  ) ;

    for(int i=0;i<nTrk;i++){
      recP->_tracks.push_back( 0 ) ;
      recP->_trackWeights.push_back( 0 ) ;
    }
    for(int i=0;i<nTrk;i++){
      SIO_PNTR( stream , &(recP->_tracks[i] ) ) ;
      SIO_PNTR( stream , &(recP->_trackWeights[i] ) ) ;
    }

    // read clusters
    int nClu ;
    SIO_DATA( stream, &nClu , 1  ) ;

    for(int i=0;i<nClu;i++){
      recP->_clusters.push_back( 0 ) ;
      recP->_clusterWeights.push_back( 0 ) ;
    }
    for(int i=0;i<nClu;i++){
      SIO_PNTR( stream , &(recP->_clusters[i] ) ) ;
      SIO_PNTR( stream , &(recP->_clusterWeights[i] ) ) ;
    }

    // read MCParticles
    int nMCP ;
    SIO_DATA( stream, &nMCP , 1  ) ;

    for(int i=0;i<nMCP;i++){
      recP->_mcParticles.push_back( 0 ) ;
      recP->_mcParticleWeights.push_back( 0 ) ;
    }
    for(int i=0;i<nMCP;i++){
      SIO_PNTR( stream , &(recP->_mcParticles[i] ) ) ;
      SIO_PNTR( stream , &(recP->_mcParticleWeights[i] ) ) ;
    }



    // read the pointer tag 
    SIO_PTAG( stream , dynamic_cast<const ReconstructedParticle*>(recP) ) ;
    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOReconstructedParticleHandler::write(SIO_stream* stream, 
				       const LCObject* obj,
				       unsigned int flag ){
    
    unsigned int status ; 
    
    // this is where we gave up type safety in order to
    // simplify the API and the implementation
    // by having a common collection of objects
    const ReconstructedParticleData* recP = dynamic_cast<const ReconstructedParticleData*>(obj)  ;


    LCSIO_WRITE( stream, recP->getTypeFlag()  ) ;
    float* mom = const_cast<float*> ( recP->getMomentum() ) ; 
    SIO_DATA( stream,  mom , 3 ) ;
    LCSIO_WRITE( stream, recP->getEnergy()  ) ;

    const FloatVec& cov = recP->getCovMatrix() ;
    for(unsigned int i=0;i<cov.size();i++){
      LCSIO_WRITE( stream, cov[i]  ) ;
    }
    LCSIO_WRITE( stream, recP->getMass()  ) ;
    LCSIO_WRITE( stream, recP->getCharge()  ) ;

    float* ref = const_cast<float*> ( recP->getReferencePoint() ) ; 
    SIO_DATA( stream,  ref , 3 ) ;


    // write Pids
    int nPid  = recP->getParticleIDsData().size() ;
    SIO_DATA( stream ,  &nPid  , 1 ) ;

    for(int i=0;i<nPid;i++){
      const ParticleIDData* pid = recP->getParticleIDsData()[i]  ;
      LCSIO_WRITE( stream, pid->getProbability()  ) ;
      LCSIO_WRITE( stream, pid->getTypeID()  ) ;
      LCSIO_WRITE( stream, pid->getIdentifier()  ) ;
      int nPara = pid->getParameters().size() ;
      SIO_DATA( stream ,  &nPara  , 1 ) ;
      for(int j=0;j<nPara;j++){
	LCSIO_WRITE( stream, pid->getParameters()[j]  ) ;
      }
    }

    // write reconstructed particles
    int nReconstructedParticles=  recP->getParticlesData().size() ;
    SIO_DATA( stream, &nReconstructedParticles , 1  ) ;
    
    for(int i=0;i<nReconstructedParticles;i++){
      SIO_PNTR( stream , &(recP->getParticlesData()[i]) ) ;
      LCSIO_WRITE( stream, recP->getParticleWeights()[i]  ) ;
    }
    
    // write tracks
    int nTrk = recP->getTracksData().size() ;
    SIO_DATA( stream, &nTrk , 1  ) ;

    for(int i=0;i<nTrk;i++){
      SIO_PNTR( stream , &(recP->getTracksData()[i]) ) ;
      LCSIO_WRITE( stream, recP->getTrackWeights()[i]  ) ;
    }

    // write clusters
    int nClu = recP->getClustersData().size() ;
    SIO_DATA( stream, &nClu , 1  ) ;

    for(int i=0;i<nClu;i++){
      SIO_PNTR( stream , &(recP->getClustersData()[i]) ) ;
      LCSIO_WRITE( stream, recP->getClusterWeights()[i]  ) ;
    }

    // write MCParticles
    int nMCP = recP->getMCParticlesData().size() ;
    SIO_DATA( stream, &nMCP , 1  ) ;

    for(int i=0;i<nMCP;i++){
      SIO_PNTR( stream , &(recP->getMCParticlesData()[i]) ) ;
      LCSIO_WRITE( stream, recP->getMCParticleWeights()[i]  ) ;
    }

    // write a ptag in order to be able to point to recPs
    SIO_PTAG( stream , recP ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
}; // namespace
