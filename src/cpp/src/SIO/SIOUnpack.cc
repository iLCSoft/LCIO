#include "SIO/SIOUnpack.h"

#include "SIO/SIOWriter.h"

namespace SIO{
  
  SIOUnpack::SIOUnpack( int recordFlag ) {
    
    _runFlag = SIOWriter::_runRecord->getUnpack() ;
    SIOWriter::_runRecord->setUnpack(   recordFlag & RUN   ) ;

    _evtFlag = SIOWriter::_evtRecord->getUnpack() ;
    SIOWriter::_evtRecord->setUnpack(   recordFlag & EVENT   ) ;

    _evtHdrFlag = SIOWriter::_hdrRecord->getUnpack() ;
    SIOWriter::_hdrRecord->setUnpack(   recordFlag & EVENTHDR   ) ;

  }

  SIOUnpack::~SIOUnpack() {

    SIOWriter::_runRecord->setUnpack(  _runFlag ) ;
    SIOWriter::_evtRecord->setUnpack(  _evtFlag ) ;
    SIOWriter::_hdrRecord->setUnpack(  _evtHdrFlag  ) ;

  }
  
} // end namespace
