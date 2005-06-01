#include "SIO/SIOParticleHandler.h"
#include "SIO/LCSIO.h"

#include "EVENT/MCParticle.h"
#include "IOIMPL/MCParticleIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"
//#include <iostream>

#include "EVENT/LCCollection.h"

using namespace EVENT ;

using namespace IOIMPL ;

namespace SIO {
  
  unsigned int SIOParticleHandler::read(SIO_stream* stream, 
					LCObject** objP){

    if ( SIO_VERSION_MAJOR(_vers)==0 && SIO_VERSION_MINOR(_vers)==8)  
      return readv00_08( stream, objP ) ;
    
    
    unsigned int status ; 
    
    // create a new object :
    MCParticleIOImpl* particle  = new MCParticleIOImpl ;
    *objP = particle ;
    
    // tell SIO the address of particle as an abstract  MCParticle ...
    // this is important, as SIO takes the bare address 
    // (int)(MCParticle*) particle != (int)particle !!!!
    SIO_PTAG( stream , dynamic_cast<MCParticle*>(particle) ) ;
    //     SIO_PNTR( stream , &(particle->_mother0) ) ;
    //     SIO_PNTR( stream , &(particle->_mother1) ) ;
    
    // parents
    int numberOfParents ; 
    SIO_DATA( stream ,  &numberOfParents , 1  ) ;
    
    //     //    particle->prepareArrayOfParents( numberOfParents ) ;
    
    // ensure vector has correct number of elements
//     for(int i=0;i<numberOfParents;i++){
//       particle->_parents.push_back( 0 ) ;
//     }
    particle->_parents.resize( numberOfParents ) ;

    for(int i=0;i<numberOfParents;i++){
      SIO_PNTR( stream , &(particle->_parents[i] ) ) ;
    }
//     for(int i=0;i<numberOfParents;i++){
      
//       // create a pointer to a pointer to a MCParticle 
//       // as SIO need the address of the pointer for pointer reallocation....
//       MCParticle** pD = new (MCParticle*) ;
//       SIO_PNTR( stream , pD ) ;
//       particle->_parents.push_back( pD ) ;
//     }
    
    SIO_DATA( stream ,  &(particle->_pdg) , 1  ) ;
    SIO_DATA( stream ,  &(particle->_genstatus) , 1  ) ;
    int simstatus ;
    SIO_DATA( stream ,  &simstatus , 1  ) ;
    particle->_simstatus = simstatus ;
    //    SIO_DATA( stream ,  &(particle->_simstatus) , 1  ) ;

    SIO_DATA( stream ,  particle->_vertex  , 3 ) ;

    if( _vers > SIO_VERSION_ENCODE( 1, 2)   ) {
      SIO_DATA( stream ,  &(particle->_time)  , 1 ) ;
    }
    
    float momentum[3] ;
    SIO_DATA( stream ,  momentum  , 3 ) ;
    particle->setMomentum( momentum ) ;

    float mass ;
    SIO_DATA( stream , &mass , 1  ) ;
    particle->setMass( mass )  ;

    SIO_DATA( stream ,  &(particle->_charge) , 1  ) ;
    
    if( particle->_simstatus.test( MCParticle::BITEndpoint ) )  // bit 31
      SIO_DATA( stream ,  particle->_endpoint  , 3 ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  unsigned int SIOParticleHandler::write(SIO_stream* stream, 
					 const LCObject* obj){

    //fg 20030609 changed to use MCParticle    
    unsigned int status ; 
    
    const MCParticle* particle  = dynamic_cast<const MCParticle*>(obj)  ;
    SIO_PTAG( stream , particle ) ;
    
//     const MCParticle* myMom0 = particle->getParent() ;
//     const MCParticle* myMom1 = particle->getSecondParent() ;
//     SIO_PNTR( stream , &myMom0 ) ;
//     SIO_PNTR( stream , &myMom1 ) ;

    int numberOfParents = particle->getParents().size() ;
    
    SIO_DATA( stream , &numberOfParents  , 1 ) ;
    
    for(int i=0;i<numberOfParents;i++){
      const MCParticle* part = particle->getParents()[i] ;
      SIO_PNTR( stream ,  &part  ); 
    }

    LCSIO_WRITE( stream, particle->getPDG() ) ;
    LCSIO_WRITE( stream, particle->getGeneratorStatus() ) ;

//     if( particle->getEndpoint() != 0 ) {
//       int simstatus = particle->getSimulatorStatus()  ;
//       simstatus |= 0x80000000 ; // set bit 31
//       LCSIO_WRITE( stream, simstatus ) ;
//     } else {
//       LCSIO_WRITE( stream, particle->getSimulatorStatus() ) ;
//     }

    LCSIO_WRITE( stream, particle->getSimulatorStatus() ) ;

    SIO_DATA( stream, const_cast<double*>( particle->getVertex() ) , 3 ) ;

    LCSIO_WRITE( stream, particle->getTime() ) ;

//     SIO_DATA( stream, const_cast<float*>( particle->getMomentum()), 3 ) ;

    LCSIO_WRITE( stream, (float) particle->getMomentum()[0] ) ;
    LCSIO_WRITE( stream, (float) particle->getMomentum()[1] ) ;
    LCSIO_WRITE( stream, (float) particle->getMomentum()[2] ) ;

    LCSIO_WRITE( stream, (float) particle->getMass() ) ;
    LCSIO_WRITE( stream, particle->getCharge() ) ;


//     if( particle->getEndpoint() != 0 ) 
    if( particle->getSimulatorStatus() &  (1<<MCParticle::BITEndpoint) )  // endpoint set !
      SIO_DATA( stream, const_cast<double*>( particle->getEndpoint() ) , 3 ) ;


    return ( SIO_BLOCK_SUCCESS ) ;

  }

  unsigned int SIOParticleHandler::readv00_08(SIO_stream* stream, 
					      LCObject** objP){
    unsigned int status ; 
    
    // create a new object :
    MCParticleIOImpl* particle  = new MCParticleIOImpl ;
    *objP = particle ;
    
    // tell SIO the address of particle as an abstract  MCParticle ...
    // this is important, as SIO takes the bare address 
    // (int)(MCParticle*) particle != (int)particle !!!!
    SIO_PTAG( stream , dynamic_cast<MCParticle*>(particle) ) ;

    int ignorePointer ;
//     SIO_PNTR( stream , &(particle->_mother0) ) ;
//     SIO_PNTR( stream , &(particle->_mother1) ) ;
    SIO_DATA( stream , &ignorePointer  , 1  ) ;
    SIO_DATA( stream , &ignorePointer  , 1  ) ;


    // daughters
    int numberOfDaughters ; 
    SIO_DATA( stream ,  &numberOfDaughters , 1  ) ;

    //    particle->prepareArrayOfDaughters( numberOfDaughters ) ;


//     for(int i=0;i<numberOfDaughters;i++){
//     }
    particle->_daughters.resize( numberOfDaughters ) ;

    for(int i=0;i<numberOfDaughters;i++){
      SIO_PNTR( stream , &(particle->_daughters[i] ) ) ;
    }
//     for(int i=0;i<numberOfDaughters;i++){
//       // create a pointer to a pointer to a MCParticle 
//       // as SIO need the address of the pointer for pointer reallocation....
//       MCParticle** pD = new (MCParticle*) ;
//       SIO_PNTR( stream , pD ) ;
//       particle->_daughters.push_back( pD ) ;
//     }

    SIO_DATA( stream ,  &(particle->_pdg) , 1  ) ;
    SIO_DATA( stream ,  &(particle->_genstatus) , 1  ) ;
    particle->_genstatus = 0 ;   // default value
    SIO_DATA( stream ,  particle->_vertex  , 3 ) ;
    SIO_DATA( stream ,  particle->_p  , 3 ) ;
    SIO_DATA( stream ,  &(particle->_mass) , 1  ) ;
    SIO_DATA( stream ,  &(particle->_charge) , 1  ) ;
    
    // if the particles doesn't have daughters we read its endpoint
    if( numberOfDaughters == 0 ) {
      SIO_DATA( stream ,  particle->_endpoint  , 3 ) ;
      particle->_genstatus = 0x80000000 ; // set bit31
    }
    return ( SIO_BLOCK_SUCCESS ) ;
  }


  void SIOParticleHandler::restoreParentDaughterRelations( EVENT::LCEvent* evt){
    
    const std::vector< std::string >* strVec = evt->getCollectionNames() ;
    std::vector< std::string >::const_iterator name ;
    
    for( name = strVec->begin() ; name != strVec->end() ; name++){
    
      LCCollection* col ;
      if( (col = evt->getCollection( *name ))->getTypeName() == LCIO::MCPARTICLE 
	  &&  ! ( col->getFlag() & ( 1 << LCCollection::BITSubset) ) ) {
	
	int nDaughtersTotal = 0 ;
	int nParentsTotal = 0 ; 
	for(int i=0; i < col->getNumberOfElements() ; i++){
	  MCParticleIOImpl* mcp = dynamic_cast<MCParticleIOImpl*>( col->getElementAt(i) ) ;
	  nDaughtersTotal += mcp->getDaughters().size()  ;
	  nParentsTotal += mcp->getParents().size() ;
	}

	for(int i=0; i < col->getNumberOfElements() ; i++){
	  
	  MCParticleIOImpl* mcp = dynamic_cast<MCParticleIOImpl*>( col->getElementAt(i) ) ;
	  
	  // for version v00-08 we restore parents from daughters
	  if ( nParentsTotal == 0 &&  nDaughtersTotal > 0 ){
	    
	    int nDaughters = mcp->getDaughters().size() ;
	    for( int j=0; j<nDaughters; j++){
	      MCParticleIOImpl* dgh = dynamic_cast<MCParticleIOImpl*>( mcp->getDaughter(j) ) ;
	      
// 	      MCParticle ** mcpP = new (MCParticle*) ;
// 	      *mcpP = mcp ;
// 	      dgh->_parents.push_back( mcpP ) ;
	      dgh->_parents.push_back( mcp ) ;
	    }
	    
	  } 
	  else if ( nParentsTotal > 0 && nDaughtersTotal == 0 ) {
	    
	    int nParents = mcp->getParents().size() ;
	    for( int j=0; j<nParents; j++){
	      MCParticleIOImpl* mom = dynamic_cast<MCParticleIOImpl*>( mcp->getParents()[j] ) ;
// 	      MCParticle ** mcpP = new (MCParticle*) ;
// 	      *mcpP = mcp ;
// 	      mom->_daughters.push_back( mcpP ) ;
	      mom->_daughters.push_back( mcp ) ;
	      
	    }
	  }

	} // loop over particles

      } // if( MCPARTICLE ) 
    } // loop over collections
  }
} // namespace
