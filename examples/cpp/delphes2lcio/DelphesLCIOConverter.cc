
/** \class DelphesLCIOConverter
 *
 *  Class handling output of Delphes ROOT tree in LCIO
 *
 *  \author F.Gaede, DESY
 *  \date June 2020
 *
 */

#include "DelphesLCIOConverter.h"
#include "classes/DelphesClasses.h"

//#include "TROOT.h"
#include "TClonesArray.h"
#include "TTree.h"

#include "lcio.h"
#include "IO/LCWriter.h"
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCRunHeaderImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/MCParticleImpl.h" 
#include "IMPL/LCTOOLS.h"


#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace lcio;

//-----------------------------------------------------------------

DelphesLCIOConverter::DelphesLCIOConverter(const char* fileName ) {

  std::string fn(fileName) ;
  
  if( ! fn.empty()){
    fWriter = lcio::LCFactory::getInstance()->createLCWriter() ;
    fWriter->open( fn ) ;
  }

}
//-----------------------------------------------------------------

DelphesLCIOConverter::~DelphesLCIOConverter(){
  if( fWriter )
    delete fWriter ;
}
//-----------------------------------------------------------------

void DelphesLCIOConverter::writeEvent(TTree* tree){

  auto* evt = new LCEventImpl ;

  convertTree2LCIO( tree, evt ) ;
  
  if( fWriter )
    fWriter->writeEvent( evt ) ;

  delete evt;
}
//-----------------------------------------------------------------

void DelphesLCIOConverter::convertTree2LCIO( TTree *tree , lcio::LCEvent* evt){


  int nEntry =  tree->GetEntriesFast() ;
  
  auto* mcps = new LCCollectionVec( LCIO::MCPARTICLE )  ;

  size_t n = tree->GetListOfBranches()->GetEntries();
  for( size_t i = 0; i < n; ++ i ) {
    TBranch *br = dynamic_cast<TBranch*>(tree->GetListOfBranches()->At(i));
//    cout << " *************** branch \"" << br->GetName() << " --- " << br->GetClassName() << std::endl;

    
    if( !strcmp( br->GetName(), "Particle" ) ){
      TClonesArray* col = *(TClonesArray**) br->GetAddress()  ;

      int ngp = col->GetEntriesFast();
      
      for(int j = 0; j < ngp ; ++j){
       	GenParticle* p = (GenParticle*) col->At(j);

	auto* mcp = new MCParticleImpl ;
	mcps->addElement( mcp ) ;

	mcp->setPDG(p->PID) ;
	double m[3] = { p->Px , p->Py , p->Pz }  ;
	mcp->setMomentum( m ) ;
	mcp->setGeneratorStatus( p->Status );
	mcp->setCharge( p->Charge );
	mcp->setMass( p->Mass );
	mcp->setTime( p->T ) ;
	double v[3] = { p->X , p->Y , p->Z }  ;
	mcp->setVertex( v ) ;
      }
      for(int j = 0; j < ngp ; ++j){
	GenParticle* p = (GenParticle*) col->At(j);
	auto* mcp = (MCParticleImpl*) mcps->getElementAt(j) ;

	if( p->M1 > -1 )
	  mcp->addParent( (MCParticle*) mcps->getElementAt( p->M1 ) ) ;
	if( p->M2 > -1 )
	  mcp->addParent( (MCParticle*) mcps->getElementAt( p->M2 ) ) ;
      }
    }

  }

  evt->addCollection( mcps, "MCParticle" ) ;


  LCTOOLS::dumpEvent( evt ) ;


}

//======================================================================
