#include "IMPL/VertexImpl.h"
#include <algorithm>

using namespace EVENT ;

namespace IMPL{

  VertexImpl::VertexImpl() :
    _primary(0),
    _chi2(0),
    _probability(0),
    _par(0),
    _aParticle(0)
  {
    _type="Unknown";
    _cov.resize( VTXCOVMATRIX ) ;
    _vpos[0] = 0. ;
    _vpos[1] = 0. ;
    _vpos[2] = 0. ;
  }

  VertexImpl::~VertexImpl(){ }
 
  bool VertexImpl::isPrimary() const { return _primary ; }
  //int VertexImpl::getAlgorithmType() const { return _type ; }
  const std::string& VertexImpl::getAlgorithmType() const { return _type ; }
  float VertexImpl::getChi2() const { return _chi2 ; }
  float VertexImpl::getProbability() const { return _probability ; }
  const float* VertexImpl::getPosition() const { return  _vpos ; }
  const EVENT::FloatVec & VertexImpl::getCovMatrix() const { return _cov ; }
  const EVENT::FloatVec & VertexImpl::getParameters() const { return _par ; }
  EVENT::ReconstructedParticle * VertexImpl::getAssociatedParticle() const { return _aParticle  ; }


  //setters
  void VertexImpl::setPrimary(bool primary){
    checkAccess("VertexImpl::setPrimary" );
    _primary = (primary==0?0:1);
  }
/*
  void VertexImpl::setAlgorithmType( int type ){
    checkAccess("VertexImpl::setAlgorithmType");
    _type = type;
  }
*/
  void VertexImpl::setAlgorithmType( const std::string type ){
    checkAccess("VertexImpl::setAlgorithmType");
    _type = type;
  }
 
  void VertexImpl::setChi2(float chi2){
    checkAccess("VertexImpl::setChi2" );
    _chi2 = chi2 ;
  }
  
  void VertexImpl::setProbability(float probability){
    checkAccess("VertexImpl::setProbability" );
    _probability = probability ;
  }
 
  void VertexImpl::setPosition( float vpos[3] ){
    setPosition( vpos[0], vpos[1], vpos[2] ) ;
  }

  void VertexImpl::setPosition( float px, float py, float pz ){
    checkAccess("VertexImpl::setPosition" );
    _vpos[0]  = px ;
    _vpos[1]  = py ;
    _vpos[2]  = pz ;
  }
                                                                                                                                        
  void VertexImpl::setCovMatrix( const float* cov ){
    checkAccess("VertexImpl::setCovMatrix" );
    for(int i=0;i<VTXCOVMATRIX;i++) _cov[i] = cov[i] ;
  }
                                                                                                                                        
  void VertexImpl::setCovMatrix( const EVENT::FloatVec& cov ){
    checkAccess("VertexImpl::" );
    for(int i=0;i<VTXCOVMATRIX;i++) _cov[i] = cov[i] ;
  }

  void VertexImpl::setAssociatedParticle( EVENT::ReconstructedParticle *aP ){
    checkAccess("VertexImpl::setAssociatedParticle" );
    _aParticle = aP;
  }

  void VertexImpl::addParameter( float p ){
    checkAccess("VertexImpl::addParameter") ;
    _par.push_back( p ) ;
  }
 
} // end namespace




