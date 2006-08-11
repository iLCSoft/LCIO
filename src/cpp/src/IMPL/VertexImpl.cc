#include "IMPL/VertexImpl.h"
#include <algorithm>

using namespace EVENT ;

namespace IMPL{

  VertexImpl::VertexImpl() :
    _primary(0),
    _chi2(0),
    _probability(0)
  {
    _cov.resize( NCOVARIANCE ) ;
    //     for(int i=0 ; i < NCOVARIANCE ; i++ ) { _cov.push_back( 0.0 ) ;  }
    _vpos[0] = 0. ;
    _vpos[1] = 0. ;
    _vpos[2] = 0. ;
  }

  VertexImpl::~VertexImpl(){	  
	  
    /*
    // delete the pids owned by this particle
    for(  ParticleIDVec::iterator iter = _pid.begin() ; iter != _pid.end() ; iter++){
      delete *iter ;
    }
    */
  }
 
  const float* VertexImpl::getPosition() const { return  _vpos ; }
  const EVENT::FloatVec & VertexImpl::getCovMatrix() const { return _cov ; }
  float VertexImpl::getChi2() const { return _chi2 ; }
  float VertexImpl::getProbability() const { return _probability ; }
  const EVENT::FloatVec & VertexImpl::getParameters() const { return _par ; }
  const EVENT::ReconstructedParticle * VertexImpl::getAssociatedParticle() const { return _aParticle  ; }
  
  bool VertexImpl::isPrimary() const { return _primary ;}


  //setters
  void VertexImpl::setPrimary(bool primary){
    checkAccess("VertexImpl::setPrimary" );
    _primary = primary ;
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
    for(int i=0;i<NCOVARIANCE;i++) _cov[i] = cov[i] ;
  }
                                                                                                                                        
  void VertexImpl::setCovMatrix( const EVENT::FloatVec& cov ){
    checkAccess("VertexImpl::" );
    for(int i=0;i<NCOVARIANCE;i++) _cov[i] = cov[i] ;
  }
  
  void VertexImpl::setParameters( const float* par ){
    checkAccess("VertexImpl::setParameters" );
    //_par = par;
  }
  
  void VertexImpl::setParameters( const EVENT::FloatVec& par ){
    checkAccess("VertexImpl::" );
    //_par = par;
  }

  void VertexImpl::setAssociatedParticle( const EVENT::ReconstructedParticle *aP ){
    checkAccess("VertexImpl::setAssociatedParticle" );
    _aParticle = aP;
  }
 
} // end namespace

