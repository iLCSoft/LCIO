#include "lcio.h"

#include "IO/LCWriter.h"

// position of record name for last random access record
#define LCSIO_RANDOMACCESS_SIZE 112

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

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

    int firstFile = 1 ;
    bool force = false ;
    std::string firstArg( argv[1] ) ;
    
    if( firstArg == "-f" ){ // force: 'undocumented' feature for fixing files with broken TOC (SplitWriter)
      firstFile++ ;
      std::cout << "  ====== called with -f (force) : will create a TOC for direct access in any case ! " << std::endl ;
      force = true ;
    }
    
    for(int i=firstFile ; i < argc ; i++){
      FILEN.push_back( argv[i] )  ;
    }

    int nFiles = argc - firstFile ;

    for( int i=0 ; i <nFiles ; ++i ) {

      try{
      

	if( force) { // --------------------------------------------------------------------------------------------
	  //           if called with force  we rename the last LCIORandomAcces record so that it is ignored and the
	  //           file treated as an old one w/o direct access....
	  
	  FILE* f = fopen( FILEN[i].c_str()  , "r+") ;
	  
	  if( f != 0 ){
	    
	    fseek( f ,  -( LCSIO_RANDOMACCESS_SIZE ) , SEEK_END ) ; 
	    std::string bla("") ;
	    bla.resize(17) ;
	    
	    int status = fread( &bla[0] , sizeof(char) , 16 , f ) ;
	    
	    if( !strcmp( bla.c_str() , "LCIORandomAccess") || status != 16 ){ 
	      
	      
	      status = fseek( f ,  -(16) , SEEK_CUR ) ;
	      
	      bla = "LCIORandomIGNORE" ;
	      
	      status = fwrite( &bla[0] , 1 , 16 , f ) ;
	      
	    } 
	  }

	  fclose( f ) ;
	} //---------- end if(force) --------------------------------------------------------------------------------


	lcWrt->open( FILEN[i] , LCIO::WRITE_APPEND )  ;

	lcWrt->close() ;

      }catch(IOException& e){

	std::cout << " io error in file  " << FILEN[i] << " : " << e.what() << std::endl ;
      }
    }

    return 0 ;
}

