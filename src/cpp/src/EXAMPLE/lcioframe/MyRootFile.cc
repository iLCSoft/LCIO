#include "MyRootFile.h"

MyRootFile* MyRootFile::_hfile = 0 ;


MyRootFile::MyRootFile(const char* name,const char* mode,const char *title) :
  TFile( name , mode, title )
{
}

MyRootFile::~MyRootFile() {

  if( _hfile )
    _hfile->Close() ;
}

MyRootFile* MyRootFile::getInstance(const char* name){

  if( !_hfile )
    _hfile = new MyRootFile( name ,
		   "RECREATE","Root file with example histograms from arbitrary LCIO file") ;

  return _hfile ;
}
