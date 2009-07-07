#include "lcio.h"

#include "IO/LCReader.h"

#include "IMPL/LCCollectionVec.h"
#include "IMPL/ReconstructedParticleImpl.h"
#include "IMPL/MCParticleImpl.h"
#include "IMPL/VertexImpl.h"
#include "IMPL/TrackImpl.h"
#include "IMPL/ClusterImpl.h"
#include "IMPL/LCRelationImpl.h"
#include "UTIL/LCTypedVector.h"
#include "IMPL/LCEventImpl.h"
// #include "EVENT/LCRunHeader.h" 

#include "lcio_templates.h"

#include "LCCol2Branch.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TClassTable.h"


#include <cstdlib>

static const char* FILEN = "simjob.slcio" ; // default file name 
static  char*  RFILEN = "simjob.root" ; // default file name 

//static  char* PFOCOL="ReconstructedParticle" ;
static  char* PFOCOL="PandoraPFOs" ;


//#define RECOTREE 
//#define EVENTTREE

using namespace std ;
using namespace lcio ;

/** example and test code for writing a root file ...
 */

int main(int argc, char** argv ){



  // read file names from command line (only argument) 
  if( argc != 3) {
    cout << " usage:  rootio input-file root-file" << endl ;
    exit(1) ;
  }

  FILEN = argv[1] ;
  RFILEN = argv[2] ;



  //---------------------------------------------------------------------------------------------
  //  load the LCIO ROOT dictionary
  // 
  //   if (!TClassTable::GetDict("IMPL::ReconstructedParticleImpl")) {
  //     unsigned res ;
  
  //     // fg: don't need to load lcio again !? 
  //     //     res = gSystem->Load("./liblcio.so"); 
  //     //     if (res != 0 ) {
  //     //       std::cout << " can't load ./liblcio.so  !  err = "  << res << std::endl ;
  //     //       exit(1) ;
  //     //     } 
  //     //     std::cout << " successfully loaded LCIO library " << std::endl ;
  
  //     res = gSystem->Load("./liblcioDict.so"); 
  
  //     if (res != 0 ) {
  //       std::cout << " can't load ./liblcioDict.so  !  err = "  << res << std::endl ;
  //       exit(1) ;
  //     } 
  //     std::cout << " successfully loaded LCIO dictionary " << std::endl ;
  //   }
  
  //   if (!TClassTable::GetDict("IMPL::ReconstructedParticleImpl")) {
  //     std::cout  << " unknown dictionary for ReconstructedParticleImpl" << std::endl ;
  //     exit(1) ;
  //   } 
  
  //---------------------------------------------------------------------------------------------
  //   create a ROOT tree with ReconstructedParticles ...
  //

  TFile* _file = new TFile( RFILEN , "RECREATE");
  TTree* _tree = new TTree("lciotree", "My Tree");


  typedef std::vector< LCBranch* > BV ;
  BV _branches ;
  _branches.reserve(50) ;
  
  _branches.push_back( new LCCol2Branch<EVENT::ReconstructedParticle>( "PandoraPFOs" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::ReconstructedParticle>( "Durham_2Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::ReconstructedParticle>( "Durham_3Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::ReconstructedParticle>( "Durham_4Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::ReconstructedParticle>( "Durham_5Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::ReconstructedParticle>( "Durham_6Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "IPVertex" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::Track>( "LDCTracks" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::MCParticle>( "MCParticlesSkimmed" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::Cluster>( "PandoraClusters" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::LCRelation>( "RecoMCTruthLink" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "V0Vertices" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "ZVRESVertices_2Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "ZVRESVertices_3Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "ZVRESVertices_4Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "ZVRESVertices_5Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "ZVRESVertices_6Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::ReconstructedParticleImpl>( "PandoraPFOs" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::ReconstructedParticleImpl>( "Durham_2Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::ReconstructedParticleImpl>( "Durham_3Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::ReconstructedParticleImpl>( "Durham_4Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::ReconstructedParticleImpl>( "Durham_5Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::ReconstructedParticleImpl>( "Durham_6Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "IPVertex" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::TrackImpl>( "LDCTracks" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::MCParticleImpl>( "MCParticlesSkimmed" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::ClusterImpl>( "PandoraClusters" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::LCRelationImpl>( "RecoMCTruthLink" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "V0Vertices" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "ZVRESVertices_2Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "ZVRESVertices_3Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "ZVRESVertices_4Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "ZVRESVertices_5Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "ZVRESVertices_6Jets" , _tree ) )  ;
  

// #ifdef RECOTREE //=======================================================================================
//     IMPL::ReconstructedParticleImpl* _p = new IMPL::ReconstructedParticleImpl() ; 
//     //    _ep = new IMPL::LCEventImpl() ;
    
//     //LCCollectionVec dummy(LCIO::RECONSTRUCTEDPARTICLE) ;
    
//     // UTIL::LCTypedVector<IMPL::ReconstructedParticleImpl>* _pv = new UTIL::LCTypedVector<IMPL::ReconstructedParticleImpl> ;
//     std::vector<IMPL::ReconstructedParticleImpl*>* _pv = new std::vector<IMPL::ReconstructedParticleImpl*> ;
    
//     //LCTypedVectorReconstructedParticleImpl* _pv = new LCTypedVectorReconstructedParticleImpl ;
    
//     //PFOVec* _pv = new PFOVec ;
    
//     //IMPL::LCCollectionVec* _pv = new IMPL::LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE) ;
//     //  _pv = new PFOVec ;
//     //  _np = 0 ;
    
    
//     //  _tree->Branch("recopart", &_p);
//     _tree->Branch("recovec", &_pv, 16000, 2 );
//     //  _tree->Branch("recovecn", &_np);
    
//     //_tree->Branch("event",&_ep); 
    
//     delete _p ;
//     //  delete _pv ;
//     //  delete _ep ;
    
// #endif //#ifdef RECOTREE //=======================================================================================
  

//     //---------------------------------------------------------------------------------------------
//     //   create a ROOT tree with complete LCEvent ...
//     //
// #ifdef EVENTTREE //=======================================================================================
    

//     IMPL::LCEventImpl* _evt  = new IMPL::LCEventImpl ;
    
    
//     _tree->Branch("lcevent", &_evt, 16000, 2 );
    
//     delete _evt ;

// #endif //#ifdef EVENTTREE //=======================================================================================



  //---------------------------------------------------------------------------------------------
  //  open the lcio file
  // 
  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
  lcReader->open( FILEN ) ;
  LCEvent* evt ;
  int nEvents = 0 ;
  



  //---------------------------------------------------------------------------------------------
  //   the event loop 
  while( (evt = lcReader->readNextEvent()) != 0 ) {
    

    for( BV::const_iterator it = _branches.begin() ; it != _branches.end()  ; ++it) {

      (*it)->fill( evt ) ;
    }

    _tree->Fill() ;
    



//     //    LCTOOLS::dumpEvent( evt ) ;

// #ifdef RECOTREE //=======================================================================================
//     try{
      
//       //      LCCollectionVec* col = dynamic_cast<LCCollectionVec*>( evt->getCollection( PFOCOL ) );
//       //      _pv = col ;
      
//       LCCollection* col =  evt->getCollection( PFOCOL ) ;

//       UTIL::LCTypedVector<IMPL::ReconstructedParticleImpl> tv(col) ;

//       //      _pv->assign( tv.begin() , tv.end() ) ;
      
//       _pv =  &tv ;

//       //_pv->v =  tv ;
//       _tree->Fill() ;
      
//     }catch(const DataNotAvailableException& e){
//       std::cout << "  collection not found : " << PFOCOL << std::endl ;
//     }

// #endif //#ifdef RECOTREE //=======================================================================================


// #ifdef EVENTTREE //=======================================================================================

//     _evt = dynamic_cast< IMPL::LCEventImpl* >( evt )  ;

//     _tree->Fill() ;

// #endif //#ifdef EVENTTREE //=======================================================================================


    nEvents ++ ;
  } 
  // -------- end of event loop -----------
  
  cout << endl <<  "  " <<  nEvents << " events copied from file: " << FILEN 
       << " to " <<  RFILEN  << std::endl  ;
  
  lcReader->close() ;
  delete lcReader ;


  _file->Write() ;
  delete _file ;
  //  delete _tree ;

  for( BV::const_iterator it = _branches.begin() ; it != _branches.end()  ; ++it) {
    
    delete (*it) ;
  }


  return 0 ;
}

  
