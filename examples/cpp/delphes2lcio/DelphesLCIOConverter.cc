
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
#include "IMPL/ReconstructedParticleImpl.h"
#include "IMPL/ParticleIDImpl.h"
#include "UTIL/LCTOOLS.h"
#include "UTIL/LCRelationNavigator.h"

#include <iostream>
#include <sstream>
#include <stdexcept>


//-----------------------------------------------------------------

DelphesLCIOConverter::DelphesLCIOConverter(const char* fileName ) {

  std::string fn(fileName) ;
  
  if( ! fn.empty()){
    fWriter = lcio::LCFactory::getInstance()->createLCWriter() ;
    fWriter->open( fn , lcio::LCIO::WRITE_NEW ) ;
  }

}
//-----------------------------------------------------------------

DelphesLCIOConverter::~DelphesLCIOConverter(){
  if( fWriter )
    delete fWriter ;
}
//-----------------------------------------------------------------

void DelphesLCIOConverter::writeEvent(TTree* tree){

  auto* evt = new lcio::LCEventImpl ;

  convertTree2LCIO( tree, evt ) ;
  
  if( fWriter )
    fWriter->writeEvent( evt ) ;

  delete evt;
}
//-----------------------------------------------------------------

void DelphesLCIOConverter::convertTree2LCIO( TTree *tree , lcio::LCEventImpl* evt){


  //int nEntry =  tree->GetEntriesFast() ;

//  size_t n = tree->GetListOfBranches()->GetEntries();
//  for( size_t i = 0; i < n; ++ i ) {
//    TBranch *br = dynamic_cast<TBranch*>(tree->GetListOfBranches()->At(i));
//    if( !strcmp(br->GetClassName(),"TClonesArray") )
//      std::cout << " *************** branch \"" << br->GetName() << " --- " << br->GetClassName() << std::endl;
//      *************** branch "Event --- TClonesArray
//      *************** branch "Particle --- TClonesArray
//      *************** branch "GenJet --- TClonesArray
//      *************** branch "GenMissingET --- TClonesArray
//      *************** branch "Track --- TClonesArray
//      *************** branch "Tower --- TClonesArray
//      *************** branch "EFlowTrack --- TClonesArray
//      *************** branch "EFlowPhoton --- TClonesArray
//      *************** branch "EFlowNeutralHadron --- TClonesArray
//      *************** branch "Photon --- TClonesArray
//      *************** branch "Electron --- TClonesArray
//      *************** branch "Muon --- TClonesArray
//      *************** branch "Jet --- TClonesArray
//      *************** branch "MissingET --- TClonesArray
//      *************** branch "ScalarHT --- TClonesArray
//  }

  // create the mandatory LCIO collections
  auto* mcps = new lcio::LCCollectionVec( lcio::LCIO::MCPARTICLE )  ;
  evt->addCollection( mcps, "MCParticle" ) ;

  auto* pfos = new lcio::LCCollectionVec( lcio::LCIO::RECONSTRUCTEDPARTICLE )  ;
  evt->addCollection( pfos, "PFOs" ) ;

  auto* jets = new lcio::LCCollectionVec( lcio::LCIO::RECONSTRUCTEDPARTICLE )  ;
  evt->addCollection( jets, "Jets" ) ;

  lcio::LCRelationNavigator recmcNav( lcio::LCIO::RECONSTRUCTEDPARTICLE , lcio::LCIO::MCPARTICLE  ) ;
  lcio::LCRelationNavigator mcrecNav( lcio::LCIO::MCPARTICLE , lcio::LCIO::RECONSTRUCTEDPARTICLE ) ;

  std::map< unsigned, lcio::MCParticleImpl*> mcpd2lmap ;
  std::map< unsigned, lcio::ReconstructedParticleImpl*> recd2lmap ;

  //=====================================================================
  TBranch *evB = tree->GetBranch("Event");

  if( evB != nullptr ){
    TClonesArray* col = *(TClonesArray**) evB->GetAddress()  ;
    LHEFEvent* e = (LHEFEvent*) col->At(0);

    // Float_t ReadTime; // read time
    // Float_t ProcTime; // processing time
    // Int_t Trigger; // trigger word
    // Int_t ProcessID; // subprocess code for the event | hepup.IDPRUP
    // Float_t Weight; // weight for the event | hepup.XWGTUP
    // Float_t CrossSection; // cross-section (read from init, implemented only for Wizard evgen)
    // Float_t ScalePDF; // scale in GeV used in the calculation of the PDFs in the event | hepup.SCALUP
    // Float_t AlphaQED; // value of the QED coupling used in the event | hepup.AQEDUP
    // Float_t AlphaQCD; // value of the QCD coupling used in the event | hepup.AQCDUP

    evt->setRunNumber( e->ProcessID ) ;  // is this useful ?
    evt->setEventNumber( e->Number ) ;
    evt->setWeight( e->Weight ) ;
    evt->setTimeStamp( e->ReadTime ) ;
  }

  //=====================================================================

  TBranch *gpB = tree->GetBranch("Particle");

  if( gpB != nullptr ){

    TClonesArray* col = *(TClonesArray**) gpB->GetAddress()  ;

    int ngp = col->GetEntriesFast();

    for(int j = 0; j < ngp ; ++j){
      GenParticle* p = (GenParticle*) col->At(j);

      auto* mcp = new lcio::MCParticleImpl ;
      mcps->addElement( mcp ) ;

      mcpd2lmap.insert( std::make_pair( p->GetUniqueID() , mcp ) ) ;
      
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
      auto* mcp = static_cast<lcio::MCParticleImpl*>( mcps->getElementAt(j) ) ;

      if( p->M1 > -1 )
	mcp->addParent( (lcio::MCParticle*) mcps->getElementAt( p->M1 ) ) ;
      if( p->M2 > -1 )
	mcp->addParent( (lcio::MCParticle*) mcps->getElementAt( p->M2 ) ) ;
    }
  }
  //=====================================================================


  TBranch *trB = tree->GetBranch("EFlowTrack");
  if( trB != nullptr ){

    TClonesArray* col = *(TClonesArray**) trB->GetAddress()  ;
    int nnh = col->GetEntriesFast();

    for(int j = 0; j < nnh ; ++j){

      Track* trk = static_cast<Track*> (col->At(j));

      auto* pfo = new lcio::ReconstructedParticleImpl ;
      pfos->addElement( pfo ) ;

      recd2lmap.insert( std::make_pair( trk->GetUniqueID() , pfo ) ) ;

      GenParticle* mcpd = (GenParticle*)trk->Particle.GetObject()  ;
      auto it = mcpd2lmap.find( mcpd->GetUniqueID() );
      if( it != mcpd2lmap.end() ){

	mcrecNav.addRelation( it->second, pfo, 1.0 ) ;
	recmcNav.addRelation( pfo, it->second, 1.0 ) ;
      } else {
	std::cout << " ### found no MC truth delphes particle ######### " << mcpd->PID<< "  uid: " <<  mcpd->GetUniqueID()  << std::endl ;
      }

      pfo->setType( 211 * trk->Charge ); // use pions for all tracks
      double mass = 0.1395701835 ; // use pion mass

      pfo->setMass( mass ) ;
      double p = trk->P ;
      double e = sqrt( p*p + mass*mass ) ;
      double th = 2.*atan( exp( - trk->Eta ) );
      double ph = trk->Phi ;
      double m[3] = { p * cos( ph ) * sin( th ) , p * sin( ph ) * sin( th ) ,   p * cos( th ) } ;

      pfo->setEnergy( e ) ;
      pfo->setMomentum( m ) ;
//      pfo->setCovMatrix (const float *cov) ; //fixme
      pfo->setCharge( trk->Charge ) ;

      auto* pid = new lcio::ParticleIDImpl ;
      pid->setPDG( trk->PID ) ;   // write the PID from Delphes here
      pid->setLikelihood( 1. ) ;

      pfo->addParticleID( pid ) ;
      pfo->setParticleIDUsed( pid );
      pfo->setGoodnessOfPID( 1. ) ;

      float ref[3] = { trk->X,  trk->Y,  trk->Z  } ;
      pfo->setReferencePoint ( ref ) ;

      //pfo->addParticle (EVENT::ReconstructedParticle *particle)
      //pfo->addCluster (EVENT::Cluster *cluster)
      //pfo->Add a cluster that has been used to create this particle.
      //pfo->addTrack (EVENT::Track *track)
      //pfo->Add a track that has been used to create this particle.
      //pfo->setStartVertex (EVENT::Vertex *sv)
    }
  }
  //----------------------------------------------------------------------------

  TBranch *nhB = tree->GetBranch("EFlowNeutralHadron");
  if( nhB != nullptr ){

    TClonesArray* col = *(TClonesArray**) nhB->GetAddress()  ;
    int nnh = col->GetEntriesFast();

    for(int j = 0; j < nnh ; ++j){

      Tower* p = static_cast<Tower*> (col->At(j));

      auto* pfo = new lcio::ReconstructedParticleImpl ;
      pfos->addElement( pfo ) ;

      recd2lmap.insert( std::make_pair( p->GetUniqueID() , pfo ) ) ;

      GenParticle* mcpd = (GenParticle*)p->Particles.At(0) ;// fixme use only first mc truth particle - can there be more than one ?
      auto it = mcpd2lmap.find( mcpd->GetUniqueID() );
      if( it != mcpd2lmap.end() ){

	mcrecNav.addRelation( it->second, pfo, 1.0 ) ;
	recmcNav.addRelation( pfo, it->second, 1.0 ) ;

      } else {
	std::cout << " ### found no MC truth delphes particle ######### " << mcpd->PID<< "  uid: " <<  mcpd->GetUniqueID()  << std::endl ;
      }

      pfo->setType( 130 ); //fixme K0L?

      double e = p->E ;
      double th = 2.*atan( exp( - p->Eta ) );
      double ph = p->Phi ;
      double m[3] = { e * cos( ph ) * sin( th ) , e * sin( ph ) * sin( th ) ,   e * cos( th ) } ;

      pfo->setEnergy( e ) ;
      pfo->setMomentum( m ) ;
//      pfo->setCovMatrix (const float *cov) ; //fixme
      pfo->setMass( 0. ) ; //fixme ?
      pfo->setCharge(0.) ;

      auto* pid = new lcio::ParticleIDImpl ;
      pid->setPDG( 130 ) ; //fixme K0L?
      pid->setLikelihood( 1. ) ;
      pid->addParameter( p->Eem  / p->E ) ;
      pid->addParameter( p->Ehad / p->E ) ;

      //pfo->setReferencePoint (const float *reference)
      pfo->addParticleID( pid ) ;
      pfo->setParticleIDUsed( pid );

      pfo->setGoodnessOfPID( 1. ) ;

      //pfo->addParticle (EVENT::ReconstructedParticle *particle)
      //pfo->addCluster (EVENT::Cluster *cluster)
      //pfo->Add a cluster that has been used to create this particle.
      //pfo->addTrack (EVENT::Track *track)
      //pfo->Add a track that has been used to create this particle.
      //pfo->setStartVertex (EVENT::Vertex *sv)
    }
  }
  //----------------------------------------------------------------------------------
  TBranch *phB = tree->GetBranch("EFlowPhoton");
  if( phB != nullptr ){

    TClonesArray* col = *(TClonesArray**) phB->GetAddress()  ;
    int nnh = col->GetEntriesFast();

    for(int j = 0; j < nnh ; ++j){

      Tower* p = static_cast<Tower*> (col->At(j));

      auto* pfo = new lcio::ReconstructedParticleImpl ;
      pfos->addElement( pfo ) ;

      recd2lmap.insert( std::make_pair( p->GetUniqueID() , pfo ) ) ;

      GenParticle* mcpd = (GenParticle*)p->Particles.At(0) ;// fixme use only first mc truth particle - can there be more than one ?
      auto it = mcpd2lmap.find( mcpd->GetUniqueID() );
      if( it != mcpd2lmap.end() ){

	mcrecNav.addRelation( it->second, pfo, 1.0 ) ;
	recmcNav.addRelation( pfo, it->second, 1.0 ) ;

      } else {
	std::cout << " ### found no MC truth delphes particle ######### " << mcpd->PID<< "  uid: " <<  mcpd->GetUniqueID()  << std::endl ;
      }

      pfo->setType( 22 );

      double e = p->E ;
      double th = 2.*atan( exp( - p->Eta ) );
      double ph = p->Phi ;
      double m[3] = { e * cos( ph ) * sin( th ) , e * sin( ph ) * sin( th ) ,   e * cos( th ) } ;

      pfo->setEnergy( e ) ;
      pfo->setMomentum( m ) ;
//      pfo->setCovMatrix (const float *cov) ; //fixme
      pfo->setMass( 0. ) ; //fixme ?
      pfo->setCharge(0.) ;

      auto* pid = new lcio::ParticleIDImpl ;
      pid->setPDG( 22 ) ; //fixme K0L?
      pid->setLikelihood( 1. ) ;
      pid->addParameter( p->Eem  / p->E ) ;
      pid->addParameter( p->Ehad / p->E ) ;

      //pfo->setReferencePoint (const float *reference)
      pfo->addParticleID( pid ) ;
      pfo->setParticleIDUsed( pid );

      pfo->setGoodnessOfPID( 1. ) ;

      //pfo->addParticle (EVENT::ReconstructedParticle *particle)
      //pfo->addCluster (EVENT::Cluster *cluster)
      //pfo->Add a cluster that has been used to create this particle.
      //pfo->addTrack (EVENT::Track *track)
      //pfo->Add a track that has been used to create this particle.
      //pfo->setStartVertex (EVENT::Vertex *sv)
    }
  }

//======================================================================

  TBranch *jB = tree->GetBranch("Jet");
  if( jB != nullptr ){

    TClonesArray* col = *(TClonesArray**) jB->GetAddress()  ;
    int nnh = col->GetEntriesFast();

    for(int j = 0; j < nnh ; ++j){

      Jet* jd = static_cast<Jet*> (col->At(j));

      auto* jet = new lcio::ReconstructedParticleImpl ;
      jets->addElement( jet ) ;


      int nc = jd->Constituents.GetEntriesFast() ;

      std::vector<lcio::ReconstructedParticle*> jetPFOs;

      for(int k=0;k<nc;++k){
	int uid = ((Candidate*)jd->Constituents.At(k))->GetUniqueID() ;

	auto it = recd2lmap.find( uid ) ;
	if( it != recd2lmap.end() ){
	  jetPFOs.push_back( it->second ) ;
	}
      }

      if( nc !=  jetPFOs.size() )
	std::cout << "***** WARNING *** jet constituents " << nc << " - pfos " << jetPFOs.size() << std::endl ;

      double eJet(0), pxJet(0), pyJet(0), pzJet(0) ;
      for( auto* pfo : jetPFOs ){

	jet->addParticle( pfo ) ;

	eJet  += pfo->getEnergy() ;
	pxJet += pfo->getMomentum()[0] ;
	pyJet += pfo->getMomentum()[1] ;
	pzJet += pfo->getMomentum()[2] ;
      }
      jet->setEnergy( eJet ) ;
      double pJet[3] = {pxJet, pyJet,pzJet} ;
      jet->setMomentum( pJet ) ;
      jet->setMass( jd->Mass ) ;
//      pfo->setCovMatrix (const float *cov) ; //fixme

      jet->setCharge(0.) ;

      auto* pid = new lcio::ParticleIDImpl ;
      pid->setPDG( 22 ) ; //fixme K0L?
      pid->setLikelihood( 1. ) ;
      pid->addParameter( p->Eem  / p->E ) ;
      pid->addParameter( p->Ehad / p->E ) ;

      //jet->setReferencePoint (const float *reference)
      jet->addParticleID( pid ) ;
      jet->setParticleIDUsed( pid );

      jet->setGoodnessOfPID( 1. ) ;

    }
  }

//======================================================================



  evt->addCollection( mcrecNav.createLCCollection(), "MCTruthRecoLink");
  evt->addCollection( recmcNav.createLCCollection(), "RecoMCTruthLink");

  lcio::LCTOOLS::dumpEvent( evt ) ;

}

//======================================================================
