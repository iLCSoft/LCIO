#include "SIO/SIOParticleHandler.h"
#include "EVENT/MCParticle.h"
#include "IOIMPL/MCParticleIOImpl.h"
#include "EVENT/LCCollection.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOParticleHandler::SIOParticleHandler() :
    SIOObjectHandler( EVENT::LCIO::MCPARTICLE ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOParticleHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    auto particle = dynamic_cast<IOIMPL::MCParticleIOImpl*>(objP) ;
    SIO_PTAG( device , dynamic_cast<EVENT::MCParticle*>(particle) ) ;
    int numberOfParents ;
    SIO_SDATA( device , numberOfParents ) ;
    particle->_parents.resize( numberOfParents ) ;
    for( int i=0 ; i<numberOfParents ; i++ ) {
      SIO_PNTR( device , &(particle->_parents[i] ) ) ;
    }
    SIO_DATA( device ,  &(particle->_pdg) , 1  ) ;
    SIO_DATA( device ,  &(particle->_genstatus) , 1  ) ;
    int simstatus ;
    SIO_DATA( device ,  &simstatus , 1  ) ;
    particle->_simstatus = simstatus ;
    SIO_DATA( device ,  particle->_vertex  , 3 ) ;
    if( vers > SIO_VERSION_ENCODE( 1, 2 ) ) {
      SIO_DATA( device ,  &(particle->_time)  , 1 ) ;
    }
    float momentum[3] ;
    SIO_DATA( device ,  momentum  , 3 ) ;
    particle->setMomentum( momentum ) ;
    float mass ;
    SIO_DATA( device , &mass , 1  ) ;
    particle->setMass( mass )  ;
    SIO_DATA( device ,  &(particle->_charge) , 1  ) ;
    if( particle->_simstatus.test( EVENT::MCParticle::BITEndpoint ) ) { // bit 31
      SIO_DATA( device ,  particle->_endpoint  , 3 ) ;
      if( vers > SIO_VERSION_ENCODE( 2, 6 )   ) {
        float mom[3] ;
        SIO_DATA( device ,  mom  , 3 ) ;
        particle->setMomentumAtEndpoint( mom ) ;
      }
    }
    if( vers > SIO_VERSION_ENCODE( 1, 51 ) ) {
      SIO_DATA( device ,  particle->_spin  , 3 ) ;
      SIO_DATA( device ,  particle->_colorFlow  , 2 ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOParticleHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto particle = dynamic_cast<const EVENT::MCParticle*>( obj ) ;
    SIO_PTAG( device , particle ) ;
    int numberOfParents = particle->getParents().size() ;
    SIO_DATA( device , &numberOfParents  , 1 ) ;
    for(int i=0;i<numberOfParents;i++){
      auto part = particle->getParents()[i] ;
      SIO_PNTR( device ,  &part  );
    }
    SIO_SDATA( device, particle->getPDG() ) ;
    SIO_SDATA( device, particle->getGeneratorStatus() ) ;
    SIO_SDATA( device, particle->getSimulatorStatus() ) ;
    SIO_DATA( device, particle->getVertex(), 3 ) ;
    SIO_SDATA( device, particle->getTime() ) ;
    SIO_SDATA( device, static_cast<float>(particle->getMomentum()[0]) ) ;
    SIO_SDATA( device, static_cast<float>(particle->getMomentum()[1]) ) ;
    SIO_SDATA( device, static_cast<float>(particle->getMomentum()[2]) ) ;
    SIO_SDATA( device, static_cast<float>(particle->getMass() ) ) ;
    SIO_SDATA( device, particle->getCharge() ) ;
    if( particle->getSimulatorStatus() &  (1<<EVENT::MCParticle::BITEndpoint) ) { // endpoint set !
      SIO_DATA( device, particle->getEndpoint() , 3 ) ;
      SIO_SDATA( device, static_cast<float>(particle->getMomentumAtEndpoint()[0]) ) ;
      SIO_SDATA( device, static_cast<float>(particle->getMomentumAtEndpoint()[1]) ) ;
      SIO_SDATA( device, static_cast<float>(particle->getMomentumAtEndpoint()[2]) ) ;
    }
    SIO_SDATA( device, (float) particle->getSpin()[0] ) ;
    SIO_SDATA( device, (float) particle->getSpin()[1] ) ;
    SIO_SDATA( device, (float) particle->getSpin()[2] ) ;
    SIO_SDATA( device, (int) particle->getColorFlow()[0] ) ;
    SIO_SDATA( device, (int) particle->getColorFlow()[1] ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOParticleHandler::create() const {
    return new IOIMPL::MCParticleIOImpl() ;
  }

  //----------------------------------------------------------------------------

  void SIOParticleHandler::restoreParentDaughterRelations( EVENT::LCEvent* evt ) {
    auto strVec = evt->getCollectionNames() ;
    for( auto name = strVec->begin() ; name != strVec->end() ; name++ ) {
      EVENT::LCCollection* col = evt->getCollection( *name ) ;
      if( (col->getTypeName() == EVENT::LCIO::MCPARTICLE) && ! ( col->getFlag() & ( 1 << EVENT::LCCollection::BITSubset) ) ) {

      	int nDaughtersTotal = 0 ;
      	int nParentsTotal = 0 ;
      	for(int i=0; i < col->getNumberOfElements() ; i++) {
      	  auto mcp = dynamic_cast<IOIMPL::MCParticleIOImpl*>( col->getElementAt(i) ) ;
      	  nDaughtersTotal += mcp->getDaughters().size()  ;
      	  nParentsTotal += mcp->getParents().size() ;
      	}
      	for(int i=0; i < col->getNumberOfElements() ; i++) {
      	  auto mcp = dynamic_cast<IOIMPL::MCParticleIOImpl*>( col->getElementAt(i) ) ;
      	  // for version v00-08 we restore parents from daughters
      	  if ( nParentsTotal == 0 &&  nDaughtersTotal > 0 ) {
      	    int nDaughters = mcp->getDaughters().size() ;
      	    for( int j=0; j<nDaughters; j++) {
      	      auto dgh = dynamic_cast<IOIMPL::MCParticleIOImpl*>( mcp->getDaughters()[j] ) ;
      	      dgh->_parents.push_back( mcp ) ;
      	    }
      	  }
      	  else if ( nParentsTotal > 0 && nDaughtersTotal == 0 ) {
      	    int nParents = mcp->getParents().size() ;
      	    for( int j=0; j<nParents; j++){
      	      auto mom = dynamic_cast<IOIMPL::MCParticleIOImpl*>( mcp->getParents()[j] ) ;
      	      mom->_daughters.push_back( mcp ) ;
      	    }
      	  }
      	} // loop over particles
      } // if( MCPARTICLE )
    } // loop over collections
  }

} // namespace
