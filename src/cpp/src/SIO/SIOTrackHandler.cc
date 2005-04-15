#include "SIO/SIOTrackHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "IOIMPL/TrackIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOTrackHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	

    // create a new object :
    TrackIOImpl* trk  = new TrackIOImpl ;
    *objP = trk ;
	
//     SIO_DATA( stream ,  &(trk->_type) , 1  ) ;
//     SIO_DATA( stream ,  &(trk->_p)  , 1 ) ;
//     SIO_DATA( stream ,  &(trk->_theta)  , 1 ) ;
//     SIO_DATA( stream ,  &(trk->_phi)  , 1 ) ;
//     SIO_DATA( stream ,  &(trk->_d0)  , 1 ) ;
//     SIO_DATA( stream ,  &(trk->_z0)  , 1 ) ;

//     char* dummy ; 
//     LCSIO_READ( stream,  &dummy ) ; 
//     trk->setType( dummy ) ;

    int type ;
    SIO_DATA( stream ,  &type  , 1 ) ;
    trk->setType( type ) ;  // type is bitset<32> - can't be set directly

    SIO_DATA( stream ,  &(trk->_d0)  , 1 ) ;
    SIO_DATA( stream ,  &(trk->_phi)  , 1 ) ;
    SIO_DATA( stream ,  &(trk->_omega)  , 1 ) ;
    SIO_DATA( stream ,  &(trk->_z0)  , 1 ) ;
    SIO_DATA( stream ,  &(trk->_tanLambda)  , 1 ) ;


    float cov[NCOVMATRIX] ;
    SIO_DATA( stream ,  cov  ,  NCOVMATRIX ) ;
    trk->setCovMatrix( cov ) ;

    SIO_DATA( stream ,  trk->_reference  , 3 ) ;
    //    SIO_DATA( stream ,  &(trk->_isReferencePointPCA )  , 1 ) ;

    SIO_DATA( stream ,  &(trk->_chi2)  , 1 ) ;
    SIO_DATA( stream ,  &(trk->_ndf)  , 1 ) ;

    SIO_DATA( stream ,  &(trk->_dEdx) , 1  ) ;
    SIO_DATA( stream ,  &(trk->_dEdxError) , 1  ) ;

    SIO_DATA( stream ,  &( trk->_radiusOfInnermostHit ) , 1  ) ;

    int nHitNumbers ;
    SIO_DATA( stream, &nHitNumbers , 1  ) ;
    trk->subdetectorHitNumbers().resize( nHitNumbers ) ;
    
    for(int i=0;i<nHitNumbers;i++){
      SIO_DATA( stream , &(trk->_subdetectorHitNumbers[i] ), 1 ) ;
    }
    
    int nTracks ;
    SIO_DATA( stream, &nTracks , 1  ) ;

    // fill the vector to have correct size
    // as we are using the addresses of the elements henceforth
    trk->_tracks.resize( nTracks ) ;
//     for(int i=0;i<nTracks;i++){
//       trk->_tracks.push_back( 0 ) ;
//     }
    for(int i=0;i<nTracks;i++){
      SIO_PNTR( stream , &(trk->_tracks[i] ) ) ;
    }


    if( LCFlagImpl(_flag).bitSet( LCIO::TRBIT_HITS ) ){ 

      int nHits ;
      SIO_DATA( stream, &nHits , 1  ) ;
      
      // fill the vector to have correct size
      // as we are using the addresses of the elements henceforth
//       for(int i=0;i<nHits;i++){
// 	trk->_hits.push_back( 0 ) ;
//       }
      trk->_hits.resize( nHits) ;

      for(int i=0;i<nHits;i++){
	SIO_PNTR( stream , &(trk->_hits[i] ) ) ;
      }
      
//       // hit collections
//       int nHitCol ;
//       SIO_DATA( stream, &nHitCol , 1  ) ;
      
//       for(int i=0 ; i< nHitCol ;i++){
	
// 	char* dummy ; 
// 	LCSIO_READ( stream,  &dummy ) ; 
// 	trk->_indexMap[ dummy ] = new IntVec ;
	
// 	int nHits ;
// 	SIO_DATA( stream, &nHits , 1  ) ;
	
// 	int* hitsArray = new int[ nHits ] ;
	
// 	SIO_DATA( stream, hitsArray ,  nHits ) ;
	
// 	for( int j=0 ; j< nHits ; j++ ){
// 	  trk->_indexMap[ dummy ]->push_back(  hitsArray[ j ] ) ;
// 	}
	
// 	delete[] hitsArray ;
//       }
    }

    // read the pointer tag 
    SIO_PTAG( stream , dynamic_cast<const Track*>(trk) ) ;
    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOTrackHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 
    
    // this is where we gave up type safety in order to
    // simplify the API and the implementation
    // by having a common collection of objects
    const Track* trk = dynamic_cast<const Track*>(obj)  ;

//     LCSIO_WRITE( stream, trk->getType()  ) ;
//     LCSIO_WRITE( stream, trk->getMomentum()  ) ;
//     LCSIO_WRITE( stream, trk->getTheta()  ) ;
//     LCSIO_WRITE( stream, trk->getPhi()  ) ;
//     LCSIO_WRITE( stream, trk->getD0()  ) ;
//     LCSIO_WRITE( stream, trk->getZ0()  ) ;

    LCSIO_WRITE( stream, trk->getType()  ) ;

    LCSIO_WRITE( stream, trk->getD0()  ) ;
    LCSIO_WRITE( stream, trk->getPhi()  ) ;
    LCSIO_WRITE( stream, trk->getOmega()  ) ;
    LCSIO_WRITE( stream, trk->getZ0()  ) ;
    LCSIO_WRITE( stream, trk->getTanLambda()  ) ;

    const FloatVec& cov = trk->getCovMatrix() ;
    for(unsigned int i=0;i<cov.size();i++){
      LCSIO_WRITE( stream, cov[i]  ) ;
    }

    float* pos = const_cast<float*> ( trk->getReferencePoint() ) ; 
    SIO_DATA( stream,  pos , 3 ) ;

    LCSIO_WRITE( stream, trk->getChi2()  ) ;
    LCSIO_WRITE( stream, trk->getNdf()  ) ;
    LCSIO_WRITE( stream, trk->getdEdx()  ) ;
    LCSIO_WRITE( stream, trk->getdEdxError()  ) ;

    LCSIO_WRITE( stream , trk->getRadiusOfInnermostHit()  ) ;

    const IntVec& hitNums = trk->getSubdetectorHitNumbers() ;
    int nHitNumbers = hitNums.size() ;
    SIO_DATA( stream, &nHitNumbers , 1  ) ;

    for(int i=0;i<nHitNumbers;i++){
      LCSIO_WRITE( stream , hitNums[i]  ) ;
    }

    const TrackVec& tracks = trk->getTracks() ;
    int nTracks=  tracks.size() ;

    SIO_DATA( stream, &nTracks , 1  ) ;
    
    for(int i=0;i<nTracks;i++){
//       const Track** _trkP ;
//       _trkP = const_cast<const Track**> ( &(tracks[i]) ) ;
//       SIO_PNTR( stream , _trkP ) ;
      SIO_PNTR( stream , &(tracks[i]) ) ;
    }


    if( LCFlagImpl(_flag).bitSet( LCIO::TRBIT_HITS ) ){ 

      const TrackerHitVec& hits = trk->getTrackerHits() ;
      int nHits=  hits.size() ;
      SIO_DATA( stream, &nHits , 1  ) ;
    
      for(int i=0;i<nHits;i++){
	SIO_PNTR( stream , &(hits[i]) ) ;
      }


//       const StringVec colNames = trk->getHitCollectionNames() ;
//       int nHitCol =  colNames.size() ;
//       SIO_DATA( stream, &nHitCol , 1  ) ;
      
//       for(unsigned int i=0;i<colNames.size();i++){
	
// 	LCSIO_WRITE( stream, colNames[i]  ) ;
	
// 	const IntVec& vec = trk->getHitIndicesForCollection( colNames[i]  ) ;
// 	int nHits =  vec.size() ;
	
// 	SIO_DATA( stream, &nHits, 1 ) ;
	
// 	for( int j=0 ; j<nHits  ; j++ ){
// 	  LCSIO_WRITE( stream, vec[j]  ) ;
// 	}
//       }
    }
    // write a ptag in order to be able to point to tracks
    SIO_PTAG( stream , trk ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
} // namespace
