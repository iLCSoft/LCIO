#include "lcio.h"

#include "IO/LCWriter.h"
//#include "EVENT/LCIO.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace lcio ;

static std::vector<std::string> FILEN ; 


/** Simple program that opens existing LCIO files and appends the records needed for direct access - if they don't exist.
 */

int main(int argc, char** argv ){
  
    
    // create sio writer
    LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter()  ;

    // read file names from command line (only argument) 
    if( argc < 2) {
      std::cout << " usage:  addRandomAccess <input-file1> [[input-file2],...]" << std::endl ;
      exit(1) ;
    }
    for(int i=1 ; i < argc ; i++){
      FILEN.push_back( argv[i] )  ;
    }

    int nFiles = argc-1 ;

    for( int i=0 ; i <nFiles ; ++i ) {

      try{
      
	lcWrt->open( FILEN[i] , LCIO::WRITE_APPEND )  ;

	lcWrt->close() ;

      }catch(IOException& e){

	std::cout << " io error in file  " << FILEN[i] << " : " << e.what() << std::endl ;
      }
    }

    return 0 ;
}

