
#include "UTIL/LCSplitWriter.h"

#include <sys/stat.h> 

#include <sstream>
#include <iomanip>
#include <iostream>

#ifdef SPLIT_WRITER_NDIGITS
#define NDIGITS SPLIT_WRITER_NDIGITS
#else
#define NDIGITS 3
#endif

#if defined(__CYGWIN__) || defined(__APPLE_CC__)
#define STAT64 stat
#else
#define STAT64 stat64
#endif



using namespace EVENT ;


namespace UTIL{



  void LCSplitWriter::open(const std::string & filename) {
    _count = 0 ;
    setBaseFilename( filename ) ;
    _wrt->open( getFilename() ) ;
  }
  
  void LCSplitWriter::open(const std::string & , int ) {
      throw Exception(" LCSplitWriter doesn't support  NEW and APPEND mode ! "
		      " Please remove your old file(s) and use the default mode." ) ;
  }
  
  void LCSplitWriter::writeRunHeader(const EVENT::LCRunHeader * hdr) {

    _wrt->flush() ;
    if( fileSize() > _maxBytes ) {
      _wrt->close() ;
      ++_count ;
      _wrt->open( getFilename()  ) ; 
    }

    _wrt->writeRunHeader( hdr ) ;
  }

  void LCSplitWriter::writeEvent(const EVENT::LCEvent * evt) {

    _wrt->flush() ;

    if( fileSize() > _maxBytes ) {
      
//       std::cout << " switching to new file - old size : " << fileSize() << " > " << _maxBytes  << std::endl ;

      _wrt->close() ;
      ++_count ;
      _wrt->open( getFilename()  ) ; 

//       std::cout << " switching  new file size : " << fileSize() << " - file " <<  getFilename() << std::endl ;

    }

    _wrt->writeEvent( evt ) ;
   }
  
  void LCSplitWriter::close() {
    _wrt->close() ;
   }

  void LCSplitWriter::flush() {
    _wrt->flush() ;
   }




  const std::string&   LCSplitWriter::getFilename() { 
    
    if( _count != _lastCount ) 
      _filename = std::string( _baseFilename + "." + getCountingString( _count ) + _extension )  ; 
    
    _lastCount = _count ;
    
    return _filename ;
  } 



  long64 LCSplitWriter::file_size( const char *fname) {
    
    struct STAT64 sbuf; 
    
    int ret = STAT64(fname, &sbuf);
    
    if( ret < 0 ) 
      return -1 ;

    return sbuf.st_size ;
  }
  
  long64 LCSplitWriter::fileSize() {
    
//     // #include <sys/stat.h> 
//     struct stat64 sbuf; 
    
//     int ret = stat64( getFilename().c_str() , &sbuf);
    
//     if( ret < 0 ) return -1 ;
    
//     return sbuf.st_size ;
    
    return file_size( getFilename().c_str()   ) ;
  }
  
  
  void LCSplitWriter::setBaseFilename( const std::string& filename ) {
    
    unsigned dotPos = filename.find_last_of('.') ;
    
    if( ( dotPos > 0  )                            &&    // we  have a basefile name
	( dotPos == filename.length() - 6  )       &&    // with a 5 character extension 
	( filename.rfind("lcio") == dotPos + 2 ) ) {     // that ends on lcio
      
      _baseFilename = filename.substr( 0 , filename.length() - 6 )  ;
      _extension = filename.substr( filename.length() - 6 , filename.length() )  ;

    }else{
       
      throw Exception(" LCSplitWriter only works with complete file names including extension, e.g. myfile.slcio" ) ;

//       _baseFilename = filename ;
    }
  }
  
  std::string LCSplitWriter::getCountingString(unsigned count) {

    std::stringstream countStream ;


    countStream << std::setw( NDIGITS ) << std::setfill('0') << count  ;
    
//     std::cout << " ------------ getCountingString(" << count << ") " <<  countStream.str() << std::endl ;

    return countStream.str() ;
  }
  
  
} 
