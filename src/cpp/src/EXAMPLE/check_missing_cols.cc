#include "lcio.h"

#include "MT/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCCollection.h"

#include <cstdlib>
#include <sstream>
#include <set>

static std::vector<std::string> FILEN ; 


/** Utility to check files for inconsistent sets of collections, i.e 
 *  collections that are not present in every event.
 *  
 */

int main(int argc, char** argv ){

  // read file names from command line (only argument) 
  if( argc < 2) {
    std::cout << " usage:  check_missing_cols <input-file1> [[input-file2],...]" << std::endl << std::endl ;
    exit(1) ;
  }
  for(int i=1 ; i < argc ; i++){
      FILEN.push_back( argv[i] )  ;
  }
  int nFiles = argc-1 ;
  
  MT::LCReader lcReader(0) ; 
  
  std::cout << "patch_events:  will open and read from files: " << std::endl ;  

  for(int i=0 ; i < nFiles ; i++){

    lcReader.open( FILEN[i] ) ;
    
    std::cout  << std::endl <<  "     "  << FILEN[i] 
	  <<       ", number of events: "  <<  lcReader.getNumberOfEvents() << " ] "   
	  << std::endl ; 
    
    lcReader.close() ;
  }  
  
  
  // open list of files
  lcReader.open( FILEN ) ;

  std::unique_ptr<EVENT::LCEvent> evt ;

  unsigned nEvents = 0 ;
  
  bool firstEvent = true ;
  
  std::set<std::pair<std::string,std::string>> minSet ;
  std::set<std::pair<std::string,std::string>> maxSet ;

  //----------- the event loop -----------
  while( (evt = lcReader.readNextEventHeader()) != 0 ) {
    
//    std::cout << " ========================== event: " << nEvents << std::endl ;

    std::set<std::pair<std::string,std::string>> thisSet ;

    // ----
    auto colNames = evt->getCollectionNames() ;

    for(unsigned i=0,n= colNames->size() ; i<n ; ++i){
      
      auto name = colNames->at(i) ;
      auto col = evt->getCollection( name ) ;

      thisSet.emplace( std::make_pair( name,  col->getTypeName() ) ) ;
    }

    if( firstEvent ){ // start minSet w/ first event

      firstEvent = false;
      minSet.insert( thisSet.begin() , thisSet.end() )  ;

    } else {

      // remove all elements from minSet that are not in this event
      
      for (auto it = minSet.begin(); it != minSet.end(); ) {
	if( thisSet.find( *it ) == thisSet.end() ) {
	  it = minSet.erase( it ) ;
	} else {
	  ++it ;
	}
      }
    }
    
    // ---- merge everything (new) into maxSet
    maxSet.merge( thisSet ) ;
    

    nEvents ++ ;
  } 
  // -------- end of event loop -----------
  
  std::cout << std::endl <<  "  " <<  nEvents << " events read " << std::endl  ;

  std::cout << "     collections that are not in all events : " << std::endl ;
  std::cout << " ================================================================ " << std::endl ;

  for(auto p : maxSet ) {
    if( minSet.find( p ) == minSet.end() )  
      std::cout << "     " <<  p.first << " : " << p.second << std::endl ;
  }
  std::cout << " ================================================================ " << std::endl ;


  lcReader.close() ;
  return 0 ;
}

  
