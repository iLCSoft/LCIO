#include "SIO/SIOIndexHandler.h"

//#include "SIO/LCIORandomAccessMgr.h"
#include "SIO/LCSIO.h"
#include "SIO_functions.h"

#include "EVENT/LCIO.h"

#include <iostream>
#include <limits.h>

namespace SIO  {


  SIOIndexHandler::SIOIndexHandler(const std::string& bname, LCIORandomAccessMgr* raMgr) : 
    SIO_block( bname.c_str() ),
    _raMgr( raMgr ) {
    
  }
 
  SIOIndexHandler::~SIOIndexHandler(){ }

  unsigned int SIOIndexHandler::xfer( SIO_stream* stream, SIO_operation op, 
					  unsigned int versionID){
  
    LCSIO::checkVersion(versionID) ;
    
    unsigned int status ; // needed by the SIO_DATA macro
  

    if( op == SIO_OP_READ ){ 

      int control ;
      int runMin ;
      long64 baseOffset ;
      int size ;

      SIO_DATA( stream ,  &control, 1  ) ;
     
      bool oneRun     = control & 1 ;
      bool longOffset = control & 2 ;

      //FIXME: do we need this ?
      if( control & 4 ){  // parameters

	std::cerr << " WARNING: SIOIndexHandler: parameters not implemented .... " << std::endl ;

	return SIO_BLOCK_SKIP ; // FIXME: test
      }

      SIO_DATA( stream ,  &runMin, 1  ) ;
      SIO_DATA( stream ,  &baseOffset, 1  ) ;
      SIO_DATA( stream ,  &size, 1  ) ;


//       int nEvents = 0;
//       int nRunHeaders = 0;

      int runNum ;
      int evtNum ;
      int runOffset ;
      long64 pos ;

      int dummy_int; 
      long64 dummy_long ;

      for (int i = 0; i < size; i++) {

	runNum = runMin ;

	if( !oneRun ) {
	  SIO_DATA( stream ,  &runOffset, 1  ) ;
	  runNum += runOffset ;
	}
	SIO_DATA( stream ,  &evtNum , 1  ) ;

// 	if (evtNum >= 0) 
// 	  nEvents++;
// 	else 
// 	  nRunHeaders++;

	if( longOffset ){

	  SIO_DATA( stream ,  &dummy_long , 1  ) ;
	  pos = dummy_long ;

	} else {

	  SIO_DATA( stream ,  &dummy_int , 1  ) ;
	  pos = dummy_int ;
	}

	pos += baseOffset ;

	//	if( evtNum < 0 ) 
	  //	  std::cout << " +++++++++++ adding run " << runNum << " evt: " << evtNum << " at " << pos <<std::endl ; 

	_raMgr->_runEvtMap.add( RunEvent( runNum , evtNum ) ,  pos ) ;
      }

//       _raMgr->addLCIORandomAccess( ra ) ;
//       std::cout << " ... LCIORandomAccess read from stream : " << *ra << std::endl ;

      //****************************************************************************************************

    }  else if( op == SIO_OP_WRITE ){ 


//       std::cout << " -- SIOindexHandler - write called - runeventmap :" 
// 		<< _raMgr->_runEvtMap << std::endl ;

      unsigned control = 0 ;

      RunEvent minEntry  = _raMgr->_runEvtMap.minRunEvent() ;
      RunEvent maxEntry  = _raMgr->_runEvtMap.maxRunEvent() ;
      
      bool oneRun = minEntry.RunNum == maxEntry.RunNum ;
      
      if( oneRun )
	control |= 1 ;

      long64 posMin = _raMgr->_runEvtMap.getPosition( minEntry ) ;
      long64 posMax = _raMgr->_runEvtMap.getPosition( maxEntry ) ;
      
      bool longOffset =   ( posMax - posMin ) > INT_MAX ;
      
      if( longOffset )
	control |= 2 ;

      //+++++ if we would add parameters to the LCIO index record
      //+++++ control |= 4 ;

      SIO_DATA( stream ,  &control, 1  ) ;

      int runMin = minEntry.RunNum ;
      SIO_DATA( stream ,  &runMin, 1  ) ;


      long64 baseOffset = posMin ;
      SIO_DATA( stream ,  &baseOffset, 1  ) ;


      int size =  _raMgr->_runEvtMap.size() ;

//       std::cout << " -- SIOindexHandler - write : " 
// 		<< " control : " << control 
// 		<< " baseoffset : " << baseOffset 
// 		<< " size: " << size 
// 		<< " min: " << minEntry 
// 		<< " max: " << maxEntry
// 		<< std::endl ;

      
      SIO_DATA( stream ,  &size, 1  ) ;
      
      for( RunEventMap::Map_IT it =  _raMgr->_runEvtMap.begin() ; it !=  _raMgr->_runEvtMap.end() ; ++it ){
	
	const RunEvent& re = it->first ;
	
	if( !oneRun ) {

	  int runOffset =  re.RunNum - runMin ;

	  SIO_DATA( stream ,  &runOffset, 1  ) ;
	}
	
	int evtNum = re.EvtNum ;
	SIO_DATA( stream ,  &evtNum , 1  ) ;
	
	// 	if (evtNum >= 0) 
	// 	  nEvents++;
	// 	else 
	// 	  nRunHeaders++;
	
	
	if( longOffset ){
	  
	  long64 dummyL = it->second - baseOffset ;
	  SIO_DATA( stream ,  &dummyL , 1  ) ;
	  
	} else {
	  
	  int dummyI = it->second - baseOffset ;
	  SIO_DATA( stream ,  &dummyI , 1  ) ;
	}
	
      }

    }
  
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  unsigned int   SIOIndexHandler::version(){
    
    return SIO_VERSION_ENCODE( EVENT::LCIO::MAJORVERSION, EVENT::LCIO::MINORVERSION ) ;
  }
  
}
