#include "SIO/LCSIO.h"

#include "SIO_functions.h"

namespace SIO {

int LCSIO::dummy_size = LCSIO::dummy_initial_size ;
char* LCSIO::dummy = new char[ LCSIO::dummy_initial_size ]   ;


unsigned int LCSIO::read( SIO_stream* stream ,char** c){
  
  int status ;
  int strLen ;
  status = SIO_functions::data( stream ,  &strLen  , 1  ) ;
  
  if( !( status & 1 ) ) return status ;
  
  // make sure our string buffer is large enough 
  while( strLen + 1 > dummy_size ){
    dummy_size += dummy_size  ;
    delete[] dummy ;
    dummy = new char[ dummy_size  ] ;
    //    cout << " read : allocating  " << dummy_size  << " bytes ! StrLen : " <<  strLen<< endl ;
    
  }
  
  SIO_functions::data( stream ,  dummy  , strLen + 1  ) ;
  
  //  std::cout << " reading string with length " << strLen << " : " << dummy << std::endl ; 
  
  *c = dummy  ;
  
  return status ;
  
}
  
unsigned int LCSIO::write( SIO_stream* stream , int i){

  int local = i ;
  return  SIO_functions::data( stream ,  &local , 1  ) ;
 
}
unsigned int LCSIO::write( SIO_stream* stream , unsigned int i){

  unsigned int local = i ;
  return  SIO_functions::data( stream ,  &local , 1  ) ;
 
}
unsigned int LCSIO::write( SIO_stream* stream , long i){

  long long local = i ;
  return  SIO_functions::data( stream ,  &local , 1  ) ;
 
}

unsigned int LCSIO::write( SIO_stream* stream , float f){

  float local = f ;
  return  SIO_functions::data( stream ,  &local , 1  ) ;
}
  

unsigned int LCSIO::write(SIO_stream* stream , const std::string& s){

  int status ;
  int strLen = s.length() ;

  // make sure our string buffer is large enough 
  while( strLen + 1 > dummy_size ){
    dummy_size += dummy_size  ;
    delete[] dummy ;
    dummy = new char[ dummy_size  ] ;
    //cout << " write : allocating  " << dummy_size  << " bytes ! StrLen : " <<  strLen<< endl ;
  }
  strcpy( dummy , s.c_str() ) ;
  
  status = SIO_functions::data( stream ,  & strLen  , 1  ) ;
  if( !( status & 1 ) ) return status ;
  status = SIO_functions::data( stream ,  dummy , strLen + 1  ) ;

  return status ;
}

}; // namespace 
 
