#include "IMPL/ParticleIDImpl.h"

namespace IMPL{

  ParticleIDImpl::ParticleIDImpl() :
    _type(0) ,
    _pdg(0) ,
    _likelihood(0) {
  }

  ParticleIDImpl::~ParticleIDImpl(){
  }

  int ParticleIDImpl::getType() const{
    return _type ; 
  }
  int ParticleIDImpl::getPDG() const{
    return _pdg ; 
  }
  float ParticleIDImpl::getLikelihood() const{
    return _likelihood ;
  }
  int ParticleIDImpl::getAlgorithmType() const{
    return _algorithmType ;
  }
  const EVENT::FloatVec & ParticleIDImpl::getParameters() const{
    return _parameters ;
  }

  EVENT::FloatVec& ParticleIDImpl::parameters() {
    return _parameters ;
  }

  void ParticleIDImpl::setType( int type ){
    checkAccess("ParticleIDImpl::setType") ;
    _type = type ;
  }
  void ParticleIDImpl::setPDG( int pdg ){
    checkAccess("ParticleIDImpl::setPDG") ;
    _pdg = pdg ;
  }

  void ParticleIDImpl::setLikelihood( float logL ){
    checkAccess("ParticleIDImpl::setLikelihood") ;
    _likelihood = logL ; 
  }
  
  void ParticleIDImpl::setAlgorithmType(int algorithmType ) {
    checkAccess("ParticleIDImpl::setAlgorithmType") ;
    _algorithmType = algorithmType ;
  }
  
  void ParticleIDImpl::addParameter( float p ){
    checkAccess("ParticleIDImpl::addParameter") ;
    _parameters.push_back( p ) ;
  }
  
}
