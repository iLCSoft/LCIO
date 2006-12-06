#include "SIO/SIOVertexHandler.h"

#include <algorithm>
#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "EVENT/Vertex.h"
#include "IOIMPL/VertexIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"
#include "SIO/SIOLCParameters.h"

//dbg
#include <iostream>

using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{

  SIOVertexHandler::~SIOVertexHandler(){ }

  unsigned int SIOVertexHandler::init( SIO_stream* stream,
                                       SIO_operation op,
                                       EVENT::LCCollection* col ,
                                       unsigned int vers ) {
                                                                                                                                                             
    unsigned int status ;
                                                                                                                                                             
                                                                                                                                                             
    if( op == SIO_OP_READ ) {
                                                                                                                                                             
                                                                                                                                                             
      SIO_DATA( stream ,  &_flag , 1  ) ;
                                                                                                                                                             
      if( vers > SIO_VERSION_ENCODE( 1, 1)   )
        SIOLCParameters::read( stream ,  col->parameters() , vers ) ;
      
      col->setFlag( _flag ) ;
      _vers = vers ;
 
      //get the parameters from the collection and initialize set/map with values
      imr.clear();
      //_set.clear();
      parameters.clear();
      col->getParameters().getStringVals("_lcio.VertexAlgorithmTypes",parameters);
      for(unsigned int i=0; i<parameters.size(); i++){
        //_set.insert(parameters[i]);
	imr[i]=parameters[i];
      }
    }
                                                                                                                                                             
    else if( op == SIO_OP_WRITE ) {
                                                                                                                                                             
                                                                                                                                                             
      _flag = col->getFlag() ;
                                                                                                                                                             
      LCSIO_WRITE( stream, _flag  ) ;
/*      
      //add parameters for vertex algorithm types using FIFO algorithm
      parameters.clear();
      Vertex* v;
      v = dynamic_cast<Vertex*>( col->getElementAt( 0 ) ) ;
      parameters.push_back( v->getAlgorithmType() );
      bool found;
      for(int i=0; i<col->getNumberOfElements(); i++){
	found=0;
	v = dynamic_cast<Vertex*>( col->getElementAt( i ) ) ;
	for(unsigned int j=0; j<parameters.size(); j++){
	  if( parameters[j] == v->getAlgorithmType() ){
	    found=1;
	    break;
	  }
	}
	if(!found){ parameters.push_back( v->getAlgorithmType() ); }
      }
      col->parameters().setValues("_lcio.VertexAlgorithmTypes",parameters);

      //initialize map with parameter values
      imw.clear();
      for(unsigned int i=0; i<parameters.size(); i++)
        imw[parameters[i]]=i;
*/
      parameters.clear();
      _set.clear();
      Vertex* v;
      for(int i=0; i<col->getNumberOfElements(); i++){
	v = dynamic_cast<Vertex*>( col->getElementAt( i ) ) ;
	_set.insert(v->getAlgorithmType());
      }
      for(std::set<std::string>::iterator it=_set.begin(); it!=_set.end(); it++){
	parameters.push_back(*it);
      }
      //add parameters to collection
      col->parameters().setValues("_lcio.VertexAlgorithmTypes",parameters);
      
      SIOLCParameters::write( stream , col->getParameters() ) ;
                                                                                                                                                             
      _vers = vers ;  // not really needed !?
                                                                                                                                                             
    }
    return ( SIO_BLOCK_SUCCESS ) ;
  }

  
  unsigned int SIOVertexHandler::read(SIO_stream* stream, LCObject** objP){

    unsigned int status ; 

    // create a new object :
    VertexIOImpl* vtx  = new VertexIOImpl ;
    *objP = vtx ;

    //read data
    SIO_DATA( stream ,  &(vtx->_primary)  , 1 ) ;
    int algtype;
    SIO_DATA( stream ,  &algtype  , 1 ) ;
    vtx->setAlgorithmType(imr[algtype]);
    //std::set<std::string>::iterator it;
    //advance(it=_set.begin(),algtype);
    //vtx->setAlgorithmType(*it);
    
    //dbg
    //std::cout<<"Reading... (int)["<<algtype<<"] (string)["<<(*it)<<"]\n";
    //std::cout<<"Reading... (int)["<<algtype<<"] (string)["<<imr[algtype]<<"]\n";
      
    SIO_DATA( stream ,  &(vtx->_chi2)  , 1 ) ;
    SIO_DATA( stream ,  &(vtx->_probability)  , 1 ) ;
    SIO_DATA( stream ,  vtx->_vpos  , 3 ) ;

    //read covMatrix
    float cov[VTXCOVMATRIX] ;
    SIO_DATA( stream ,  cov  ,  VTXCOVMATRIX ) ;
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
    
    
  unsigned int SIOVertexHandler::write(SIO_stream* stream, const LCObject* obj){
    
    unsigned int status ; 
    
    // this is where we gave up type safety in order to
    // simplify the API and the implementation
    // by having a common collection of objects
    const Vertex* vtx = dynamic_cast<const Vertex*>(obj)  ;

    //write data
    LCSIO_WRITE( stream, vtx->isPrimary()  ) ;
    //LCSIO_WRITE( stream, imw[vtx->getAlgorithmType()]  ) ;
   
    LCSIO_WRITE( stream, static_cast<size_t> (distance( _set.begin(),_set.find( vtx->getAlgorithmType() )))) ;
    
    //dbg
    //std::cout<<"Writing... (string)["<<vtx->getAlgorithmType()<<"] (int)["<<distance( _set.begin(),_set.find( vtx->getAlgorithmType() ))<<"]\n";
    
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
	
    
    // write a ptag in order to be able to point to vertices
    SIO_PTAG( stream , vtx ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
