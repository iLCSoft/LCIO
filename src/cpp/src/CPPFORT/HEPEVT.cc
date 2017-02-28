
#include "CPPFORT/HEPEVT.h"

#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std ;

using namespace lcio ;

namespace HEPEVTIMPL{

  void HEPEVT::fromHepEvt(LCEvent * evt, const char* mcpColName){

      float* p = new float[3] ;

      // create and add mc particles from stdhep COMMON
      LCCollectionVec* mcVec = new LCCollectionVec( LCIO::MCPARTICLE )  ;

      // create a particle instance for each HEPEVT entry
      // and add it to the collection - leave parent relations empty
      int* NMCPART = &FTNhep.nhep;
      for(int j=0;j < *NMCPART;j++)
      {
        MCParticleImpl* mcp = new MCParticleImpl ;
        mcp->setPDG( FTNhep.idhep[j] ) ;
        mcp->setGeneratorStatus( FTNhep.isthep[j] ) ;
        mcp->setSimulatorStatus( 0 ) ;

        // now momentum, vertex, charge
        for(int k=0;k<3;k++)  p[k] = (float)FTNhep.phep[j][k];
        mcp->setMomentum( p );
        mcp->setMass( (float)FTNhep.phep[j][4] ) ;
        mcp->setVertex( FTNhep.vhep[j] ) ;

        // finally store pointer and set charge
        mcp->setCharge( FTNhep1.mcchargev[j] ) ;

        mcVec->push_back( mcp ) ;
      }

      // now loop one more time and add parent relations
      // NB: this assumes that the parent relations are consistent, i.e. any particle
      // referred to as daughter in the hepevt common block refers to the corresponding 
      // particle as mother
      for(int j=0;j < *NMCPART;j++) {
	MCParticleImpl* mcp = dynamic_cast<MCParticleImpl*>( mcVec->getElementAt( j ) ) ;
	
        // now get parents if required and set daughter if parent1 is defined
        int parent1 = FTNhep.jmohep[j][0] ;
        int parent2 = FTNhep.jmohep[j][1] ;

	if( parent1 > 0 ) {
	  MCParticle* mom = dynamic_cast<MCParticle*>( mcVec->getElementAt( parent1-1 ) ) ;
	  mcp->addParent( mom ) ;
	  if( parent2 > 0 )
	    for(int i = parent1 ; i < parent2 ; i++ ){ 
	      MCParticle* mom2 = dynamic_cast<MCParticle*>( mcVec->getElementAt( i ) ) ;
	      mcp->addParent( mom2 ) ;
	    }
        }
      }

      std::string colName("MCParticle") ;
      if( mcpColName != 0 )
	colName = mcpColName ;

      // add all collection to the event
      evt->addCollection( (LCCollection*) mcVec , colName ) ;

      // now fill pointers for MCParticle collection
      LCEventImpl* evtimpl = reinterpret_cast<LCEventImpl*>(evt) ;
      LCCollection* getmcVec = evtimpl->getCollection( "MCParticle" ) ;
      int nelem = getmcVec->getNumberOfElements() ;
      for(int j=0;j < nelem; j++)
      {
        FTNhep1.mcpointerv[j] = reinterpret_cast<PTRTYPE>( getmcVec->getElementAt( j ) ) ;
      }

      delete[] p ;
  }  // end of fromHepEvt

/* ============================================================================================================= */

  void HEPEVT::toHepEvt(const LCEvent* evt, const char* mcpColName){

      int* kmax      = new int ;
      double* maxxyz = new double;


      // set event number in stdhep COMMON
      FTNhep.nevhep = evt->getEventNumber() ;

      std::string colName("MCParticle") ;
      if( mcpColName != 0 )
	colName = mcpColName ;

      // fill MCParticles into stdhep COMMON
      LCCollection* mcVec = evt->getCollection( colName )  ;
      FTNhep.nhep = mcVec->getNumberOfElements() ;
      int* NMCPART = &FTNhep.nhep ;

      for(int j=0;j < *NMCPART;j++)
      {

        //for each MCParticle fill hepevt common line
        const MCParticle* mcp = 
        dynamic_cast<const MCParticle*>( mcVec->getElementAt( j ) ) ;

        FTNhep.idhep[j] = mcp->getPDG() ;
        FTNhep.isthep[j] = mcp->getGeneratorStatus() ;

        // store mother indices
        FTNhep.jmohep[j][0] = 0 ;
        FTNhep.jmohep[j][1] = 0 ;
        const MCParticle* mcpp  = 0 ;
        int nparents = mcp->getParents().size() ;
	if(  nparents > 0 ) mcpp = mcp->getParents()[0] ;
	
        try{
          for(int jjm=0;jjm < *NMCPART;jjm++)
	    {
	      if (mcpp  == dynamic_cast<const MCParticle*>(mcVec->getElementAt( jjm )) ){
		FTNhep.jmohep[j][0] = jjm + 1 ;
		break ;
	      }
	    }
        }catch(exception& e){
        }
        if (  FTNhep.jmohep[j][0] > 0 )
        {
          try{
	    const MCParticle* mcpsp  = 0 ;
	    if(  mcp->getParents().size() > 1 ) mcpsp = mcp->getParents()[ nparents-1 ] ;

            for(int jjj=0;jjj < *NMCPART;jjj++)
            {
                 
              if (mcpsp  == dynamic_cast<const MCParticle*>(mcVec->getElementAt( jjj )) ){
                FTNhep.jmohep[j][1] = jjj + 1 ;
                break ;
              }
            }
          }catch(exception& e){
            FTNhep.jmohep[j][1] = 0 ;
          }
        }


        // store daugther indices
        FTNhep.jdahep[j][0] = 0 ;
        FTNhep.jdahep[j][1] = 0 ;
        // for the StdHep convention particles with GeneratorStatus = 3 have no daughters
        if ( FTNhep.isthep[j] != 3 )
        {
          int ndaugthers = mcp->getDaughters().size() ;

          if (ndaugthers > 0)
          {
             const MCParticle* mcpd = mcp->getDaughters()[0] ;
             for (int jjj=0; jjj < *NMCPART; jjj++)
             {
               const MCParticle* mcpdtest = dynamic_cast<const MCParticle*>(mcVec->getElementAt( jjj )) ;
               if ( mcpd == mcpdtest )
               {
                 FTNhep.jdahep[j][0] = jjj + 1 ;
                 FTNhep.jdahep[j][1] = FTNhep.jdahep[j][0] + ndaugthers -1 ;
                 break ;
               }
             }
          }
        }

        // now momentum, energy, and mass
        for(int k=0;k<3;k++)  FTNhep.phep[j][k] = (double)mcp->getMomentum()[k] ;
        FTNhep.phep[j][3] = (double)mcp->getEnergy() ;
        FTNhep.phep[j][4] = (double)mcp->getMass() ;

        // get vertex and production time
        *kmax   = 0 ;
        *maxxyz = 0. ;
        for(int k=0;k<3;k++){
          FTNhep.vhep[j][k] = mcp->getVertex()[k] ;
          if ( fabs( FTNhep.vhep[j][k] ) > *maxxyz ){
            *maxxyz = fabs( FTNhep.vhep[j][k] ) ;
            *kmax = k ;
          }
        }
        if ( mcpp != 0 && *maxxyz > 0. )
        {
          FTNhep.vhep[j][3] = FTNhep.vhep[FTNhep.jmohep[j][0]-1][3]
                              + (mcp->getVertex()[*kmax] - mcpp->getVertex()[*kmax]) * mcpp->getEnergy()
                              / mcpp->getMomentum()[*kmax] ;
        }
        else
        {
          FTNhep.vhep[j][3] = 0. ;  // no production time for MCParticle
        }

        // finally store pointer and get charge
        FTNhep1.mcpointerv[j] = reinterpret_cast<PTRTYPE>( (mcp) ) ;
        FTNhep1.mcchargev[j] = mcp->getCharge() ;
      }

      delete kmax ;
      delete maxxyz ;
  } // end of toHepEvt

} //namespace HEPEVTIMPL

