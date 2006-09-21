#include "SIO/SIOReconstructedParticleHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "IOIMPL/ReconstructedParticleIOImpl.h"
#include "IOIMPL/ParticleIDIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"
#include <bitset>


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
  
  unsigned int SIOReconstructedParticleHandler::read(SIO_stream* stream, 
						     LCObject** objP){
    unsigned int status ; 
    
    
    // create a new object :
    ReconstructedParticleIOImpl* recP  = new ReconstructedParticleIOImpl ;
    *objP = recP ;
    
    if( _vers > SIO_VERSION_ENCODE( 1, 2)   ) {
      
      
      SIO_DATA( stream ,  &(recP->_type)  , 1 ) ;
      
      float momentum[3] ;
      SIO_DATA( stream ,  momentum  , 3 ) ;
      //     SIO_DATA( stream ,  recP->_momentum  , 3 ) ;
      recP->setMomentum( momentum ) ;
      
      float energy ;
      SIO_DATA( stream , &energy , 1  ) ;
      recP->setEnergy( energy )  ;
//       SIO_DATA( stream ,  &(recP->_energy)  , 1 ) ;
      
      float errpos[ NCOVARIANCE ] ;
      SIO_DATA( stream , errpos   , NCOVARIANCE ) ;
      recP->setCovMatrix( errpos ) ;
      
      float mass ;
      SIO_DATA( stream , &mass , 1  ) ;
      recP->setMass( mass )  ;
//       SIO_DATA( stream ,  &(recP->_mass)  , 1 ) ;

      SIO_DATA( stream ,  &(recP->_charge)  , 1 ) ;
      
      SIO_DATA( stream ,  recP->_reference  , 3 ) ;
      
      // read PIDs
      int nPid ;
      
      SIO_DATA( stream ,  &nPid  , 1 ) ;
      for(int i=0;i<nPid;i++){
	// create new Pid objects
	ParticleIDIOImpl* pid = new ParticleIDIOImpl ;
	
	SIO_DATA( stream ,  &(pid->_likelihood) , 1  ) ;
	SIO_DATA( stream ,  &(pid->_type) , 1  ) ;
	SIO_DATA( stream ,  &(pid->_pdg) , 1  ) ;
	SIO_DATA( stream ,  &(pid->_algorithmType) , 1  ) ;

	int nPara  ;
	SIO_DATA( stream ,  &nPara  , 1 ) ;
	float aParameter ;
	for(int j=0;j<nPara;j++){
	  SIO_DATA( stream ,  &aParameter  , 1 ) ;
	  pid->addParameter( aParameter ) ;
	}
	SIO_PTAG( stream , pid  ) ;

	recP->addParticleID( pid) ;
      }
      
      SIO_PNTR( stream ,   &(recP->_pidUsed) ) ;
      SIO_DATA( stream ,  &(recP->_goodnessOfPID) , 1  ) ;

      // read reconstructed particles
      int nRecP ;
      SIO_DATA( stream, &nRecP , 1  ) ;

      recP->_particles.resize( nRecP ) ;
      for(int i=0;i<nRecP;i++){
	SIO_PNTR( stream , &(recP->_particles[i] ) ) ;
      }
      
      // read tracks
      int nTrk ;
      SIO_DATA( stream, &nTrk , 1  ) ;

      recP->_tracks.resize( nTrk ) ;
      for(int i=0;i<nTrk;i++){
	SIO_PNTR( stream , &(recP->_tracks[i] ) ) ;
      }
      
      // read clusters
      int nClu ;
      SIO_DATA( stream, &nClu , 1  ) ;

      recP->_clusters.resize( nClu ) ;
      for(int i=0;i<nClu;i++){
	SIO_PNTR( stream , &(recP->_clusters[i] ) ) ;
      }
      
      if( _vers > SIO_VERSION_ENCODE( 1,7 )   ) {
	
	//read pointers to start/end vertices
	SIO_PNTR( stream ,   &(recP->_sv) ) ;
	//SIO_PNTR( stream ,   &(recP->_ev) ) ;
      }
      
      
    } else {   // old version (1.0-1.2 keep for a while - no official release !! )

      int typeFlag ;
      SIO_DATA( stream ,  &typeFlag , 1  ) ;
      recP->setType( 0x0000ffff & typeFlag ) ;
//       recP->setPrimary( (1<<31) &  typeFlag ) ;
      
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
	
	SIO_DATA( stream ,  &(pid->_likelihood) , 1  ) ;
	SIO_DATA( stream ,  &(pid->_type) , 1  ) ;
	SIO_DATA( stream ,  &(pid->_algorithmType) , 1  ) ;
	
	int nPara  ;
	SIO_DATA( stream ,  &nPara  , 1 ) ;
	float aParameter ;
	for(int j=0;j<nPara;j++){
	  SIO_DATA( stream ,  &aParameter  , 1 ) ;
	  pid->addParameter( aParameter ) ;
	}
	recP->addParticleID( pid) ;
      }
      
      recP->_pidUsed = 0 ;

      // read reconstructed particles
      int nRecP ;
      SIO_DATA( stream, &nRecP , 1  ) ;
      
      for(int i=0;i<nRecP;i++){
	recP->_particles.push_back( 0 ) ;
      }
      float dummyWeights ;
      for(int i=0;i<nRecP;i++){
	SIO_PNTR( stream , &(recP->_particles[i] ) ) ;
	SIO_DATA( stream ,  &dummyWeights  , 1 ) ;
      }
      
      // read tracks
      int nTrk ;
      SIO_DATA( stream, &nTrk , 1  ) ;
      
      for(int i=0;i<nTrk;i++){
	recP->_tracks.push_back( 0 ) ;
      }
      for(int i=0;i<nTrk;i++){
	SIO_PNTR( stream , &(recP->_tracks[i] ) ) ;
	SIO_DATA( stream ,  &dummyWeights  , 1 ) ;
      }
      
      // read clusters
      int nClu ;
      SIO_DATA( stream, &nClu , 1  ) ;
      
      for(int i=0;i<nClu;i++){
	recP->_clusters.push_back( 0 ) ;
      }
      for(int i=0;i<nClu;i++){
	SIO_PNTR( stream , &(recP->_clusters[i] ) ) ;
	SIO_DATA( stream ,  &dummyWeights  , 1 ) ;
      }
      
      // read MCParticles
      int nMCP ;
      SIO_DATA( stream, &nMCP , 1  ) ;

      SIO_DATA( stream ,  &dummyWeights  , nMCP ) ;
      SIO_DATA( stream ,  &dummyWeights  , nMCP ) ;
      
//       for(int i=0;i<nMCP;i++){
// 	recP->_mcParticles.push_back( 0 ) ;
// 	recP->_mcParticleWeights.push_back( 0 ) ;
//       }
//       for(int i=0;i<nMCP;i++){
// 	SIO_PNTR( stream , &(recP->_mcParticles[i] ) ) ;
// 	SIO_PNTR( stream , &(recP->_mcParticleWeights[i] ) ) ;
//       }
      
    } // --- end old version 1.0-1.2
      
    // read the pointer tag 
    SIO_PTAG( stream , dynamic_cast<const ReconstructedParticle*>(recP) ) ;
    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOReconstructedParticleHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 
    
    // this is where we gave up type safety in order to
    // simplify the API and the implementation
    // by having a common collection of objects
    const ReconstructedParticle* recP = dynamic_cast<const ReconstructedParticle*>(obj)  ;


//     // bit 31 is primary flag 
//     // lower 16 bits are type 
//     std::bitset<32> typeFlag =   recP->getType() ; 
//     if( recP->isPrimary() ) typeFlag[31] = 1 ;
//     LCSIO_WRITE( stream, (int) typeFlag.to_ulong()  ) ;


    LCSIO_WRITE( stream, recP->getType()  ) ;

//     float* mom = const_cast<float*> ( recP->getMomentum() ) ; 
//     SIO_DATA( stream,  mom , 3 ) ;
    LCSIO_WRITE( stream, (float) recP->getMomentum()[0] ) ;
    LCSIO_WRITE( stream, (float) recP->getMomentum()[1] ) ;
    LCSIO_WRITE( stream, (float) recP->getMomentum()[2] ) ;
    

    LCSIO_WRITE( stream, (float) recP->getEnergy()  ) ;

    const FloatVec& cov = recP->getCovMatrix() ;
    for(unsigned int i=0;i<cov.size();i++){
      LCSIO_WRITE( stream, cov[i]  ) ;
    }
    LCSIO_WRITE( stream, (float) recP->getMass()  ) ;
    LCSIO_WRITE( stream, recP->getCharge()  ) ;

    float* ref = const_cast<float*> ( recP->getReferencePoint() ) ; 
    SIO_DATA( stream,  ref , 3 ) ;


    // write Pids
    int nPid  = recP->getParticleIDs().size() ;
    SIO_DATA( stream ,  &nPid  , 1 ) ;

    for(int i=0;i<nPid;i++){
      const ParticleID* pid = recP->getParticleIDs()[i]  ;
      LCSIO_WRITE( stream, pid->getLikelihood()  ) ;
      LCSIO_WRITE( stream, pid->getType()  ) ;
      LCSIO_WRITE( stream, pid->getPDG()  ) ;
      LCSIO_WRITE( stream, pid->getAlgorithmType()  ) ;

      int nPara = pid->getParameters().size() ;
      SIO_DATA( stream ,  &nPara  , 1 ) ;
      for(int j=0;j<nPara;j++){
	LCSIO_WRITE( stream, pid->getParameters()[j]  ) ;
      }
      SIO_PTAG( stream ,  pid  ) ;
    }

    EVENT::ParticleID* pidUsed = recP->getParticleIDUsed() ;
    SIO_PNTR( stream , &pidUsed  ) ;
    LCSIO_WRITE( stream, recP->getGoodnessOfPID()  ) ;
    
    // write reconstructed particles
    int nReconstructedParticles=  recP->getParticles().size() ;
    SIO_DATA( stream, &nReconstructedParticles , 1  ) ;
    
    for(int i=0;i<nReconstructedParticles;i++){
      SIO_PNTR( stream , &(recP->getParticles()[i]) ) ;
//       LCSIO_WRITE( stream, recP->getParticleWeights()[i]  ) ;
    }
    
    // write tracks
    int nTrk = recP->getTracks().size() ;
    SIO_DATA( stream, &nTrk , 1  ) ;

    for(int i=0;i<nTrk;i++){
      SIO_PNTR( stream , &(recP->getTracks()[i]) ) ;
//       LCSIO_WRITE( stream, recP->getTrackWeights()[i]  ) ;
    }

    // write clusters
    int nClu = recP->getClusters().size() ;
    SIO_DATA( stream, &nClu , 1  ) ;

    for(int i=0;i<nClu;i++){
      SIO_PNTR( stream , &(recP->getClusters()[i]) ) ;
//       LCSIO_WRITE( stream, recP->getClusterWeights()[i]  ) ;
    }

//     // write MCParticles
//     int nMCP = recP->getMCParticles().size() ;
//     SIO_DATA( stream, &nMCP , 1  ) ;

//     for(int i=0;i<nMCP;i++){
//       SIO_PNTR( stream , &(recP->getMCParticles()[i]) ) ;
//       LCSIO_WRITE( stream, recP->getMCParticleWeights()[i]  ) ;
//     }

    // write pointers to start/end vertices
    EVENT::Vertex* sv = recP->getStartVertex() ;
    SIO_PNTR( stream , &sv  ) ;
    //EVENT::Vertex* ev = recP->getEndVertex() ;
    //SIO_PNTR( stream , &ev  ) ;


    // write a ptag in order to be able to point to recPs
    SIO_PTAG( stream , recP ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
} // namespace
