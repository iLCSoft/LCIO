
#include "CPPFORT/lcmhepevtc2f.h"
#include "CPPFORT/HEPEVT.h"

#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>

using namespace lcio ;
using namespace HEPEVTIMPL ;

#define LCEVENT_PNTR( event ) if(! (event) ) return LCIO::ERROR ; \
LCEvent* lcEvent = reinterpret_cast<LCEvent*>( (event) ) ;


 PTRTYPE LCSetHepEvt( PTRTYPE evtout ){
  LCEVENT_PNTR( evtout ) ;
  try { 
    HEPEVT::fromHepEvt(  lcEvent ) ;
    return LCIO::SUCCESS ;
  }catch( Exception& e1) {
    std::cerr << "Exception in LCSetHepEvt: " << e1.what() << std::endl ;
    return LCIO::ERROR ;
  }
}


PTRTYPE LCGetHepEvt( PTRTYPE event ){
  LCEvent* lcEvent = reinterpret_cast<LCEvent*>( (event) ) ;
  try { 
    HEPEVT::toHepEvt(  lcEvent ) ;
    return LCIO::SUCCESS ;
  }catch( Exception& e1) {
    std::cerr << "Exception in LCGetHepEvt: " << e1.what() << std::endl ;
    return LCIO::ERROR ;
  }
}

