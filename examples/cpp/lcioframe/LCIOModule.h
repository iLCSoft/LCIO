#ifndef LCIOModule_h
#define LCIOModule_h 1

#include "lcio.h"

#include "IO/LCRunListener.h"
#include "IO/LCEventListener.h"
#include "IO/LCReader.h"
#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"

#include <map>

using namespace std ;
using namespace lcio ;

class LCIOModule ;
class LCIOModuleMap ;

typedef std::map< std::string , LCIOModule* > ModuleMap ;

/** Base class for modules for analyzing LCIO files.
 * Provides (empty) default implementations of member functions.
 * Inherit from this class and create one global instance 
 * of your module.
 */
class LCIOModule : public LCRunListener, public LCEventListener{
  
private:

  LCIOModule() ; 

public:
  

  /* Only constructor - specify a unique name  for your module.
   * Do not allocate any resources in the constructor but use
   * initModule() instead.
   */
  LCIOModule(const char* moduleName) ; 

  virtual ~LCIOModule() ; 
  

  // the LCRunListener and LceventListener interfaces default implementations
  // overwrite in your module as needed (typically the processRunHeader/Event methods)
  virtual void processEvent( LCEvent * evt ) {  /*no_op*/ ;}
  virtual void processRunHeader( LCRunHeader* run) { /*no_op*/; }

  virtual void modifyEvent( LCEvent * evt ) { /*no_op*/ ;}
  virtual void modifyRunHeader( LCRunHeader* run) { /*no_op*/; }   


  virtual void initModule() ;
  
};

class LCIOModuleMap : public ModuleMap{

public:
  // returns the map of all modules
  static LCIOModuleMap* getInstance() ;

protected:
  // static map of all known modules
  static LCIOModuleMap* _map ;
} ;

#endif
