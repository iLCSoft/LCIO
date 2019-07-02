#include "SIO/SIOSimCalHitHandler.h"

// -- lcio headers
#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IOIMPL/SimCalorimeterHitIOImpl.h"
#include "IMPL/LCFlagImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOSimCalHitHandler::SIOSimCalHitHandler() :
    SIOObjectHandler( EVENT::LCIO::SIMCALORIMETERHIT ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOSimCalHitHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    auto hit  = dynamic_cast<IOIMPL::SimCalorimeterHitIOImpl*>( objP ) ;
    IMPL::LCFlagImpl flag(_flag) ;
    SIO_DATA( device ,  &(hit->_cellID0) , 1  ) ;
    // in v00-08 cellid1 has been stored by default
    if( flag.bitSet( EVENT::LCIO::CHBIT_ID1 ) || ( SIO_VERSION_MAJOR(vers)==0 && SIO_VERSION_MINOR(vers)==8) ) {
      SIO_DATA( device ,  &(hit->_cellID1) , 1  ) ;
    }
    SIO_DATA( device ,  &(hit->_energy) , 1  ) ;
    if( flag.bitSet( EVENT::LCIO::CHBIT_LONG ) ) {
      SIO_DATA( device ,  hit->_position  , 3 ) ;
    }
    // read MCContributions
    int nCon ;
    SIO_DATA( device ,  &nCon , 1  ) ;
    hit->_vec.reserve(nCon) ;
    for(int i=0; i< nCon ; i++) {
      auto mcCon = new IMPL::MCParticleCont ;
      SIO_PNTR( device , &(mcCon->Particle)  ) ;
      SIO_DATA( device , &(mcCon->Energy) , 1 ) ;
      SIO_DATA( device , &(mcCon->Time)   , 1 ) ;
      if( flag.bitSet( EVENT::LCIO::CHBIT_STEP )) {
      	if( vers > SIO_VERSION_ENCODE( 2, 10 ) ) {
      	  SIO_DATA( device , &(mcCon->Length) , 1 ) ;
      	}
      	SIO_DATA( device , &(mcCon->PDG)  , 1 ) ;
      	if( vers > SIO_VERSION_ENCODE( 1, 51 ) ) {
      	  SIO_DATA( device , &(mcCon->StepPosition[0])    , 1 ) ;
      	  SIO_DATA( device , &(mcCon->StepPosition[1])    , 1 ) ;
      	  SIO_DATA( device , &(mcCon->StepPosition[2])    , 1 ) ;
      	}
      }
      hit->_vec.push_back(  mcCon  );
    }
    // read a pointer tag for  reference to calorimeter hits
    if( vers > SIO_VERSION_ENCODE( 1, 0) ) {
      SIO_PTAG( device , dynamic_cast<const EVENT::SimCalorimeterHit*>(hit) ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOSimCalHitHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto hit = dynamic_cast<const EVENT::SimCalorimeterHit*>(obj) ;
    SIO_SDATA( device, hit->getCellID0()  ) ;
    IMPL::LCFlagImpl flag(_flag) ;
    if( flag.bitSet( EVENT::LCIO::CHBIT_ID1 ) ){
      SIO_SDATA( device, hit->getCellID1()  ) ;
    }
    SIO_SDATA( device, hit->getEnergy()  ) ;
    if( flag.bitSet( EVENT::LCIO::CHBIT_LONG ) ){
      SIO_DATA( device,  hit->getPosition() , 3 ) ;
    }
    // now the MCParticle contributions
    int nMC = hit->getNMCContributions() ;
    SIO_DATA( device,  &nMC , 1 ) ;
    for(int i=0; i<nMC ;i++) {
      auto part = hit->getParticleCont(i)  ;
      SIO_PNTR( device , &part ) ;
      SIO_SDATA( device, hit->getEnergyCont(i)  ) ;
      SIO_SDATA( device, hit->getTimeCont(i)  ) ;
      if( flag.bitSet( EVENT::LCIO::CHBIT_STEP ) ) {
      	SIO_SDATA( device, hit->getLengthCont(i)  ) ;
      	SIO_SDATA( device, hit->getPDGCont(i)  ) ;
      	float* sp = const_cast<float*> ( hit->getStepPosition(i) ) ;
      	SIO_DATA( device,  sp , 3 ) ;
      }
    }
    // add a pointer tag for reference to sim. calorimeter hits - added in v1.1
    SIO_PTAG( device , hit ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOSimCalHitHandler::create() const {
    return new IOIMPL::SimCalorimeterHitIOImpl() ;
  }

} // namespace
