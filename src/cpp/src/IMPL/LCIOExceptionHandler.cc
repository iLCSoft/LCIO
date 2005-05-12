#include <exception> 
#include <iostream>
#include <cstdlib>
#include "IMPL/LCIOExceptionHandler.h"

namespace IMPL {


  // create one globale instance of the handler
  LCIOExceptionHandler* LCIOExceptionHandler::_me = 0 ;


/** Method catches any std::exception, prints the message to stdout
 * and then exits the program. 
 */
void lcio_unexpected(){
  
  try{
    
    throw ;
    
  }catch( std::exception& e){
    
    std::cout << " A runtime error has occured : " 
	      << e.what() 
	      << std::endl
	      << " the program will have to be terminated - sorry." << std::endl ;
    exit(1) ;
  }
}

  LCIOExceptionHandler::LCIOExceptionHandler(){
      
    std::set_unexpected( lcio_unexpected ) ;
    std::set_terminate( lcio_unexpected ) ;
  }
    

  LCIOExceptionHandler* LCIOExceptionHandler::createInstance(){

    if( ! _me ){
      _me = new LCIOExceptionHandler ;
    }

    return _me ;
  }
}
