#ifndef LCCol2Branch_h
#define LCCol2Branch_h 1

#include "lcio.h"
#include "EVENT/LCEvent.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/ReconstructedParticleImpl.h"
#include "UTIL/LCTypedVector.h"

//#include "TSystem.h"
//#include "TFile.h"
#include "TTree.h"
//#include "TClassTable.h"

//typedef IMPL::ReconstructedParticleImpl T ;


class LCBranch{

public: 
  virtual void fill( EVENT::LCEvent* evt )=0 ;
} ;

/** Helper class that allows to create and fill a ROOT branch from an LCCollection.
 *  
 */
template <typename T>
class LCCol2Branch : public LCBranch {
  
public: 
  
  /**
   */
  LCCol2Branch(const char* name,  TTree* tree ) : 
    _name(name) {
    
    _tv = new std::vector<T*> ;
    
    tree->Branch( name  , &_tv, 16000, 2 );

  }
  
  
  
  
  virtual void fill( EVENT::LCEvent* evt ){

    _tv->clear() ;
    _tv->resize( 0 ) ;


    try{
      
      EVENT::LCCollection* col =  evt->getCollection( _name ) ;
      
      int n = col->getNumberOfElements() ;
      
      _tv->resize( n ) ;
      
      for( int i=0; i<n ; ++i){
	
	(*_tv)[i] = dynamic_cast<T*>( col->getElementAt(i) )  ;
      }
      
    } catch(const lcio::DataNotAvailableException& e){

       std::cout << "  collection not found : " << _name << std::endl ;
    }    


    //    std::cout << "  size of _tv " << _tv->size()  << " for " << _name  << std::endl ;

  }



  virtual ~LCCol2Branch() { 

    delete _tv ;
  }
  


protected:
  std::string _name ;
  std::vector<T*>* _tv  ;

}; // class


#endif 
//=============================================================================
