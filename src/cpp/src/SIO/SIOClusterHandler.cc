#include "SIO/SIOClusterHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "IOIMPL/ClusterIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"

using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOClusterHandler::read(SIO_stream* stream, 
				      LCObject** objP ){
    unsigned int status ; 
	

    // create a new object :
    ClusterIOImpl* cluster  = new ClusterIOImpl ;
    *objP = cluster ;
	
    //SIO_DATA( stream ,  &(cluster->_type) , 1  ) ;
    int type ;
    SIO_DATA( stream ,  &type  , 1 ) ;
    cluster->setType( type ) ;  // type is bitset<32> - can't be set directly

    SIO_DATA( stream ,  &(cluster->_energy)  , 1 ) ;
    SIO_DATA( stream ,  cluster->_position  , 3 ) ;

    float errpos[ NERRPOS ] ;
    SIO_DATA( stream , errpos   , NERRPOS ) ;
    cluster->setPositionError( errpos ) ;
    
    SIO_DATA( stream ,  &(cluster->_theta)  , 1 ) ;
    SIO_DATA( stream ,  &(cluster->_phi)  , 1 ) ;
    
    float errdir[ NERRDIR ] ;
    SIO_DATA( stream , errdir  , NERRDIR ) ;
    cluster->setDirectionError( errdir ) ;
  
    float shape[NSHAPE] ;
    SIO_DATA( stream , shape  , NSHAPE ) ;
    cluster->setShape( shape ) ;

    float pType[3] ;
    SIO_DATA( stream , pType  , 3 ) ;
    cluster->setEMWeight( pType[0] ) ;
    cluster->setHADWeight( pType[1] ) ;
    cluster->setMuonWeight( pType[2] ) ;
    
    int nClusters ; 
    SIO_DATA( stream, &nClusters , 1  ) ;

    // fill the vector to have correct size
    // as we are using the addresses of the elements henceforth

    cluster->_clusters.resize( nClusters ) ;
    for(int i=0;i<nClusters;i++){
      SIO_PNTR( stream , &(cluster->_clusters[i] ) ) ;
    }

    if( LCFlagImpl(_flag).bitSet( LCIO::CLBIT_HITS ) ){ 

      int nHits ;
      SIO_DATA( stream, &nHits , 1  ) ;

      cluster->_hits.resize( nHits ) ;
      cluster->_weights.resize( nHits ) ;

      for(int i=0;i<nHits;i++){
	SIO_PNTR( stream , &(cluster->_hits[i] ) ) ;
	SIO_PNTR( stream , &(cluster->_weights[i] ) ) ;
      }
      
    }
    int nEnergies ;
    SIO_DATA( stream, &nEnergies , 1  ) ;
    cluster->_subdetectorEnergies.resize( nEnergies ) ;
    for(int i=0;i<nEnergies;i++){
      SIO_DATA( stream, &(cluster->_subdetectorEnergies[i] )  , 1  ) ;
    }

    // read the pointer tag 
    SIO_PTAG( stream , dynamic_cast<const Cluster*>(cluster) ) ;
    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOClusterHandler::write(SIO_stream* stream, 
				       const LCObject* obj ){
    
    unsigned int status ; 
    
    // this is where we gave up type safety in order to
    // simplify the API and the implementation
    // by having a common collection of objects
    const Cluster* cluster = dynamic_cast<const Cluster*>(obj)  ;


    LCSIO_WRITE( stream, cluster->getType()  ) ;
    LCSIO_WRITE( stream, cluster->getEnergy()  ) ;
    float* pos = const_cast<float*> ( cluster->getPosition() ) ; 
    SIO_DATA( stream,  pos , 3 ) ;

    const FloatVec& errpos = cluster->getPositionError() ;
    for(unsigned int i=0;i<errpos.size();i++){
      LCSIO_WRITE( stream, errpos[i]  ) ;
    }
    LCSIO_WRITE( stream, cluster->getITheta()  ) ;
    LCSIO_WRITE( stream, cluster->getIPhi()  ) ;

    const FloatVec& errdir = cluster->getDirectionError() ;
    for(unsigned int i=0;i<errdir.size();i++){
      LCSIO_WRITE( stream, errdir[i]  ) ;
    }
    const FloatVec& shape = cluster->getShape() ;
    for(unsigned int i=0;i<shape.size();i++){
      LCSIO_WRITE( stream, shape[i]  ) ;
    }
    const FloatVec& particleType = cluster->getParticleType() ;
    for(unsigned int i=0;i<particleType.size();i++){
      LCSIO_WRITE( stream, particleType[i]  ) ;
    }

    const ClusterVec& clusters = cluster->getClusters() ;
    int nClusters =  clusters.size() ;

    SIO_DATA( stream, &nClusters , 1  ) ;
    
    for(int i=0;i<nClusters;i++){
      SIO_PNTR( stream , &(clusters[i]) ) ;
    }

    if( LCFlagImpl(_flag).bitSet( LCIO::CLBIT_HITS ) ){ 


      const CalorimeterHitVec& hits = cluster->getCalorimeterHits() ;
      const FloatVec& weights = cluster->getHitContributions() ;
      int nHits = hits.size() ;
      SIO_DATA( stream, &nHits , 1  ) ;
      
      for(int i=0;i<nHits;i++){
	SIO_PNTR( stream , &(hits[i]) ) ;
	SIO_PNTR( stream , &(weights[i]) ) ;
      }

    }

    const FloatVec& subdetectorEnergies = cluster->getSubdetectorEnergies() ;
    int nEnergies = subdetectorEnergies.size() ;

    LCSIO_WRITE( stream, nEnergies  ) ;
    for(int i=0;i<nEnergies;i++){
      LCSIO_WRITE( stream, subdetectorEnergies[i] ) ;
    }
    // write a ptag in order to be able to point to clusters
    SIO_PTAG( stream , cluster ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
}; // namespace
