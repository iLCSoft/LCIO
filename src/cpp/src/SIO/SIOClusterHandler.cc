#include "SIO/SIOClusterHandler.h"

// -- lcio headers
#include "EVENT/LCIO.h"
#include "IOIMPL/ParticleIDIOImpl.h"
#include "IOIMPL/ClusterIOImpl.h"
#include "IMPL/LCFlagImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {
  
  SIOClusterHandler::SIOClusterHandler() : 
    SIOObjectHandler( EVENT::LCIO::CLUSTER ) {
    
  }
  
  //----------------------------------------------------------------------------
  
  void SIOClusterHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    IOIMPL::ClusterIOImpl* cluster = dynamic_cast<IOIMPL::ClusterIOImpl*>(objP) ;
    // type is bitset<32> - can't be set directly
    int type ;
    SIO_DATA( device ,  &type  , 1 ) ;
    cluster->setType( type ) ;
    SIO_DATA( device ,  &(cluster->_energy)  , 1 ) ;
    if( vers > SIO_VERSION_ENCODE( 1, 51 ) ) {
      SIO_DATA( device ,  &(cluster->_energyError) , 1  ) ;
    }
    SIO_DATA( device ,  cluster->_position  , 3 ) ;
    float errpos[ NERRPOS ] ;
    SIO_DATA( device , &errpos[0]   , NERRPOS ) ;
    cluster->setPositionError( errpos ) ;
    SIO_DATA( device ,  &(cluster->_theta)  , 1 ) ;
    SIO_DATA( device ,  &(cluster->_phi)  , 1 ) ;
    float errdir[ NERRDIR ] ;
    SIO_DATA( device , &errdir[0]  , NERRDIR ) ;
    cluster->setDirectionError( errdir ) ;
    int nShape ;
    if( vers > SIO_VERSION_ENCODE(1,2) ) {
      SIO_DATA( device , &nShape  , 1 ) ;
    } 
    else {
      nShape = NSHAPE_OLD ;
    }
    cluster->_shape.resize( nShape )   ;
    SIO_DATA( device , &(cluster->_shape[0])  ,nShape ) ;
    if( vers > SIO_VERSION_ENCODE(1,2) ) {      
      // read PIDs
      int nPid ;
      SIO_DATA( device ,  &nPid  , 1 ) ;
      for(int i=0;i<nPid;i++) {
        // create new Pid objects
        IOIMPL::ParticleIDIOImpl* pid = new IOIMPL::ParticleIDIOImpl() ;
        SIO_DATA( device ,  &(pid->_likelihood) , 1  ) ;
        SIO_DATA( device ,  &(pid->_type) , 1  ) ;
        SIO_DATA( device ,  &(pid->_pdg) , 1  ) ;
        SIO_DATA( device ,  &(pid->_algorithmType) , 1  ) ;
        int nPara  ;
        SIO_DATA( device ,  &nPara  , 1 ) ;
        float aParameter ;
        for(int j=0;j<nPara;j++) {
          SIO_DATA( device ,  &aParameter  , 1 ) ;
          pid->addParameter( aParameter ) ;
        }
        cluster->addParticleID( pid) ;
      }
    }
    else {
      float pType[3] ;
      SIO_DATA( device , pType  , 3 ) ;    
    }
    int nClusters ; 
    SIO_DATA( device, &nClusters , 1  ) ;
    // fill the vector to have correct size
    // as we are using the addresses of the elements henceforth
    cluster->_clusters.resize( nClusters ) ;
    for(int i=0;i<nClusters;i++) {
      SIO_PNTR( device , &(cluster->_clusters[i] ) ) ;
    }
    if( IMPL::LCFlagImpl(_flag).bitSet( EVENT::LCIO::CLBIT_HITS ) ) {
      int nHits ;
      SIO_DATA( device, &nHits , 1  ) ;
      cluster->_hits.resize( nHits ) ;
      cluster->_weights.resize( nHits ) ;
      for(int i=0;i<nHits;i++) {
        SIO_PNTR( device , &(cluster->_hits[i] ) ) ;
        SIO_DATA( device , &(cluster->_weights[i] ) , 1 ) ;
      }
    }
    int nEnergies ;
    SIO_DATA( device, &nEnergies , 1  ) ;
    cluster->_subdetectorEnergies.resize( nEnergies ) ;
    for(int i=0;i<nEnergies;i++){
      SIO_DATA( device, &(cluster->_subdetectorEnergies[i] )  , 1  ) ;
    }
    // read the pointer tag 
    SIO_PTAG( device , dynamic_cast<const EVENT::Cluster*>(cluster) ) ;
  }
  
  //----------------------------------------------------------------------------
  
  void SIOClusterHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    // this is where we gave up type safety in order to
    // simplify the API and the implementation
    // by having a common collection of objects
    const EVENT::Cluster* cluster = dynamic_cast<const EVENT::Cluster*>(obj) ;
    SIO_SDATA( device, cluster->getType()  ) ;
    SIO_SDATA( device, cluster->getEnergy()  ) ;
    SIO_SDATA( device, cluster->getEnergyError()  ) ;
    SIO_DATA( device,  cluster->getPosition() , 3 ) ;
    auto& errpos = cluster->getPositionError() ;
    for(unsigned int i=0;i<errpos.size();i++) {
      SIO_SDATA( device, errpos[i]  ) ;
    }
    SIO_SDATA( device, cluster->getITheta()  ) ;
    SIO_SDATA( device, cluster->getIPhi()  ) ;
    auto& errdir = cluster->getDirectionError() ;
    for(unsigned int i=0;i<errdir.size();i++){
      SIO_SDATA( device, errdir[i]  ) ;
    }
    auto& shape = cluster->getShape() ;
    int nshape = shape.size() ;
    SIO_SDATA( device, nshape ) ;
    for(unsigned int i=0;i<shape.size();i++){
      SIO_SDATA( device, shape[i]  ) ;
    }
    // write Pids
    int nPid  = cluster->getParticleIDs().size() ;
    SIO_SDATA( device ,  nPid ) ;
    for(int i=0;i<nPid;i++) {
      auto pid = cluster->getParticleIDs()[i]  ;
      SIO_SDATA( device, pid->getLikelihood()  ) ;
      SIO_SDATA( device, pid->getType()  ) ;
      SIO_SDATA( device, pid->getPDG()  ) ;
      SIO_SDATA( device, pid->getAlgorithmType()  ) ;
      int nPara = pid->getParameters().size() ;
      SIO_DATA( device ,  &nPara  , 1 ) ;
      for(int j=0;j<nPara;j++){
        SIO_SDATA( device, pid->getParameters()[j]  ) ;
      }
    }
    auto& clusters = cluster->getClusters() ;
    int nClusters =  clusters.size() ;
    SIO_DATA( device, &nClusters , 1  ) ;
    for(int i=0;i<nClusters;i++) {
      SIO_PNTR( device , &(clusters[i]) ) ;
    }
    if( IMPL::LCFlagImpl(_flag).bitSet( EVENT::LCIO::CLBIT_HITS ) ) {
      auto& hits = cluster->getCalorimeterHits() ;
      auto& weights = cluster->getHitContributions() ;
      int nHits = hits.size() ;
      SIO_DATA( device, &nHits , 1  ) ;
      for(int i=0;i<nHits;i++){
        SIO_PNTR( device , &(hits[i]) ) ;
        SIO_SDATA( device , weights[i] ) ;
      }
    }
    auto& subdetectorEnergies = cluster->getSubdetectorEnergies() ;
    int nEnergies = subdetectorEnergies.size() ;
    SIO_SDATA( device, nEnergies  ) ;
    for(int i=0;i<nEnergies;i++) {
      SIO_SDATA( device, subdetectorEnergies[i] ) ;
    }
    // write a ptag in order to be able to point to clusters
    SIO_PTAG( device , cluster ) ;
  }
  
  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOClusterHandler::create() const {
    return new IOIMPL::ClusterIOImpl() ;
  }
  
} // namespace
