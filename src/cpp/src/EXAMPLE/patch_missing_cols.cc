#include "lcio.h"

#include "IO/LCReader.h"
#include "IO/LCReader.h"
#include "EVENT/LCCollection.h"
#include "UTIL/CheckCollections.h"


/** Utility to create a fixed file  with empty collections added that are not
 *  present in all events in the input file.
 *  
 *  \author F.Gaede, DESY
 *  \date Dec 2022
 */

int main(int argc, char** argv ){
  
  // read file names from command line (only argument) 
  if( argc != 3) {
    std::cout << " usage:  patch_missing_cols <input-file> <output-file> " << std::endl << std::endl ;
    exit(1) ;
  }

  std::string inFile = argv[1] ;
  std::string outFile = argv[2] ;

  auto* lcWrt = IOIMPL::LCFactory::getInstance()->createLCWriter() ;

  try{ lcWrt->open( outFile , EVENT::LCIO::WRITE_NEW ) ; } 
    
  catch(IO::IOException& e){
    std::cout << "[patch_missing_cols] Can't open file for writing -  " 
	      << e.what()  << std::endl ;
    exit(1) ;
  }

  UTIL::CheckCollections colCheck ;
  
  colCheck.checkFile( inFile ) ;
  
  colCheck.addPatchCollections( colCheck.getMissingCollections() ) ;


  // -- now create copy of file with all collections present ---

  auto* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;

  lcReader->open( inFile ) ;
  
  EVENT::LCEvent* evt = 0 ;
  
  while( (evt = lcReader->readNextEvent()) != nullptr ){

    colCheck.patchCollections( evt ) ;

    lcWrt->writeEvent( evt ) ;
  } 

  lcWrt->close() ;
  
  return 0 ;
}

  
