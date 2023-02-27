#include "lcio.h"

#include "MT/LCReader.h"
#include "EVENT/LCCollection.h"
#include "UTIL/CheckCollections.h"

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
	  <<       " [ nEvt = "  <<  lcReader.getNumberOfEvents() << " ] "
	  << std::endl ; 
    
    lcReader.close() ;
  }  

  UTIL::CheckCollections colCheck ;
  
  colCheck.checkFiles( FILEN ) ;

  colCheck.print( std::cout ) ;
  

  return 0 ;
}

  
