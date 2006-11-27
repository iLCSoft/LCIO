#include "lcio.h"

#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCCollection.h" 
#include "EVENT/Track.h" 
#include "EVENT/Cluster.h" 

//static const char* FILEN = "recjob.slcio" ; // default file name 
static std::vector<std::string> FILEN ; 

using namespace std ;
using namespace lcio ;



struct TrkCluLink : LCNToNRelation<TrkCluLink,Track,Cluster> {} ;

struct Index : LCOwnedLinkTraits<Index,int> {} ;


/** Example/test program for new LCIO runtime relations.
 */

int main(int argc, char** argv ){

  // read file names from command line (only argument) 
  if( argc < 2) {
    cout << " usage:  anajob <input-file1> [[input-file2],...]" << endl ;
    exit(1) ;
  }
  for(int i=1 ; i < argc ; i++){
      FILEN.push_back( argv[i] )  ;
  }
  int nFiles = argc-1 ;
  
  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
  
  cout << " will open and read from files: " << endl ;  
  for(int i=0 ; i < nFiles ; i++){
    cout  << "     "  << FILEN[i] << endl ; 
  }  

  //  loop over the file  and dump event data

  lcReader->open( FILEN ) ;

  LCEvent* evt ;
  int nEvents = 0 ;
  

  //----------- the event loop -----------
  while( (evt = lcReader->readNextEvent()) != 0 && nEvents<10 ) {
    
//     LCTOOLS::dumpEvent( evt ) ;

    const StringVec* names = evt->getCollectionNames() ;

    LCCollection* trkcol = 0  ;
    LCCollection* clucol = 0  ;

    for(unsigned int i=0;i< names->size() ;++i){
      
      LCCollection* col = evt->getCollection( (*names)[i] ) ;
      
      if(  col->getTypeName() == LCIO::TRACK ) 
	trkcol = col ;
      
      if(  col->getTypeName() == LCIO::CLUSTER ) 
	clucol = col ;
    }

    if( trkcol && clucol ) {

      int nclu = clucol->getNumberOfElements() ;
      int ntrk = trkcol->getNumberOfElements() ;


      for(int j=0 ; j< ntrk  ; j++ ){
	
	Track*   trk = dynamic_cast<Track*>   ( trkcol->getElementAt(j) ) ;
	
	trk->ext<Index>() = new int(j) ;
	
	for(int k=0 ; k< nclu ; k++ ){
	  
	  Cluster* clu = dynamic_cast<Cluster*> ( clucol->getElementAt(k) ) ;
	  
 	  if( j == 0 )
	    clu->ext<Index>() = new int(k) ;
	  
  	  add_relation<TrkCluLink>( trk ,clu );
	  
	}

      }
      
      
      // --- now print the relation:

      for(int j=0 ; j< ntrk ; j++ ){

	std::cout << " track " << j << " cluster relations:   " ; 

	for( TrkCluLink::to_traits::const_iterator iclu =  
	       trkcol->getElementAt(j)->to<TrkCluLink>().begin()  ;
	     
	     iclu != trkcol->getElementAt(j)->to<TrkCluLink>().end() ; ++iclu ){
	  
 	  std::cout << *(*iclu)->ext<Index>() << ", " ; 
	}
	std::cout << std::endl ; 
      }


    } else {
      std::cout << " couldn't find Track and Cluster collection in event !" << std::endl ;
    }

    nEvents ++ ;
  } 
  // -------- end of event loop -----------
  
  cout << endl <<  "  " <<  nEvents << " events read from files: " << endl  ;
  for(int i=0 ; i < nFiles ; i++){
    cout  << "     "  << FILEN[i] << endl ; 
  }  

  lcReader->close() ;
  delete lcReader ;
  return 0 ;
}

  
