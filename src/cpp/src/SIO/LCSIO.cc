#include "SIO/LCSIO.h"

#include "SIO_functions.h"
#include "SIO_definitions.h"

namespace SIO {

  int LCSIO::dummy_size = LCSIO::dummy_initial_size ;
  char* LCSIO::dummy = new char[ LCSIO::dummy_initial_size ]   ;
  
  const char* LCSIO::RUNRECORDNAME = "LCRunHeader" ;
  const char* LCSIO::RUNBLOCKNAME = "RunHeader" ;
  const char* LCSIO::EVENTRECORDNAME = "LCEvent"  ;
  const char* LCSIO::EVENTBLOCKNAME="Event" ;
  const char* LCSIO::HEADERRECORDNAME = "LCEventHeader"  ;
  const char* LCSIO::HEADERBLOCKNAME="EventHeader" ;



  unsigned int LCSIO::read( SIO_stream* stream ,char** c, int versionID){
    
    int status ;
    int strLen ;
    status = SIO_functions::data( stream ,  &strLen  , 1  ) ;
    
    if( !( status & 1 ) ) return status ;
    
    // make sure our string buffer is large enough 
    while( strLen + 1 > dummy_size ){
      dummy_size += dummy_size  ;
      delete[] dummy ;
      dummy = new char[ dummy_size  ] ;
    }
    
    // fg20030508 old files have trailing '\00' for strings ...
    if( SIO_VERSION_MAJOR( versionID ) < 1 && SIO_VERSION_MINOR( versionID ) < 3 ) {
      
      SIO_functions::data( stream ,  dummy  , strLen + 1  ) ;
      
    } else {
      
      SIO_functions::data( stream ,  dummy  , strLen ) ;
      dummy[ strLen ] = '\0' ; // still needed for char* ...
    }
    
    *c = dummy  ;
    
    return status ;
    
  }
  
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
    
    SIO_functions::data( stream ,  dummy  , strLen ) ;
    dummy[ strLen ] = '\0' ; // still needed for char* ...
    
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
  }
  strcpy( dummy , s.c_str() ) ;
  
  status = SIO_functions::data( stream ,  & strLen  , 1  ) ;
  if( !( status & 1 ) ) return status ;
  
  // fg 20030508 don't write trailing '\0's any more (v-00-03)
  status = SIO_functions::data( stream ,  dummy , strLen ) ;

  return status ;
}

}; // namespace 
 
