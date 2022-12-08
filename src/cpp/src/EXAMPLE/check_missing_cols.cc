#include "lcio.h"

#include "MT/LCReader.h"
#include "EVENT/LCCollection.h"

#include <map>

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
  
  
  // open list of files
  lcReader.open( FILEN ) ;

  std::unique_ptr<EVENT::LCEvent> evt ;

  unsigned nEvents = 0 ;
  
  
  std::map<std::string,unsigned> cntMap ;
  std::map<std::string,std::string> typeMap ;

  //----------- the event loop -----------
  while( (evt = lcReader.readNextEventHeader()) != 0 ) {
    
    auto colNames = evt->getCollectionNames() ;

    for(unsigned i=0,n= colNames->size() ; i<n ; ++i){
      
      auto name = colNames->at(i) ;
      
      unsigned cnt = cntMap[ name ] ++ ;

      if( cnt == 0 ){ // enter type only for first occurence
	auto col = evt->getCollection( name ) ;
	typeMap.insert( std::make_pair( name,  col->getTypeName() ) ) ;
      }
    }

    nEvents ++ ;
  } 

  // -------- end of event loop -----------
  
  std::cout << " ================================================================ " << std::endl ;
  std::cout << std::endl <<  "  " <<  nEvents << " events read " << std::endl  ;
  std::cout << "     collections that are not in all events : " << std::endl ;
  std::cout << " ================================================================ " << std::endl ;


  unsigned cntSize = cntMap.size() ;
  unsigned typeSize = typeMap.size() ;

  if( cntSize  != typeSize ) {
    std::cout << " ERROR : different sizes in maps : count: " << cntSize << " types: "
	      << typeSize << std::endl ;
    exit(1) ;
  }

  auto it_c = cntMap.begin();
  auto it_t = typeMap.begin();
  for( ; it_c != cntMap.end() ; it_c++, it_t++ ){

    if( it_c->second != nEvents )
      std::cout << "     " <<  it_c->first << " : " << it_t->second << "  - " << it_c->second << std::endl ;
  }

  std::cout << " ================================================================ " << std::endl ;
  std::cout << "     collections that are in all events : " << std::endl ;
  std::cout << " ================================================================ " << std::endl ;


  it_c = cntMap.begin();
  it_t = typeMap.begin();
  for( ; it_c != cntMap.end() ; it_c++, it_t++ ){

    if( it_c->second == nEvents )
      std::cout << "     " <<  it_c->first << " : " << it_t->second << "  - " << it_c->second << std::endl ;
  }
  std::cout << " ================================================================ " << std::endl ;


  lcReader.close() ;
  return 0 ;
}

  
