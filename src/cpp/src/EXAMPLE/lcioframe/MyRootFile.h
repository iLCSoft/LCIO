#ifndef MyRootFile_h
#define MyRootFile_h

#include "TFile.h"

/** Singleton for one instance of a root file used by all modules.
 */
class MyRootFile: public TFile{

 private:
  MyRootFile() ; 


  MyRootFile(const char* name,const char* mode,const char *title) ; 

public:
  
  ~MyRootFile() ; 

  /** Returns the current root file is it exists - the first call to this 
   * function determines the name of the file !
   */
  static MyRootFile* getInstance(const char* FileName ) ; 

  //private:
  static MyRootFile* _hfile ; // the root file
};


#endif
