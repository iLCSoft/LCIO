#include "SIO/SIOIndexHandler.h"

// -- lcio headers
#include "SIO/LCSIO.h"
#include "EVENT/LCIO.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

// -- std headers
#include <limits>

namespace SIO  {

  SIOIndexHandler::SIOIndexHandler() :
    sio::block( LCSIO::IndexBlockName, LCSIO::blockVersion() ),
    _runEventMap( std::make_shared<RunEventMap>() ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOIndexHandler::setRunEventMap( std::shared_ptr<RunEventMap> remap ) {
    _runEventMap = remap ;
  }

  //----------------------------------------------------------------------------

  std::shared_ptr<RunEventMap> SIOIndexHandler::runEventMap() const {
    return _runEventMap ;
  }

  //----------------------------------------------------------------------------

  void SIOIndexHandler::read( sio::read_device &device, sio::version_type vers ) {
    LCSIO::checkVersion( vers ) ;
    int control ;
    SIO_DATA( device ,  &control, 1  ) ;
    bool oneRun     = control & 1 ;
    bool longOffset = control & 2 ;
    //FIXME: do we need this ?
    if( control & 4 ) {  // parameters
      SIO_THROW( sio::error_code::io_failure, "SIOIndexHandler: parameters not implemented ...." ) ;
    }
    int runMin ;
    SIO_DATA( device ,  &runMin, 1  ) ;
    long64 baseOffset ;
    SIO_DATA( device ,  &baseOffset, 1  ) ;
    int size ;
    SIO_DATA( device ,  &size, 1  ) ;

    int runNum ;
    int evtNum ;
    int runOffset ;
    long64 pos ;
    int dummy_int;
    long64 dummy_long ;

    for (int i = 0; i < size; i++) {
    	runNum = runMin ;
    	if( !oneRun ) {
    	  SIO_DATA( device ,  &runOffset, 1  ) ;
    	  runNum += runOffset ;
    	}
    	SIO_DATA( device ,  &evtNum , 1  ) ;
    	if( longOffset ) {
    	  SIO_DATA( device ,  &dummy_long , 1  ) ;
    	  pos = dummy_long ;
    	}
      else {
    	  SIO_DATA( device ,  &dummy_int , 1  ) ;
    	  pos = dummy_int ;
    	}
    	pos += baseOffset ;
    	_runEventMap->add( RunEvent( runNum , evtNum ) ,  pos ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOIndexHandler::write( sio::write_device &device ) {
    unsigned control = 0 ;
    auto minEntry  = _runEventMap->minRunEvent() ;
    auto maxEntry  = _runEventMap->maxRunEvent() ;
    bool oneRun = minEntry.RunNum == maxEntry.RunNum ;
    if( oneRun ) {
    	control |= 1 ;
    }
    auto posMin = _runEventMap->getPosition( minEntry ) ;
    auto posMax = _runEventMap->getPosition( maxEntry ) ;
    bool longOffset =   ( posMax - posMin ) > std::numeric_limits<int>::max() ;
    if( longOffset ) {
    	control |= 2 ;
    }
    SIO_DATA( device ,  &control, 1  ) ;
    int runMin = minEntry.RunNum ;
    SIO_DATA( device ,  &runMin, 1  ) ;
    long64 baseOffset = posMin ;
    SIO_DATA( device ,  &baseOffset, 1  ) ;
    int size = _runEventMap->size() ;
    SIO_DATA( device ,  &size, 1  ) ;
    for( auto it = _runEventMap->begin() ; it != _runEventMap->end() ; ++it ) {
    	if( !oneRun ) {
    	  int runOffset =  it->first.RunNum - runMin ;
    	  SIO_DATA( device ,  &runOffset, 1  ) ;
    	}
    	int evtNum = it->first.EvtNum ;
    	SIO_DATA( device ,  &evtNum , 1  ) ;
    	if( longOffset ) {
    	  long64 dummyL = it->second - baseOffset ;
    	  SIO_DATA( device ,  &dummyL , 1  ) ;
    	}
      else {
    	  int dummyI = it->second - baseOffset ;
    	  SIO_DATA( device ,  &dummyI , 1  ) ;
    	}
    }
  }

}
