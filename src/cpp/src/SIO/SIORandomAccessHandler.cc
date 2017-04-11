#include "SIO/SIORandomAccessHandler.h"

#include "SIO/LCIORandomAccess.h"
#include "SIO/LCSIO.h"
#include "SIO_functions.h"

#include "EVENT/LCIO.h"

#include <iostream>


namespace SIO  {


  SIORandomAccessHandler::SIORandomAccessHandler(const std::string& bname, LCIORandomAccessMgr* raMgr) : 
    SIO_block( bname.c_str() ),
    _raMgr( raMgr ) {
    
  }
 

  SIORandomAccessHandler::~SIORandomAccessHandler(){ }

  
  unsigned int SIORandomAccessHandler::xfer( SIO_stream* stream, SIO_operation op, 
					  unsigned int versionID){
  
    LCSIO::checkVersion(versionID) ;
    
    unsigned int status ; // needed by the SIO_DATA macro
  

    if( op == SIO_OP_READ ){ 

      LCIORandomAccess* ra = new LCIORandomAccess ;

      SIO_DATA( stream ,  &(ra->_minRunEvt.RunNum) , 1  ) ;
      SIO_DATA( stream ,  &(ra->_minRunEvt.EvtNum) , 1  ) ;
      
      SIO_DATA( stream ,  &( ra->_maxRunEvt.RunNum) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_maxRunEvt.EvtNum) , 1  ) ;
      
      SIO_DATA( stream ,  &( ra->_nRunHeaders) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_nEvents) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_recordsAreInOrder) , 1  ) ;
      
      SIO_DATA( stream ,  &( ra->_indexLocation) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_prevLocation) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_nextLocation) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_firstRecordLocation) , 1  ) ;

 
      // read record size - ignored for now - need to read if record size changes in the future ...
      int recSize ;
      SIO_DATA( stream ,  &recSize , 1  ) ;

      
      _raMgr->addLCIORandomAccess( ra ) ;

    }  else if( op == SIO_OP_WRITE ){ 
    

      LCIORandomAccess* ra = const_cast<LCIORandomAccess*> ( _raMgr->lastLCIORandomAccess() ) ;
      // these SIO functions really should also exist for const arguments ....

      SIO_DATA( stream ,  &(ra->_minRunEvt.RunNum) , 1  ) ;
      SIO_DATA( stream ,  &(ra->_minRunEvt.EvtNum) , 1  ) ;
      
      SIO_DATA( stream ,  &( ra->_maxRunEvt.RunNum) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_maxRunEvt.EvtNum) , 1  ) ;
      
      SIO_DATA( stream ,  &( ra->_nRunHeaders) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_nEvents) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_recordsAreInOrder) , 1  ) ;
      
      SIO_DATA( stream ,  &( ra->_indexLocation) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_prevLocation) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_nextLocation) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_firstRecordLocation) , 1  ) ;
      

      // write the current size of the LCIORandomAccess record as last word - incl. check at bits 31-16: 0xabcd
      int recSize = (   0xabcd0000 | LCSIO_RANDOMACCESS_SIZE ) ;
      SIO_DATA( stream ,  &recSize , 1  ) ;
     
    }
    
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  unsigned int   SIORandomAccessHandler::version(){
    
    return SIO_VERSION_ENCODE( EVENT::LCIO::MAJORVERSION, EVENT::LCIO::MINORVERSION ) ;
  }
  
}
