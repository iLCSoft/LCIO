#include "IMPL/ParticleIDImpl.h"

namespace IMPL{

   ParticleIDImpl::ParticleIDImpl(){
  }
  ParticleIDImpl::~ParticleIDImpl(){
  }
   int ParticleIDImpl::getTypeID() const{
    return _typeID ; 
  }
  float ParticleIDImpl::getProbability() const{
    return _probability ;
  }
  const std::string& ParticleIDImpl::getIdentifier() const{
    return _identifier ;
  }
  const DATA::FloatVec & ParticleIDImpl::getParameters() const{
    return _parameters ;
  }

  void ParticleIDImpl::setTypeID( int typeID ){
    checkAccess("ParticleIDImpl::setTypeID") ;
    _typeID = typeID ;
  }
  
  void ParticleIDImpl::setProbability( float probability ){
    checkAccess("ParticleIDImpl::setProbability") ;
    _probability = probability ; 
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
