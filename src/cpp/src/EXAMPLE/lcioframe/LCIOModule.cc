#include "LCIOModule.h"

using namespace lcio ;


LCIOModuleMap* LCIOModuleMap::_map = 0 ;

LCIOModuleMap* LCIOModuleMap::getInstance(){
  if( !_map )
    _map = new LCIOModuleMap ;
  return _map ;
}


LCIOModule::LCIOModule(const char* moduleName){

  //register module in map
  (* LCIOModuleMap::getInstance() )[  moduleName  ] = this ; 
}

LCIOModule::LCIOModule(){}

LCIOModule::~LCIOModule() {}


void LCIOModule::initModule() {
  // nothing to be done here
}
