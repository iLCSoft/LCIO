#include "lcio.h"
#include "LCIOModule.h"
#include "CalorimeterHitHistograms.h"
#include "IO/LCReader.h"

#include "LCIOFrameSteer.h"
#include <sstream>
#include <fstream>
#include <string>

using namespace std ;
using namespace lcio ;


typedef std::vector<const char*> StringVec ;

void registerModule(const char* moduleName, LCReader* lcReader) ;
int readNextValidLine( std::string& str , istream& stream) ;


/** Example of a mini framework that can be used to ananlyse  LCIO data files 
 *  with root. The main program is independent of root and can be used with other 
 *  analysis tools as well. All you need to do is to write your own subclasses
 *  of LCModule.
 */
int main(int argc, char** argv ){
  
  const char* steeringFileName = "lcioframe.steer" ;
  
  
  // read file name from command line
  if( argc > 1 ) {
    
    string filename( argv[1]  ) ;
    if( filename.substr(   filename.length() - strlen(".slcio") , strlen(".slcio") ) !=
	".slcio") {
      
      LCIOFrameSteer::baseFileName = filename ; 
      
    } else {
      
      LCIOFrameSteer::baseFileName =  filename.substr(0 ,  filename.length() - strlen(".slcio") )  ;
    }
    
  } else {
    cout << " usage: lcioframe lciofile [lcioframe.steer]" << endl 
	 << "     lciofile: filename " << endl 
	 << "     [lcioframe.steer] optional steering file name - default is lcioframe.steer" 
	 << endl ;
    exit(1) ;
  }

  if( argc > 2 ){
    steeringFileName = argv[2] ;
  }
  
  stringstream fileName ; 
  fileName << LCIOFrameSteer::baseFileName << ".slcio" << ends ;

  // create lcio reader 
  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
  
  lcReader->open( fileName.str()  ) ; 
  
  // open steering file with module names 

  ifstream inFile( steeringFileName ) ;
  if( ! inFile ){
    std::cout << " Couldn't open steering file: " << steeringFileName  << std::endl ;
    exit(1) ;
  }
  string moduleName ;
  // try{
  while( readNextValidLine(moduleName, inFile)  ){

    registerModule( moduleName.c_str()  , lcReader ) ;

  }

  lcReader->readStream() ;
  lcReader->close() ;

  return 0 ;

}

void registerModule(const char* moduleName, LCReader* lcReader){

  LCIOModule*lcioModule = (* LCIOModuleMap::getInstance() )[  moduleName  ] ;
  if( lcioModule ) {
    lcioModule->initModule() ;
    lcReader->registerLCRunListener( lcioModule ) ; 
    lcReader->registerLCEventListener( lcioModule ) ; 
    cout << " registering module : " << moduleName << endl ;
  } else{
    cout << "WARNING: no instance of module: " << moduleName << " found ! " << endl ;
  }

}

/** Small helper method that reads the next line from stdin (cin)  
 * that is not empty or starts with a '#' 
 */
int readNextValidLine( std::string& str , istream& stream){
  
//   static ifstream inFile("modules.steer") ;

//   if( ! inFile )
//     throw new lcio::Exception(" Couldn't open steering file: lcioframe.steer" ) ;

  while(  ! stream.eof()  ) {

    getline( stream , str ) ;

    if( str.length() != 0  && str[0] != '#' )
      return  str.length() ;
  }
  return 0 ;
}
