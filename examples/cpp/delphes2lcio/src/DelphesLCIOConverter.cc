
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
#include "UTIL/PIDHandler.h"
#include "UTIL/LCIterator.h"
#include "UTIL/Operators.h"
#include "UTIL/EventSummary.h"
#include "UTIL/ProcessFlag.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

//-----------------------------------------------------------------
// some helper methods
namespace{

  template <class T>
    unsigned getUID(TObject* obj){
    T* p = static_cast<T*>(obj) ;
    return p->Particle.GetUniqueID() ;
  }
  template <>
    unsigned getUID<Photon>(TObject* obj){
    Photon* p = static_cast<Photon*>(obj) ;
    return p->Particles.At(0)->GetUniqueID() ;
  }

}
//-----------------------------------------------------------------

DelphesLCIOConverter::DelphesLCIOConverter(const char* fileName ) {

  std::string fn(fileName) ;
  
  if( ! fn.empty()){
    _writer = lcio::LCFactory::getInstance()->createLCWriter() ;
    _writer->open( fn , lcio::LCIO::WRITE_NEW ) ;

    _evtSumCol = new lcio::LCCollectionVec( lcio::LCIO::LCGENERICOBJECT ) ;
  }

  _cfg = new DelphesLCIOConfig ;

}
//-----------------------------------------------------------------

DelphesLCIOConverter::~DelphesLCIOConverter(){

  if( _evtSumCol ){

    auto evt = new lcio::LCEventImpl ;
    evt->setRunNumber( -99 ) ;
    evt->setEventNumber( -99 ) ;
    evt->setWeight( 0. ) ;

    evt->addCollection( _evtSumCol, "EventSummaries" ) ;

    _writer->writeEvent( evt ) ;
  }

  if( _writer ){

    _writer->close() ;

    delete _writer ;
  }
}
//-----------------------------------------------------------------

void DelphesLCIOConverter::readConfigFile( const char* fileName ){

  _cfg->readConfigFile( fileName ) ;

  // read optional event parameter maps
  _evtParF = _cfg->getFloatMap("EventParametersFloat") ;
  _evtParI = _cfg->getIntMap("EventParametersInt") ;
  _evtParS = _cfg->getStringMap("EventParametersString") ;
}

//-----------------------------------------------------------------

void DelphesLCIOConverter::writeEvent(TTree* tree){

  auto* evt = new lcio::LCEventImpl ;

  convertTree2LCIO( tree, evt ) ;
  
  if( _writer )
    _writer->writeEvent( evt ) ;

  delete evt;
}
//-----------------------------------------------------------------

void DelphesLCIOConverter::convertTree2LCIO( TTree *tree , lcio::LCEventImpl* evt){


  //int nEntry =  tree->GetEntries() ;

  static bool first_event = true ;

  if( first_event ) {
    first_event = false ;
    size_t n = tree->GetListOfBranches()->GetEntries();
    for( size_t i = 0; i < n; ++ i ) {
      TBranch *br = dynamic_cast<TBranch*>(tree->GetListOfBranches()->At(i));
      if( !strcmp(br->GetClassName(),"TClonesArray") )
	std::cout << " *************** branch \"" << br->GetName() << " --- " << br->GetClassName()
		  << " - " <<   (*(TClonesArray**) br->GetAddress())->GetEntries()
		  << std::endl;
    }

    std::cout << _cfg->toString() << std::endl ;
  }

  // -------------------------------------------- 

  _mcpd2lmap.clear();
  _recd2lmap.clear();

  // ============= create the mandatory LCIO collections ===============================

  auto* mcps = new lcio::LCCollectionVec( lcio::LCIO::MCPARTICLE )  ;
  evt->addCollection( mcps, _cfg->getMCPParameter("lcioName") ) ;

  auto* pfos = new lcio::LCCollectionVec( lcio::LCIO::RECONSTRUCTEDPARTICLE )  ;
  evt->addCollection( pfos, _cfg->getPFOParameter("lcioName") ) ;

  auto* jets = new lcio::LCCollectionVec( lcio::LCIO::RECONSTRUCTEDPARTICLE )  ;
  evt->addCollection( jets,  _cfg->getJetParameter("lcioName") ) ;

  auto* muons = new lcio::LCCollectionVec( lcio::LCIO::RECONSTRUCTEDPARTICLE )  ;
  muons->setSubset(true) ;
  evt->addCollection( muons,  _cfg->getMuonParameter("lcioName") ) ;

  auto* electrons = new lcio::LCCollectionVec( lcio::LCIO::RECONSTRUCTEDPARTICLE )  ;
  electrons->setSubset(true) ;
  evt->addCollection( electrons,  _cfg->getElectronParameter("lcioName") ) ;

  auto* photons = new lcio::LCCollectionVec( lcio::LCIO::RECONSTRUCTEDPARTICLE )  ;
  photons->setSubset(true) ;
  evt->addCollection( photons, _cfg->getPhotonParameter("lcioName") ) ;

  lcio::PIDHandler pfopidH( pfos );

  int showerParamID = pfopidH.addAlgorithm( "ShowerParameters" , {"emFraction","hadFraction"} ) ;
  int trackParamID = pfopidH.addAlgorithm( "TrackParameters" , {"L","PT","D0","DZ","Phi","CtgTheta","ErrorPT","ErrorD0",
								"ErrorDZ","ErrorPhi","ErrorCtgTheta"} ) ;


  lcio::LCRelationNavigator recmcNav( lcio::LCIO::RECONSTRUCTEDPARTICLE , lcio::LCIO::MCPARTICLE  ) ;
  lcio::LCRelationNavigator mcrecNav( lcio::LCIO::MCPARTICLE , lcio::LCIO::RECONSTRUCTEDPARTICLE ) ;


  if( _evtSumCol ){  // add an element to event summary collection

    _evtSumCol->addElement( new UTIL::EventSummary  );
  }
  
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

    evt->parameters().setValue("crossSection",  e->CrossSection ) ;
    evt->parameters().setValue("ProcessID",     e->ProcessID  ) ;


    // set event meta data as given in the configuration file
    for( auto p : _evtParF ){
      evt->parameters().setValue( p.first , (float ) p.second ) ;
    }
    for( auto p : _evtParI ){
      evt->parameters().setValue( p.first , (int) p.second ) ;
    }
    for( auto p : _evtParS ){
      evt->parameters().setValue( p.first , p.second ) ;
    }




  }

  //=====================================================================

  // convert the MCParticle branch

  TBranch *gpB = tree->GetBranch( _cfg->getMCPParameter("branchName").c_str() );

  if( gpB != nullptr ){

    TClonesArray* col = *(TClonesArray**) gpB->GetAddress()  ;

    int ngp = col->GetEntries();

    for(int j = 0; j < ngp ; ++j){
      GenParticle* p = (GenParticle*) col->At(j);

      auto* mcp = new lcio::MCParticleImpl ;
      mcps->addElement( mcp ) ;

      _mcpd2lmap.insert( std::make_pair( p->GetUniqueID() , mcp ) ) ;
      
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

  // convert the charged PFO branch


  TBranch *trB = tree->GetBranch( _cfg->getPFOParameter("branchNameCharged").c_str()  );

  int pdgCh = _cfg->toInt( _cfg->getPFOParameter("pdgCharged") ) ;

  float massCh = _cfg->toFloat( _cfg->getPFOParameter("massCharged") ) ;

  int chPFONum(0), neuPFONum(0) ;
  
  if( trB != nullptr ){

    TClonesArray* col = *(TClonesArray**) trB->GetAddress()  ;
    int nnh = col->GetEntries();

    for(int j = 0; j < nnh ; ++j){

      Track* trk = static_cast<Track*> (col->At(j));

      auto* pfo = new lcio::ReconstructedParticleImpl ;
      pfos->addElement( pfo ) ;
      ++chPFONum;
      
      _recd2lmap.insert( std::make_pair( trk->GetUniqueID() , pfo ) ) ;

      GenParticle* mcpd = (GenParticle*)trk->Particle.GetObject()  ;
      auto it = _mcpd2lmap.find( mcpd->GetUniqueID() );
      if( it != _mcpd2lmap.end() ){

	mcrecNav.addRelation( it->second, pfo, 1.0 ) ;
	recmcNav.addRelation( pfo, it->second, 1.0 ) ;
      } else {
	std::cout << " ### found no MC truth delphes particle ######### " << mcpd->PID<< "  uid: " <<  mcpd->GetUniqueID()  << std::endl ;
      }

      pfo->setType( pdgCh * trk->Charge );

      pfo->setMass( massCh ) ;

      double p = trk->P ;
      double e = sqrt( p*p + massCh*massCh ) ;
      double th = 2.*atan( exp( - trk->Eta ) );
      double ph = trk->Phi ;
      double m[3] = { p * cos( ph ) * sin( th ) , p * sin( ph ) * sin( th ) ,   p * cos( th ) } ;

      pfo->setEnergy( e ) ;
      pfo->setMomentum( m ) ;
//      pfo->setCovMatrix (const float *cov) ; //fixme
      pfo->setCharge( trk->Charge ) ;

      // auto* pid = new lcio::ParticleIDImpl ;
      // pid->setPDG( trk->PID ) ;   // write the PID from Delphes here
      // pid->setLikelihood( 1. ) ;
      // pfo->addParticleID( pid ) ;
      // pfo->setParticleIDUsed( pid );

      pfopidH.setParticleID( pfo, 0, trk->PID , 1. , trackParamID, { trk->L,trk->PT,trk->D0,trk->DZ,trk->Phi,trk->CtgTheta,
								     trk->ErrorPT, trk->ErrorD0,trk->ErrorDZ,trk->ErrorPhi,
								     trk->ErrorCtgTheta  } ) ;
      pfopidH.setParticleIDUsed( pfo, trackParamID );


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

  TBranch *nhB = tree->GetBranch( _cfg->getPFOParameter("branchNameNHadron").c_str() );

  int pdgNH = _cfg->toInt( _cfg->getPFOParameter("pdgNHadron") ) ;

  float massNH = _cfg->toFloat( _cfg->getPFOParameter("massNHadron") ) ;

  if( nhB != nullptr ){

    TClonesArray* col = *(TClonesArray**) nhB->GetAddress()  ;
    int nnh = col->GetEntries();

    for(int j = 0; j < nnh ; ++j){

      Tower* p = static_cast<Tower*> (col->At(j));

      auto* pfo = new lcio::ReconstructedParticleImpl ;
      pfos->addElement( pfo ) ;
      ++neuPFONum ;
      
      _recd2lmap.insert( std::make_pair( p->GetUniqueID() , pfo ) ) ;

      GenParticle* mcpd = (GenParticle*)p->Particles.At(0) ;

      auto it = _mcpd2lmap.find( mcpd->GetUniqueID() );
      if( it != _mcpd2lmap.end() ){

	mcrecNav.addRelation( it->second, pfo, 1.0 ) ;
	recmcNav.addRelation( pfo, it->second, 1.0 ) ;

      } else {
	std::cout << " ### found no MC truth delphes particle ######### "
		  << mcpd->PID<< "  uid: " <<  mcpd->GetUniqueID()  << std::endl ;
      }

      pfo->setType( pdgNH );

      double e = p->E ;
      double th = 2.*atan( exp( - p->Eta ) );
      double ph = p->Phi ;
      double pp = e ; // sqrt( e * e - massNH * massNH ) ;
      // create a massless 4-vector to avoid nan's

      double m[3] = { pp * cos( ph ) * sin( th ) , pp * sin( ph ) * sin( th ) , pp * cos( th ) } ;

      pfo->setEnergy( e ) ;
      pfo->setMomentum( m ) ;
//      pfo->setCovMatrix (const float *cov) ; //fixme
      pfo->setMass( massNH ) ;
      pfo->setCharge(0.) ;

      pfopidH.setParticleID( pfo, 0, pdgNH , 1. , showerParamID, { p->Eem  / p->E , p->Ehad / p->E   } ) ;
      pfopidH.setParticleIDUsed( pfo, showerParamID );

      pfo->setGoodnessOfPID( 1. ) ;

    }
  }
  //----------------------------------------------------------------------------------

  TBranch *efphB = tree->GetBranch(  _cfg->getPFOParameter("branchNamePhoton").c_str() );

  if( efphB != nullptr ){

    TClonesArray* col = *(TClonesArray**) efphB->GetAddress()  ;
    int nnh = col->GetEntries();

    for(int j = 0; j < nnh ; ++j){

      Tower* p = static_cast<Tower*> (col->At(j));

      auto* pfo = new lcio::ReconstructedParticleImpl ;
      pfos->addElement( pfo ) ;
      ++neuPFONum ;

      _recd2lmap.insert( std::make_pair( p->GetUniqueID() , pfo ) ) ;

      GenParticle* mcpd = (GenParticle*)p->Particles.At(0) ;// fixme use only first mc truth particle - can there be more than one ?
      auto it = _mcpd2lmap.find( mcpd->GetUniqueID() );
      if( it != _mcpd2lmap.end() ){

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
      pfo->setMass( 0. ) ;
      pfo->setCharge(0.) ;

      // auto* pid = new lcio::ParticleIDImpl ;
      // pid->setPDG( 22 ) ; //fixme K0L?
      // pid->setLikelihood( 1. ) ;
      // pid->addParameter( p->Eem  / p->E ) ;
      // pid->addParameter( p->Ehad / p->E ) ;
      // pfo->addParticleID( pid ) ;
      // pfo->setParticleIDUsed( pid );

      pfopidH.setParticleID( pfo, 0,  22 , 1. , showerParamID, { p->Eem  / p->E , p->Ehad / p->E   } ) ;
      pfopidH.setParticleIDUsed( pfo, showerParamID );

      pfo->setGoodnessOfPID( 1. ) ;

      //pfo->setReferencePoint (const float *reference)

      //pfo->addParticle (EVENT::ReconstructedParticle *particle)
      //pfo->addCluster (EVENT::Cluster *cluster)
      //pfo->Add a cluster that has been used to create this particle.
      //pfo->addTrack (EVENT::Track *track)
      //pfo->Add a track that has been used to create this particle.
      //pfo->setStartVertex (EVENT::Vertex *sv)
    }
  }


//======================================================================
  TBranch *br ;
  int pdg = -99 ;

//======================================================================

  // ----  convert default jet branch

  br = tree->GetBranch( _cfg->getJetParameter("branchName").c_str() ) ;

  int useDelphes4Vec = _cfg->toInt( _cfg->getJetParameter("useDelphes4Vec") ) ;

  if( br != nullptr ){

    TClonesArray* tca = *(TClonesArray**) br->GetAddress()  ;

    convertJetCollection( tca, jets , useDelphes4Vec , 0 ) ;
  }

  // ----  convert extra jet collections if requested:

  const auto& ejnames = _cfg->getExtraJetMapNames() ;

  EVENT::LCCollection* yflipCol = nullptr ;

  for( auto& jName : ejnames ){

    auto* jcol = new lcio::LCCollectionVec( lcio::LCIO::RECONSTRUCTEDPARTICLE )  ;

    evt->addCollection( jcol,  _cfg->getMapParameter("lcioName", jName ) ) ;

    br = tree->GetBranch( _cfg->getMapParameter("branchName", jName ).c_str() ) ;

    useDelphes4Vec = _cfg->toInt( _cfg->getMapParameter("useDelphes4Vec", jName ) ) ;

    if( br != nullptr ){

      TClonesArray* tca = *(TClonesArray**) br->GetAddress()  ;

      int storeYMerge = _cfg->toInt( _cfg->getMapParameter("storeYMerge", jName ) ) ;

      if( storeYMerge > 1 ) // request to store these y flip values in event summary
	yflipCol = jcol ;

      convertJetCollection( tca, jcol , useDelphes4Vec , storeYMerge ) ;
    }
  }
//======================================================================
  // ----  convert extra pfo collections if requested:

  const auto& epnames = _cfg->getExtraPFOMapNames() ;

  for( auto& pName : epnames ){

    auto* pcol = new lcio::LCCollectionVec( lcio::LCIO::RECONSTRUCTEDPARTICLE )  ;

    evt->addCollection( pcol,  _cfg->getMapParameter("lcioName", pName ) ) ;

    br = tree->GetBranch( _cfg->getMapParameter("branchName", pName ).c_str() ) ;

    int pdg = _cfg->toInt( _cfg->getMapParameter("pdg", pName ) ) ;

    int isCharged = _cfg->toInt( _cfg->getMapParameter("isCharged", pName ) ) ;

    double mass = _cfg->toFloat( _cfg->getMapParameter("mass", pName ) ) ;

    if( br != nullptr ){

      TClonesArray* tca = *(TClonesArray**) br->GetAddress()  ;

//      std::cout << " convertExtraPFOsCharged(" << pName <<", "  << pdg << ", " << mass << ")" << std::endl ;

      if( isCharged )
	convertExtraPFOsCharged( tca, pcol , pdg, mass ) ;
      else
	convertExtraPFOsNeutral( tca, pcol , pdg, mass ) ;
    }
  }
//======================================================================

  br = tree->GetBranch( _cfg->getPhotonParameter("branchName").c_str() ) ;

  pdg = _cfg->toInt( _cfg->getPhotonParameter("pdg") ) ;

  if( br != nullptr ){

    TClonesArray* tca = *(TClonesArray**) br->GetAddress()  ;

    convertPFORefCollection( tca, photons, mcrecNav, ::getUID<Photon>, pdg );
  }

//======================================================================
  br = tree->GetBranch( _cfg->getElectronParameter("branchName").c_str() );

  pdg = _cfg->toInt( _cfg->getElectronParameter("pdg") ) ;

  if( br != nullptr ){

    TClonesArray* tca = *(TClonesArray**) br->GetAddress()  ;

    convertPFORefCollection( tca, electrons, mcrecNav, ::getUID<Electron>, pdg );
  }

//======================================================================
  br = tree->GetBranch(_cfg->getMuonParameter("branchName").c_str() );

  pdg = _cfg->toInt( _cfg->getMuonParameter("pdg") ) ;

  if( br != nullptr ){

    TClonesArray* tca = *(TClonesArray**) br->GetAddress()  ;

    convertPFORefCollection( tca, muons, mcrecNav, ::getUID<Muon>, pdg );
  }

//======================================================================

  if( _evtSumCol ){  // add to event summary collection

    using namespace UTIL;

    auto evts = new EventSummary( _evtSumCol->getElementAt( _evtSumCol->getNumberOfElements() - 1 ) )  ;

    evts->setI( ESI::runnum, evt->getRunNumber() ) ;
    evts->setI( ESI::evtnum, evt->getEventNumber() ) ;

    evts->setI( ESI::chpfonum,  chPFONum );
    evts->setI( ESI::neupfonum, neuPFONum );
    evts->setI( ESI::munum,  muons->getNumberOfElements() );
    evts->setI( ESI::phonum, photons->getNumberOfElements() );
    evts->setI( ESI::elnum,  electrons->getNumberOfElements() );
    evts->setI( ESI::jetnum, jets->getNumberOfElements()  );


    if( yflipCol != nullptr ){

      EVENT::FloatVec yflip ;
      yflipCol->getParameters().getFloatVals("ExclYflip12_78", yflip ) ;

      if( yflip.size() == 7 ){

	evts->setF( ESF::y12, yflip[0] ) ;
	evts->setF( ESF::y23, yflip[1] ) ;
	evts->setF( ESF::y34, yflip[2] ) ;
	evts->setF( ESF::y45, yflip[3] ) ;
	evts->setF( ESF::y56, yflip[4] ) ;
	evts->setF( ESF::y67, yflip[5] ) ;
	evts->setF( ESF::y78, yflip[6] ) ;
      }
    }

    // total reconstructed energy
    double epfoTot(0), pfopx(0), pfopy(0), pfopz(0)  ;

    lcio::LCIterator<lcio::ReconstructedParticle> pfoIT( pfos ) ;

    while( auto p = pfoIT.next() ){

      epfoTot += p->getEnergy() ;
      pfopx += p->getMomentum()[0] ;
      pfopy += p->getMomentum()[1] ;
      pfopz += p->getMomentum()[2] ;
    }

    // total true and visible energy
    double emcpVis = 0;
    double emcpTot(0), mcppx(0), mcppy(0), mcppz(0)  ;

    lcio::LCIterator<lcio::MCParticle> mcpIT( mcps ) ;

    while( auto mcp = mcpIT.next() ){

      // count only stable Particles
      if( mcp->getGeneratorStatus() != 1 ) continue ;

      emcpTot += mcp->getEnergy() ;
      mcppx   += mcp->getMomentum()[0] ;
      mcppy   += mcp->getMomentum()[1] ;
      mcppz   += mcp->getMomentum()[2] ;


      int pdg = abs( mcp->getPDG() ) ;

      // exclude neutrinos
      if( pdg == 12 ) continue ;
      if( pdg == 14 ) continue ;
      if( pdg == 16 ) continue ;

      emcpVis += mcp->getEnergy() ;
    }


    evts->setF( ESF::epfotot, epfoTot ) ;
    evts->setF( ESF::emcptot, emcpVis ) ;

    // missing 4-momentum
    evts->setF( ESF::emiss,  emcpTot - epfoTot ) ;
    evts->setF( ESF::pxmiss, mcppx - pfopx ) ;
    evts->setF( ESF::pymiss, mcppy - pfopy ) ;
    evts->setF( ESF::pzmiss, mcppz - pfopz ) ;


#if LCIO_VERSION_GE( 2 , 15 )
    const int maxParticles = 10 ; // defined in ProcessFlag.h
    int nMCPCheck = maxParticles > mcps->getNumberOfElements() ? mcps->getNumberOfElements()  : maxParticles ;
    ProcessFlag pFlag = decodeMCTruthProcess( mcps, nMCPCheck ) ;
    evts->setI( ESI::mcproc,  pFlag ) ;
//    std::cout << " ---- mc truth process : " << pFlag << std::endl ;
#endif

  }



//======================================================================


  evt->addCollection( mcrecNav.createLCCollection(), "MCTruthRecoLink");
  evt->addCollection( recmcNav.createLCCollection(), "RecoMCTruthLink");

//  lcio::LCTOOLS::dumpEvent( evt ) ;

}

//======================================================================

bool DelphesLCIOConverter::convertPFORefCollection(TClonesArray* tca, EVENT::LCCollection* col,
						   UTIL::LCRelationNavigator& mcrecNav,
						   std::function<unsigned(TObject*)> uid,
						   int pdg) {
  bool success = true ;

  int n = tca->GetEntries();

  for(int j = 0; j < n ; ++j){

    unsigned puid = uid( tca->At(j) ) ;

    auto it = _mcpd2lmap.find( puid );

    if( it != _mcpd2lmap.end() ){

      auto* mcp = it->second ;
      auto& objs = mcrecNav.getRelatedToObjects( mcp ) ;

      if( !objs.empty() ){

	auto pfo = static_cast<lcio::ReconstructedParticleImpl*>(objs[0]) ;

	if( pdg != -99 ){ // set the correct pid code
	  int newPDG = pfo->getCharge() * pdg ;
	  pfo->setType( newPDG ) ;
	}

	col->addElement( pfo ) ;

      } else {
	std::cout << " ### found no ReconstructedParticle #########  for mcp with pdg: " <<  mcp->getPDG()  << std::endl ;
	success = false ;

      }

    } else {

      std::cout << " ### found no MC truth delphes particle #########  for type  with uid: " <<  puid  << std::endl ;
      success = false ;
    }
  }
  return success ;
}

//======================================================================

bool DelphesLCIOConverter::convertJetCollection(TClonesArray* tca, EVENT::LCCollection* col, int useDelphes4Vec, int storeYMerge ) {

  bool success = true ;

  lcio::PIDHandler jetpidH( col );
  int jetParamID = jetpidH.addAlgorithm( "JetParameters" , {"Flavor","FlavorAlgo","FlavorPhys","BTag",
							    "BTagAlgo","BTagPhys",
							    "TauTag","Charge","EhadOverEem"} ) ;
  int n = tca->GetEntries();

  for(int j = 0; j < n ; ++j){

    Jet* jd = static_cast<Jet*> (tca->At(j));

    if( storeYMerge && (j==0) ) { // store the ymerge values for the first jet in this collection

      std::vector<float> yflip = { 0.f, (float)jd->ExclYmerge23, (float)jd->ExclYmerge34,
				        (float)jd->ExclYmerge45, (float)jd->ExclYmerge56 , 0.f, 0.f } ;

      col->parameters().setValues("ExclYflip12_78", yflip) ;
    }

    auto* jet = new lcio::ReconstructedParticleImpl ;

    col->addElement( jet ) ;

    int nc = jd->Constituents.GetEntries() ;

    std::vector<lcio::ReconstructedParticle*> jetPFOs;

    for(int k=0;k<nc;++k){

      int uid = ((Candidate*)jd->Constituents.At(k))->GetUniqueID() ;

      auto it = _recd2lmap.find( uid ) ;

      if( it != _recd2lmap.end() ){

	jetPFOs.push_back( it->second ) ;

      } else {

        std::cout << "***** WARNING *** jet constituent has no pfo created for pid = "
		  << ((Candidate*)jd->Constituents.At(k))->PID
		  << std::endl ;

	success = false ;
      }

    }

    if( nc !=  jetPFOs.size() ) {
      std::cout << "***** WARNING *** jet constituents " << nc << " - pfos " << jetPFOs.size() << std::endl ;

      success = false ;
    }

    if( ! useDelphes4Vec ){ //--------  jet 4-vector from constituents

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

    } else {  //--- jet 4-vector from Delphes

      double pt = jd->PT ;
      double ph = jd->Phi ;
      double jm = jd->Mass ;

      double th = 2.*atan( exp( - jd->Eta ) );
      double jp = pt / sin(th) ;

      double jp3[3] = { pt * cos( ph )  , pt * sin( ph ) ,   pt / tan( th ) } ;
      double je  = sqrt( jp*jp + jm * jm ) ;

      jet->setEnergy( je ) ;

      jet->setMomentum( jp3 ) ;
    }

    jet->setMass( jd->Mass ) ;
//      pfo->setCovMatrix (const float *cov) ; //fixme

    jet->setCharge(0.) ;

    jetpidH.setParticleID( jet, 0, 0 , 1. , jetParamID,  { (float)jd->Flavor, (float)jd->FlavorAlgo, (float)jd->FlavorPhys,
							   (float)jd->BTag, (float)jd->BTagAlgo, (float)jd->BTagPhys, (float)jd->TauTag,
							   (float)jd->Charge, jd->EhadOverEem} );
    jetpidH.setParticleIDUsed( jet, jetParamID );

    // jet->setReferencePoint (const float *reference)
    // jet->addParticleID( pid ) ;
    // jet->setParticleIDUsed( pid );

    jet->setGoodnessOfPID( 1. ) ;

  }

  return success ;
}

//======================================================================

int DelphesLCIOConverter::convertExtraPFOsCharged(  TClonesArray* tca, EVENT::LCCollection* pfos,
						    int pdgCh, double massCh ) {

  int chPFONum = 0;

  lcio::PIDHandler pfopidH( pfos );
  int trackParamID = pfopidH.addAlgorithm( "TrackParameters" , {"L","PT","D0","DZ","Phi","CtgTheta","ErrorPT","ErrorD0",
								"ErrorDZ","ErrorPhi","ErrorCtgTheta"} ) ;

  int nnh = tca->GetEntries();

  for(int j = 0; j < nnh ; ++j){
    Track* trk = static_cast<Track*> (tca->At(j));

    auto* pfo = new lcio::ReconstructedParticleImpl ;
    pfos->addElement( pfo ) ;
    ++chPFONum;

    pfo->setType( pdgCh * trk->Charge );

    pfo->setMass( massCh ) ;

    double p = trk->P ;
    double e = sqrt( p*p + massCh*massCh ) ;
    double th = 2.*atan( exp( - trk->Eta ) );
    double ph = trk->Phi ;
    double m[3] = { p * cos( ph ) * sin( th ) , p * sin( ph ) * sin( th ) ,   p * cos( th ) } ;

    pfo->setEnergy( e ) ;
    pfo->setMomentum( m ) ;
//      pfo->setCovMatrix (const float *cov) ; //fixme
    pfo->setCharge( trk->Charge ) ;

    pfopidH.setParticleID( pfo, 0, trk->PID , 1.,
			   trackParamID, { trk->L,trk->PT,trk->D0,trk->DZ,trk->Phi,trk->CtgTheta,
					   trk->ErrorPT, trk->ErrorD0,trk->ErrorDZ,trk->ErrorPhi,
					   trk->ErrorCtgTheta  } ) ;

    pfopidH.setParticleIDUsed( pfo, trackParamID );

    pfo->setGoodnessOfPID( 1. ) ;

    float ref[3] = { trk->X,  trk->Y,  trk->Z  } ;
    pfo->setReferencePoint ( ref ) ;

  }
  return chPFONum ;
}
//======================================================================
int DelphesLCIOConverter::convertExtraPFOsNeutral(  TClonesArray* tca, EVENT::LCCollection* pfos,
						    int pdgNH, double massNH ) {

  int neuPFONum = 0;

  lcio::PIDHandler pfopidH( pfos );
  int showerParamID = pfopidH.addAlgorithm( "ShowerParameters" , {"emFraction","hadFraction"} ) ;

  int nnh = tca->GetEntries();

  for(int j = 0; j < nnh ; ++j){

    Tower* p = static_cast<Tower*> (tca->At(j));

    auto* pfo = new lcio::ReconstructedParticleImpl ;
    pfos->addElement( pfo ) ;
    ++neuPFONum ;

    pfo->setType( pdgNH );

    double e = p->E ;
    double th = 2.*atan( exp( - p->Eta ) );
    double ph = p->Phi ;
    double pp = e ; // sqrt( e * e - massNH * massNH ) ;
    // create a massless 4-vector to avoid nan's

    double m[3] = { pp * cos( ph ) * sin( th ) , pp * sin( ph ) * sin( th ) , pp * cos( th ) } ;

    pfo->setEnergy( e ) ;
    pfo->setMomentum( m ) ;
    //pfo->setCovMatrix (const float *cov) ; //fixme
    pfo->setMass( massNH ) ;
    pfo->setCharge(0.) ;

    pfopidH.setParticleID( pfo, 0, pdgNH , 1. , showerParamID, { p->Eem  / p->E , p->Ehad / p->E   } ) ;
    pfopidH.setParticleIDUsed( pfo, showerParamID );

    pfo->setGoodnessOfPID( 1. ) ;
  }


  return neuPFONum ;
}
//======================================================================
