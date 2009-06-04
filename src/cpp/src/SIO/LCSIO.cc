#include "SIO/LCSIO.h"

#include "SIO_functions.h"
#include "SIO_definitions.h"
#include <cctype>
#include "Exceptions.h"

#include <iostream>
#include <sstream>

using namespace EVENT ;

namespace SIO {

  int LCSIO::uid = 0 ;
  int LCSIO::dummy_size = LCSIO::dummy_initial_size ;
  char* LCSIO::dummy = new char[ LCSIO::dummy_initial_size ]   ;
  
  const char* LCSIO::RUNRECORDNAME = "LCRunHeader" ;
  const char* LCSIO::RUNBLOCKNAME = "RunHeader" ;
  const char* LCSIO::EVENTRECORDNAME = "LCEvent"  ;
  const char* LCSIO::EVENTBLOCKNAME="Event" ;
  const char* LCSIO::HEADERRECORDNAME = "LCEventHeader"  ;
  const char* LCSIO::HEADERBLOCKNAME="EventHeader" ;

  const char* LCSIO::FILE_EXTENSION=".slcio" ;


  void LCSIO::checkVersion(int versionID){
    if ( SIO_VERSION_MAJOR( versionID ) < 1 && SIO_VERSION_MINOR(versionID) < 8)
      throw IO::IOException(" Old file versions ( < v00-08 ) no longer supported !") ;    
  }

//   unsigned int LCSIO::read( SIO_stream* stream ,char** c, int versionID){
    
//     int status ;
//     int strLen ;
//     status = SIO_functions::data( stream ,  &strLen  , 1  ) ;
    
//     if( !( status & 1 ) ) return status ;
    
//     // make sure our string buffer is large enough 
//     while( strLen + 1 > dummy_size ){
//       dummy_size += dummy_size  ;
//       delete[] dummy ;
//       dummy = new char[ dummy_size  ] ;
//     }
    
//     // fg20030508 old files have trailing '\00' for strings ...
//     if( SIO_VERSION_MAJOR( versionID ) < 1 && SIO_VERSION_MINOR( versionID ) < 3 ) {
      
//       SIO_functions::data( stream ,  dummy  , strLen + 1  ) ;
      
//     } else {
      
//       SIO_functions::data( stream ,  dummy  , strLen ) ;
//       dummy[ strLen ] = '\0' ; // still needed for char* ...
//     }
    
//     *c = dummy  ;
    
//     return status ;
    
//   }
  
  unsigned int LCSIO::read( SIO_stream* stream ,char** c, int* len){
    
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
    if(len!=0) 
      *len = strLen ;

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

// needed for LP64 architectures
#if defined(_LP64) || defined(__APPLE_CC__)
unsigned int LCSIO::write( SIO_stream* stream , size_t i){

  unsigned int local = i ;
  return  SIO_functions::data( stream ,  &local , 1  ) ;
 
}
#endif

unsigned int LCSIO::write( SIO_stream* stream , long64 i){

  long64 local = i ;
  return  SIO_functions::data( stream ,  &local , 1  ) ;
 
}

unsigned int LCSIO::write( SIO_stream* stream , float f){

  float local = f ;
  return  SIO_functions::data( stream ,  &local , 1  ) ;
}
  

unsigned int LCSIO::write(SIO_stream* stream , const std::string& s){

//   int status ;
//   int strLen = s.length() ;

//   // make sure our string buffer is large enough 
//   while( strLen + 1 > dummy_size ){
//     dummy_size += dummy_size  ;
//     delete[] dummy ;
//     dummy = new char[ dummy_size  ] ;
//   }
//   strcpy( dummy , s.c_str() ) ;
  
//   status = SIO_functions::data( stream ,  & strLen  , 1  ) ;
//   if( !( status & 1 ) ) return status ;
  
//   // fg 20030508 don't write trailing '\0's any more (v-00-03)
//   status = SIO_functions::data( stream ,  dummy , strLen ) ;

//   return status ;

  // fg: why not use const_cast here - then we don't need to copy to a buffer ....
  int strLen = s.length() ;
  int status = SIO_functions::data( stream ,  & strLen  , 1  ) ;
  if( !( status & 1 ) ) return status ;
  return  SIO_functions::data( stream , const_cast<char*>( s.c_str() ), strLen );
}
  
  

  
  //const char* LCSIO::getValidSIOName(const std::string& aName ){
std::string LCSIO::getValidSIOName(const std::string& aName ) {

  const char* name  = aName.c_str()  ;

  char* newName =  new char[  aName.length() + 1 ];
  const char * returnStrP = newName ;
  std::stringstream returnStr ;

  if( *name == '\\' || *name == '/' || *name=='.' )
    *newName++ = '_' ;
  // if first char is not alpha or '_' replace it with an 'A'
  else if ( (*name < 0 ) || ( !isalpha( (int)*name ) && *name != '_' ) )
    *newName++ = 'A' ;
  else 
    *newName++ = *name ;
  

  for( name += 1; *name != '\0'; name++ ){
    
    if( *name == '\\' || *name == '/' || *name=='.' )
      *newName++ = '_' ;

    else if( (*name>=0) && ( isalnum( (int)*name ) || *name == '_' ) )
      *newName++ = *name ;
    else
    ; // ignore
  } 
  
  *newName = '\0' ;

  // JE: append uid to name
  returnStr << returnStrP << uid++;
  delete[] returnStrP ;

  return returnStr.str() ;
}


} // namespace 
 
