#include "IMPL/ParticleIDImpl.h"

namespace IMPL{

  ParticleIDImpl::ParticleIDImpl() :
    _type(0) ,
    _pdg(0) ,
    _loglikelihood(0) {
  }

  ParticleIDImpl::~ParticleIDImpl(){
  }

  int ParticleIDImpl::getType() const{
    return _type ; 
  }
  int ParticleIDImpl::getPDG() const{
    return _pdg ; 
  }
  float ParticleIDImpl::getLoglikelihood() const{
    return _loglikelihood ;
  }
  const std::string& ParticleIDImpl::getIdentifier() const{
    return _identifier ;
  }
  const EVENT::FloatVec & ParticleIDImpl::getParameters() const{
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

  void ParticleIDImpl::setLoglikelihood( float logL ){
    checkAccess("ParticleIDImpl::setLoglikelihood") ;
    _loglikelihood = logL ; 
  }
  
  void ParticleIDImpl::setIdentifier(std::string identifier ) {
    checkAccess("ParticleIDImpl::setIdentifier") ;
    _identifier = identifier ;
  }
  
  void ParticleIDImpl::addParameter( float p ){
    checkAccess("ParticleIDImpl::addParameter") ;
    _parameters.push_back( p ) ;
  }
  
} ;
