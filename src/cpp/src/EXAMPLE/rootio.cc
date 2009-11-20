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

//#include "IOIMPL/rootio_templates.h"

#include "LCCol2Branch.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TRefArray.h"
#include "TClassTable.h"


#include <cstdlib>

static const char* FILEN = "simjob.slcio" ; // default file name 
static  char*  RFILEN = "simjob.root" ; // default file name 



using namespace std ;
using namespace lcio ;


struct RecoWrapper{
  EVENT::ReconstructedParticle* RecPart ;
  TRefArray  Tracks ;
} ;



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
  //   create a ROOT tree with ReconstructedParticles ...
  //

  TFile* _file = new TFile( RFILEN , "RECREATE");
  TTree* _tree = new TTree("lciotree", "My Tree");


  typedef std::vector< LCBranch* > BV ;
  BV _branches ;
  _branches.reserve(50) ;
  

  // ---  create branches for lcio collections --------
  //      -> how should we do this in general ? - read the first n events ? ....
  //

  //_branches.push_back( new LCCol2Branch<ReconstructedParticleROOT>( "PandoraPFOs" , _tree ) )  ;

//   _branches.push_back( new LCCol2Branch<EVENT::ReconstructedParticle>( "PandoraPFOs" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::ReconstructedParticle>( "Durham_2Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::ReconstructedParticle>( "Durham_3Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::ReconstructedParticle>( "Durham_4Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::ReconstructedParticle>( "Durham_5Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::ReconstructedParticle>( "Durham_6Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "IPVertex" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::Track>( "LDCTracks" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::MCParticle>( "MCParticlesSkimmed" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::Cluster>( "PandoraClusters" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::LCRelation>( "RecoMCTruthLink" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "V0Vertices" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "ZVRESVertices_2Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "ZVRESVertices_3Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "ZVRESVertices_4Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "ZVRESVertices_5Jets" , _tree ) )  ;
//   _branches.push_back( new LCCol2Branch<EVENT::Vertex>( "ZVRESVertices_6Jets" , _tree ) )  ;

  _branches.push_back( new LCCol2Branch<IMPL::ReconstructedParticleImpl>( "PandoraPFOs" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::ReconstructedParticleImpl>( "Durham_2Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::ReconstructedParticleImpl>( "Durham_3Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::ReconstructedParticleImpl>( "Durham_4Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::ReconstructedParticleImpl>( "Durham_5Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::ReconstructedParticleImpl>( "Durham_6Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "IPVertex" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::TrackImpl>( "LDCTracks" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::MCParticleImpl>( "MCParticlesSkimmed" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::ClusterImpl>( "PandoraClusters" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::LCRelationImpl>( "RecoMCTruthLink" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "V0Vertices" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "ZVRESVertices_2Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "ZVRESVertices_3Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "ZVRESVertices_4Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "ZVRESVertices_5Jets" , _tree ) )  ;
  _branches.push_back( new LCCol2Branch<IMPL::VertexImpl>( "ZVRESVertices_6Jets" , _tree ) )  ;
  

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
    
    nEvents ++ ;
  } 
  // -------- end of event loop -----------
  
  cout << endl <<  "  " <<  nEvents << " events copied from file: " << FILEN 
       << " to " <<  RFILEN  << std::endl  ;


  _file->Write() ;
  delete _file ;
  
  lcReader->close() ;
  delete lcReader ;
  
  for( BV::const_iterator it = _branches.begin() ; it != _branches.end()  ; ++it) {
    delete (*it) ;
  }
  
  return 0 ;
}


