#include "IOIMPL/LCFactory.h"



#include "IO/LCWriter.h"
#include "IO/LCReader.h"

#include "SIO/SIOWriter.h"
#include "SIO/SIOReader.h"


#include "RIO/RIOWriter.h"
#include "RIO/RIOReader.h"


using namespace IO ;
//using namespace SIO ;

namespace IOIMPL{

//   LCFactory* LCFactory::_me = 0 ;
  
  
  LCFactory::LCFactory() {  

    // for now get I/O type from $LCIO_IO_TYPE
    // one of "ss","sr","rs","rr" 
    // i.e. SIOReader/SIOWriter, SIOReader/RIOWriter, ...

    char* t = getenv("LCIO_IO_TYPE");

    if( t==0 ) 
      t = "ss" ;

    _readerType = t[0] ;
    _writerType = t[1] ;

  }
  

  LCFactory* LCFactory::getInstance() { 

    static LCFactory _me ;
    //    if( !_me ) _me = new LCFactory ;

    return &_me ;
  }
  

  LCFactory::~LCFactory() { 
    //    delete _me ;
  }
  
  LCWriter * LCFactory::createLCWriter() { 
    
    // the reason for having this class
    // so far we just create SIO objects
    
    switch(_writerType ) {
      
    case 'r':
      return new RIO::RIOWriter ;
      break ;
      
    default :
      return new SIO::SIOWriter ;
    }
    
  }
  
  LCReader * LCFactory::createLCReader(int lcReaderFlag) {
    
    switch(_readerType ) {

    case 'r':
      return new RIO::RIOReader( lcReaderFlag );
      break;

    default :
      return new SIO::SIOReader( lcReaderFlag ) ;
    }

  }
  
  
} // namespace
