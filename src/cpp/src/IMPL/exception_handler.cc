#include <exception> 
#include <iostream>

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

/** Sets the default error handling for unexpected and uncaught exceptions.
 */
class LCIOExceptionHandler{

 public:
  LCIOExceptionHandler(){

    std::set_unexpected( lcio_unexpected ) ;
    std::set_terminate( lcio_unexpected ) ;
  }


};





