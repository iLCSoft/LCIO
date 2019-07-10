#include "SIO/SIOVertexHandler.h"

// -- lcio headers
#include "EVENT/LCIO.h"
#include "EVENT/Vertex.h"
#include "IOIMPL/VertexIOImpl.h"
#include "IMPL/LCFlagImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

// -- std headers
#include <algorithm>

namespace SIO {

  SIOVertexHandler::SIOVertexHandler() :
    SIOObjectHandler( EVENT::LCIO::VERTEX ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOVertexHandler::initReading( sio::read_device &device, EVENT::LCCollection *collection, sio::version_type vers ) {
    SIOObjectHandler::initReading( device, collection, vers ) ;
    _imr.clear() ;
    _parameters.clear() ;
    collection->getParameters().getStringVals("_lcio.VertexAlgorithmTypes" , _parameters ) ;
    for(unsigned int i=0; i<_parameters.size(); i++) {
      _imr[i] = _parameters[i] ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOVertexHandler::initWriting( sio::write_device &device, EVENT::LCCollection *collection ) {
    _parameters.clear() ;
    _set.clear() ;
    for( int i=0 ; i<collection->getNumberOfElements() ; i++) {
      auto v = dynamic_cast<EVENT::Vertex*>( collection->getElementAt( i ) ) ;
      _set.insert(v->getAlgorithmType());
    }
    for( auto it=_set.begin() ; it != _set.end() ; it++ ) {
      _parameters.push_back(*it) ;
    }
    collection->parameters().setValues("_lcio.VertexAlgorithmTypes", _parameters ) ;
    SIOObjectHandler::initWriting( device, collection ) ;
  }

  //----------------------------------------------------------------------------

  void SIOVertexHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type /*vers*/ ) {
    auto vtx = dynamic_cast<IOIMPL::VertexIOImpl*>(objP) ;
    SIO_DATA( device ,  &(vtx->_primary)  , 1 ) ;
    int algtype;
    SIO_DATA( device ,  &algtype  , 1 ) ;
    vtx->setAlgorithmType( _imr[algtype] ) ;
    SIO_DATA( device ,  &(vtx->_chi2)  , 1 ) ;
    SIO_DATA( device ,  &(vtx->_probability)  , 1 ) ;
    SIO_DATA( device ,  vtx->_vpos  , 3 ) ;
    float cov[VTXCOVMATRIX] ;
    SIO_DATA( device ,  &cov[0]  ,  VTXCOVMATRIX ) ;
    vtx->setCovMatrix( cov ) ;
    int nPara  ;
    SIO_DATA( device ,  &nPara  , 1 ) ;
    float aParameter ;
    for( int i=0 ; i<nPara ; i++ ) {
      SIO_DATA( device ,  &aParameter  , 1 ) ;
      vtx->addParameter( aParameter ) ;
    }
    //read pointer to associated reconstructed particle
    SIO_PNTR( device ,   &(vtx->_aParticle) ) ;
    // read the pointer tag
    SIO_PTAG( device , dynamic_cast<const EVENT::Vertex*>(vtx) ) ;
  }

  //----------------------------------------------------------------------------

  void SIOVertexHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto vtx = dynamic_cast<const EVENT::Vertex*>(obj)  ;
    SIO_SDATA( device, vtx->isPrimary()  ) ;
    SIO_SDATA( device, static_cast<int> (distance( _set.begin(),_set.find( vtx->getAlgorithmType() )))) ;
    SIO_SDATA( device, vtx->getChi2()  ) ;
    SIO_SDATA( device, vtx->getProbability()  ) ;
    SIO_DATA( device,  vtx->getPosition() , 3 ) ;
    auto cov = vtx->getCovMatrix() ;
    for( unsigned int i=0 ; i<cov.size() ; i++ ) {
      SIO_SDATA( device, cov[i]  ) ;
    }
    int nPara = vtx->getParameters().size() ;
    SIO_DATA( device ,  &nPara  , 1 ) ;
    for( int i=0 ; i<nPara ; i++ ) {
      SIO_SDATA( device, vtx->getParameters()[i]  ) ;
    }
    //write pointer to associated reconstructed particle
    auto recP = vtx->getAssociatedParticle() ;
    SIO_PNTR( device , &recP  ) ;
    // write a ptag in order to be able to point to vertices
    SIO_PTAG( device , vtx ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOVertexHandler::create() const {
    return new IOIMPL::VertexIOImpl() ;
  }

} // namespace
