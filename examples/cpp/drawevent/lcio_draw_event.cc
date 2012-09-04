#include "lcio.h"
#include <stdio.h>

#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCRunHeader.h" 

//#include "EVENT/SimCalorimeterHit.h" 
#include "EVENT/CalorimeterHit.h" 
//#include "EVENT/RawCalorimeterHit.h" 
#include "EVENT/ReconstructedParticle.h"
//#include "UTIL/CellIDDecoder.h"
#include "UTIL/Operators.h"
#include "UTIL/LCIterator.h"

//---- ROOT stuff --------------
#include "TCanvas.h"
#include "TApplication.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoMatrix.h"
//----------------------

#include <cstdlib>

using namespace std ;
using namespace lcio ;

static FILE* _file = 0 ;

//--------------------------------------------------------------------

void draw_particle( ReconstructedParticle* p, int color, const char *opt, 
		    TGeoManager *geom,
		    TGeoMedium  *medSolid,
		    TGeoVolume  *top
		    ) ;

//--------------------------------------------------------------------


/** draw the given event to screen - and write the hits to an ascii file for further visualization...
 */

int main(int argc, char** argv ){


  char* FILEN ;
  int runNumber=0 ;
  int evtNumber=0 ;
  int nthEvent=1 ;

  // read file name from command line (only argument) 
  if( argc < 4 ) {
    
    cout << " usage: lcio_draw_event filename colName runNum evtNum " << endl ;
    cout << "    or: lcio_draw_event filename colName n      " << endl ;
    cout << "  where the first draws the ReconstructedParticles from colllection colName with the specified run and event number" << endl ;
    cout << "  and the second simply draws it for the n-th event in the file" << endl ;

    exit(1) ;
  }
  
  FILEN = argv[1] ;
  std::string colName( argv[2] ) ;

  bool dumpNthEvent( argc == 4 ) ;
 

  if( dumpNthEvent ) {

    nthEvent  = atoi( argv[3] ) ;

    if( nthEvent < 1 ) {

      cout << " usage: lcio_draw_event filename n   -   whith  n > 0 !  " << endl ;
      
      exit(1) ;
    }

  }else{

    runNumber = atoi( argv[3] ) ;
    evtNumber = atoi( argv[4] ) ;
  }
  
  //--------------------------------------------------------------------------------
  LCReader* lcReader = LCFactory::getInstance()->createLCReader(LCReader::directAccess) ;
  
  LCEvent* evt(0) ;

  try{
    
    lcReader->open( FILEN ) ;
    
    if( dumpNthEvent ){
      
      if( nthEvent > 1 )
	lcReader->skipNEvents(  nthEvent - 1 ) ;
      
      evt = lcReader->readNextEvent() ; 
      
    }else{
      
      evt = lcReader->readEvent(runNumber,  evtNumber) ; 
    }
    
    
    if( !evt  ){
      
      if(dumpNthEvent){
	
	cout << " less than " << nthEvent << "  events in  file " << FILEN << endl ;    
	
      }else{
	
	cout << " couldn't find event " << evtNumber << " - run " << runNumber 
	     << " in file " << FILEN << endl ;    
      } 
      
      exit(1) ;
    }
    
    //------------------------------------------------

    TApplication app("lcio_draw_event", &argc, argv, 0, 0);

    _file = fopen( "lcio_draw_event.txt" , "w" ) ;

    
    //------------------------------------------------
    while( evt != 0 ) {  // not really a loop  - single events for now ...
      
      //LCTOOLS::dumpEvent( evt ) ;
      
      LCIterator<ReconstructedParticle> it( evt , colName ) ;
      
      if( ! it() ){
	
	std::cout << " No collection " << colName << " found in event ! " << std::endl ;
	
	exit(1) ;
      }
      
      
      //===============  OGL-Root stuff ==========================================
      
      TGeoManager* geom = new TGeoManager("RootGLViewer", "Display LCIO collections in Root GL");
      geom->SetNsegments(4); // Doesn't matter keep low
      
      TGeoMaterial *matEmptySpace = new TGeoMaterial("EmptySpace", 0, 0, 0);
      TGeoMaterial *matSolid      = new TGeoMaterial("Solid"    , .938, 1., 10000.);
      
      TGeoMedium *medEmptySpace  = new TGeoMedium("Empty", 1, matEmptySpace);
      TGeoMedium  *medSolid      = new TGeoMedium("Solid", 1, matSolid);
      
      TGeoVolume  *top = geom->MakeBox("WORLD", medEmptySpace, 6000., 6000., 12000.);
      geom->SetTopVolume( top );
      
	
      //*************************************
      //  draw the event
      //*************************************
      
      int iPart = 0 ;
      while( ReconstructedParticle* part = it.next() ) {
	
        int color = 50 + iPart  ; // how to select a 'nice' color scheme for ROOT/ogl ?

	draw_particle( part , color , "" , geom, medSolid, top) ;
      }
      //*************************************
      
      std::cout << " *************************************************************  \n"
		<< " *    drawing event : " << evt->getEventNumber() << " run : " << evt->getRunNumber() << "\n"   
		<< " *      [quit in OGL viewer]                                    \n" 
		<< " *************************************************************   " << std::endl ;
     
      
      geom->CloseGeometry();
      top->Draw("ogl") ;
      app.Run() ;
      
	
      //--------- event loop does not seem to work w/ GL viewer !? ---- 
      // std::cout << " --- Next? [yes/no/quit] " <<  std::endl ;
      // static const Int_t kMaxLen = 1024;
      // Char_t temp[kMaxLen];
      // cin.getline(temp,kMaxLen);
	
      // TString opts(temp);
      // opts.ToLower();
	
      // if (!opts.Length()) {
      // 	break ;
      // 	continue;
      // } else if (opts[0] == 'n' || opts[0] == 'q') {
      // 	  break;
      // }	  

      // evt = lcReader->readNextEvent() ;
      
      evt = 0 ; // single event loop ;-(

    } // event loop 
    
    //------------------------------------------------
    
    lcReader->close() ;

    fclose( _file ) ;
    
  }  catch( IOException& e) {
    cout << e.what() << endl ;
    exit(1) ;
  }
  
  return 0 ;
}



//==============================================================================================================

void draw_particle( ReconstructedParticle* part, int color, const char *opt, 
		    TGeoManager *geom,
		    TGeoMedium  *medSolid,
		    TGeoVolume  *top
		    ) {
  
  // const static int TrkStyle = 6 ;
  // const static int CluStyle = 2 ;
  
  // //int color = _colors[ ip % _colors.size() ] ;
  
  // if (!gPad ) return;
  // gPad->cd();
  
  //-----------------------------------
  double energyCut = 0.003 ;  // MeV
  //-----------------------------------

  TrackVec trks   = part->getTracks();
  ClusterVec clus = part->getClusters();
  
  int nTrk = trks.size();
  int nClu = clus.size();
  
  // // count all hits first !
  // int nCluHits(0), nTrkHits(0) ;
  // for (int i=0; i<nClu; nCluHits += clus[i++]->getCalorimeterHits().size() );
  // for (int i=0; i<nTrk; nTrkHits += trks[i++]->getTrackerHits().size() ) ;
  
  // TPolyMarker3D *pmTrk = new TPolyMarker3D( nTrkHits );
  // pmTrk->SetBit( kCanDelete );
  // pmTrk->SetMarkerColor( color );
  // pmTrk->SetMarkerStyle( TrkStyle );
  // TPolyMarker3D *pmClu = new TPolyMarker3D( nCluHits );
  // pmClu->SetBit( kCanDelete );
  // pmClu->SetMarkerColor( color );
  // pmClu->SetMarkerStyle( CluStyle );
  

  std::stringstream ss ;
  ss << "pfo_" << unsigned( part->id() ) ;

  //-------------------------------
  static const float size = 5. ;
  //-------------------------------

  TGeoVolume*  sph = geom->MakeSphere( ss.str().c_str() ,  medSolid,  0., size );
  TGeoVolume*  box = geom->MakeBox( ss.str().c_str() ,  medSolid, size , size, size);
  sph->SetLineColor( color ) ;
  box->SetLineColor( color ) ;



  //  std::cout << " ---- drawing particle: " <<  lcshort( part ) << std::endl ;
  
  int index = 0 ;

  for (int i=0; i<nTrk; ++i ) {
    
    for(int j=0,N=trks[i]->getTrackerHits().size()  ; j<N ; j+=2 ){ // draw every other hit only
      
      TrackerHit * hit = trks[i]->getTrackerHits()[j] ;
      
      float x = hit->getPosition()[0] ;
      float y = hit->getPosition()[1] ;
      float z = hit->getPosition()[2] ;
      
      //      pmTrk->SetPoint( index++ ,  x, y , z  ) ;
      top->AddNode( sph , index++ , new TGeoTranslation( x , y  , z )  ) ;

      if( _file ) 
	fprintf( _file , " %d  %d  %f   %f   %f  \n" , part->id() , 42 , x , y , z ) ;  
      
    }
  }
  for (int i=0; i<nClu; ++i ) {
    
    for(int j=0,N=clus[i]->getCalorimeterHits().size()  ; j<N ;  ++j ){
      
      CalorimeterHit * hit = clus[i]->getCalorimeterHits()[j] ;
      
      float x = hit->getPosition()[0] ;
      float y = hit->getPosition()[1] ;
      float z = hit->getPosition()[2] ;
      
      //      pmClu->SetPoint( index++ ,  x, y , z  ) ;

      if( hit->getEnergy() > energyCut )  
	top->AddNode( box , index++ , new TGeoTranslation( x , y  , z )  ) ;

      if( _file ) 
	fprintf( _file , " %d  %d  %f   %f   %f  \n" , part->id() , 43 , x , y , z ) ;  
    }
  }

  // pmTrk->Draw();
  // pmClu->Draw();
  // gPad->Update();
}


