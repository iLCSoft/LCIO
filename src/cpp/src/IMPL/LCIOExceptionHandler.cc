
#include <exception> 
#include <iostream>
#include <cstdlib>
#include "IMPL/LCIOExceptionHandler.h"

namespace IMPL {

  void LCIOExceptionHandler::setupHandlers() {
    // thread safe functions ... but deprecated functions ...
    // std::set_unexpected( &LCIOExceptionHandler::handler ) ;
    // std::set_terminate( &LCIOExceptionHandler::handler ) ;
  }
  
  void LCIOExceptionHandler::handler() {
    try {
      throw ;
    }
    catch( std::exception& e) {      
      std::cout << " A runtime error has occured : " 
          << e.what() 
          << std::endl
          << " the program will have to be terminated - sorry." << std::endl ;
      exit(1) ;
    }
  }
}