#include "SIO/LCSIO.h"

#include "SIO_functions.h"
#include "SIO_stream.h"
#include "SIO_recordManager.h"
#include <cctype>
#include <cerrno>
#include "Exceptions.h"

#include <iostream>
#include <sstream>

using namespace EVENT ;

namespace SIO {

  //-------------------------------
  SIORecords::SIORecords(){
   
    add( _records[ Event ]  , LCSIO_EVENTRECORDNAME ) ;
    add( _records[ Header ] , LCSIO_HEADERRECORDNAME ) ;
    add( _records[ Run ]    , LCSIO_RUNRECORDNAME ) ;
    add( _records[ Access ] , LCSIO_ACCESSRECORDNAME ) ;
    add( _records[ Index ]  , LCSIO_INDEXRECORDNAME ) ;

  }
  SIO_record* SIORecords::operator[](size_t idx){

    if( idx >= NumberOfRecords ) 
      throw IO::IOException( " Out of range in SIORecords::operator[](size_t i)" ) ;

    return _records[ idx ] ;
  }

  void SIORecords::add(SIO_record*& rec , const char* name){
    
    rec = SIO_recordManager::add( name ) ; 

    if( rec == 0 ) {
      std::string err("[LCSIO::SIORecords]: Could not add SIO record: ") ;
      err+=name ;
      throw IO::IOException( err ) ;
    }
  }
  
  void  SIORecords::setCompress( bool flag) {

    for( unsigned i=0; i < NumberOfRecords ; ++i ){

      if( i != Access )
	_records[i]->setCompress( flag ) ;
      else
	_records[i]->setCompress( false ) ; // random access records are always uncompressed ....

    }
  }

  SIORecords::Unpack::Unpack( unsigned recordFlag ) {

    for( unsigned i=0; i < NumberOfRecords ; ++i ){

      _flags[i] = LCSIO::records()[i]->getUnpack() ;
    
      LCSIO::records()[ i ]->setUnpack(   recordFlag & ( 0x0001 << i )  ) ;
    }
  }
  SIORecords::Unpack::~Unpack() {

    for( unsigned i=0; i < NumberOfRecords ; ++i ){

      LCSIO::records()[ i ]->setUnpack( _flags[i] ) ;
    }
    
  }

  //-------------------------------
 

  int LCSIO::uid = 0 ;
  int LCSIO::dummy_size = LCSIO::dummy_initial_size ;
  char* LCSIO::dummy = new char[ LCSIO::dummy_initial_size ]   ;
  
//   //FIXME: remove these ....
//   const char* LCSIO::RUNRECORDNAME = "LCRunHeader" ;
//   const char* LCSIO::RUNBLOCKNAME = "RunHeader" ;
//   const char* LCSIO::EVENTRECORDNAME = "LCEvent"  ;
//   const char* LCSIO::EVENTBLOCKNAME="Event" ;
//   const char* LCSIO::HEADERRECORDNAME = "LCEventHeader"  ;
//   const char* LCSIO::HEADERBLOCKNAME="EventHeader" ;

  const char* LCSIO::FILE_EXTENSION=".slcio" ;


  void LCSIO::seekStream( SIO_stream* stream , long64 pos) {

    if( stream->getState() != SIO_STATE_OPEN ){
      
      throw IO::IOException( std::string(" stream not open: ")+ *stream->getName() ) ;
    }
    
    int status ;
    
    if( pos < 0 ) 
      status = stream->seek( pos , SEEK_END ) ; 
    else
      status = stream->seek( pos ) ;
    
    if( status != SIO_STREAM_SUCCESS ) {
      std::stringstream s ;  s << "[LCSIO::seekStream] Can't seek stream to " << pos << "  errno : " << errno ;
      throw IO::IOException( s.str() ) ;
    }

  }


  void LCSIO::checkVersion(int versionID){
    if ( SIO_VERSION_MAJOR( versionID ) < 1 && SIO_VERSION_MINOR(versionID) < 8)
      throw IO::IOException(" Old file versions ( < v00-08 ) no longer supported !") ;    
  }


  SIORecords& LCSIO::records() {
    static SIORecords recs ;
    return recs ;
  }

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
  } 
  
  *newName = '\0' ;

  // JE: append uid to name
  returnStr << returnStrP << uid++;
  delete[] returnStrP ;

  return returnStr.str() ;
}


} // namespace 
 
