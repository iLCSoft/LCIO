#include "SIO/SIOVertexHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "IOIMPL/VertexIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOVertexHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	

    // create a new object :
    VertexIOImpl* vtx  = new VertexIOImpl ;
    *objP = vtx ;

    //read data
    SIO_DATA( stream ,  &(vtx->_primary)  , 1 ) ;
    SIO_DATA( stream ,  &(vtx->_chi2)  , 1 ) ;
    SIO_DATA( stream ,  &(vtx->_probability)  , 1 ) ;
    SIO_DATA( stream ,  vtx->_vpos  , 3 ) ;

    //read covMatrix
    float cov[NCOVARIANCE] ;
    SIO_DATA( stream ,  cov  ,  NCOVARIANCE ) ;
    vtx->setCovMatrix( cov ) ;

    //read parameters
    int nPara  ;
    SIO_DATA( stream ,  &nPara  , 1 ) ;
    float aParameter ;
    for(int i=0; i<nPara; i++){
      SIO_DATA( stream ,  &aParameter  , 1 ) ;
      vtx->addParameter( aParameter ) ;
    }

    //read pointer to associated reconstructed particle
    SIO_PNTR( stream ,   &(vtx->_aParticle) ) ;

    // read the pointer tag 
    SIO_PTAG( stream , dynamic_cast<const Vertex*>(vtx) ) ;
    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOVertexHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 
    
    // this is where we gave up type safety in order to
    // simplify the API and the implementation
    // by having a common collection of objects
    const Vertex* vtx = dynamic_cast<const Vertex*>(obj)  ;

    //write data
    LCSIO_WRITE( stream, vtx->isPrimary()  ) ;
    LCSIO_WRITE( stream, vtx->getChi2()  ) ;
    LCSIO_WRITE( stream, vtx->getProbability()  ) ;

    //write position of the vertex
    float* pos = const_cast<float*> ( vtx->getPosition() ) ; 
    SIO_DATA( stream,  pos , 3 ) ;
    
    //write covariance matrix
    const FloatVec& cov = vtx->getCovMatrix() ;
    for(unsigned int i=0; i<cov.size(); i++){
      LCSIO_WRITE( stream, cov[i]  ) ;
    }

    //write parameters
    int nPara = vtx->getParameters().size() ;
    SIO_DATA( stream ,  &nPara  , 1 ) ;
    for(int i=0; i<nPara; i++){
      LCSIO_WRITE( stream, vtx->getParameters()[i]  ) ;
    }

    //write pointer to associated reconstructed particle
    EVENT::ReconstructedParticle* recP = vtx->getAssociatedParticle() ;
    SIO_PNTR( stream , &recP  ) ;
	
    
    // write a ptag in order to be able to point to vertexes
    SIO_PTAG( stream , vtx ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
    
  }
  
} // namespace
