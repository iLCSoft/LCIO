#include "SIO/SIOClusterHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "IOIMPL/ClusterIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"

using namespace DATA ;
using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOClusterHandler::read(SIO_stream* stream, 
				      LCObject** objP,
				      unsigned int flag,
				      unsigned int vers ){
    unsigned int status ; 
	

    // create a new object :
    ClusterIOImpl* cluster  = new ClusterIOImpl ;
    *objP = cluster ;
	
    SIO_DATA( stream ,  &(cluster->_type) , 1  ) ;
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
    for(int i=0;i<nClusters;i++){
      cluster->_clusters.push_back( 0 ) ;
    }
    for(int i=0;i<nClusters;i++){
      SIO_PNTR( stream , &(cluster->_clusters[i] ) ) ;
    }

    if( LCFlagImpl(flag).bitSet( LCIO::CLBIT_HITS ) ){ 

      // hit collections
      int nHitCol ;
      SIO_DATA( stream, &nHitCol , 1  ) ;
      
      for(int i=0 ; i< nHitCol ;i++){
	
	char* dummy ; 
	LCSIO_READ( stream,  &dummy ) ; 
	WeightedIndices* wi = new WeightedIndices ;
	cluster->_indexMap[ dummy ] = wi ;
	wi->Indices = new IntVec ;
	wi->Weights = new FloatVec ;
	
	int nHits ;
	SIO_DATA( stream, &nHits , 1  ) ;
	
	int* hitsArray = new int[ nHits ] ;
	SIO_DATA( stream, hitsArray ,  nHits ) ;

	float* wgtsArray = new float[ nHits ] ;
	SIO_DATA( stream, wgtsArray ,  nHits ) ;
	
	for( int j=0 ; j< nHits ; j++ ){
	  wi->Indices->push_back(  hitsArray[ j ] ) ;
	  wi->Weights->push_back(  wgtsArray[ j ] ) ;
	}
	
	delete[] hitsArray ;
	delete[] wgtsArray ;
      }
    }

    // read the pointer tag 
    SIO_PTAG( stream , dynamic_cast<const Cluster*>(cluster) ) ;
    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOClusterHandler::write(SIO_stream* stream, 
				       const LCObject* obj,
				       unsigned int flag ){
    
    unsigned int status ; 
    
    // this is where we gave up type safety in order to
    // simplify the API and the implementation
    // by having a common collection of objects
    const ClusterData* cluster = dynamic_cast<const ClusterData*>(obj)  ;


    LCSIO_WRITE( stream, cluster->getType()  ) ;
    LCSIO_WRITE( stream, cluster->getEnergy()  ) ;
    float* pos = const_cast<float*> ( cluster->getPosition() ) ; 
    SIO_DATA( stream,  pos , 3 ) ;

    const FloatVec& errpos = cluster->getPositionError() ;
    for(unsigned int i=0;i<errpos.size();i++){
      LCSIO_WRITE( stream, errpos[i]  ) ;
    }
    LCSIO_WRITE( stream, cluster->getTheta()  ) ;
    LCSIO_WRITE( stream, cluster->getPhi()  ) ;

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

    const ClusterDataVec& clusters = cluster->getClustersData() ;
    int nClusters=  clusters.size() ;

    SIO_DATA( stream, &nClusters , 1  ) ;
    
    for(int i=0;i<nClusters;i++){
      SIO_PNTR( stream , &(clusters[i]) ) ;
    }

    if( LCFlagImpl(flag).bitSet( LCIO::TRBIT_HITS ) ){ 
      const StringVec colNames = cluster->getHitCollectionNames() ;
      int nHitCol =  colNames.size() ;
      SIO_DATA( stream, &nHitCol , 1  ) ;
      
      for(unsigned int i=0;i<colNames.size();i++){
	
	LCSIO_WRITE( stream, colNames[i]  ) ;
	
	const IntVec& vec = cluster->getHitIndicesForCollection( colNames[i]  ) ;
	const FloatVec& wgt = cluster->getHitContributionsForCollection( colNames[i]  ) ;
	int nHits =  vec.size() ;
	
	SIO_DATA( stream, &nHits, 1 ) ;
	
	for( int j=0 ; j<nHits  ; j++ ){
	  LCSIO_WRITE( stream, vec[j]  ) ;
	}
	for( int j=0 ; j<nHits  ; j++ ){
	  LCSIO_WRITE( stream, wgt[j]  ) ;
	}
      }
    }
    // write a ptag in order to be able to point to clusters
    SIO_PTAG( stream , cluster ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
}; // namespace
