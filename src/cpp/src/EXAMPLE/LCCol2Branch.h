#ifndef LCCol2Branch_h
#define LCCol2Branch_h 1

#include "lcio.h"
#include "EVENT/LCEvent.h"
#include "IMPL/LCCollectionVec.h"
//#include "IMPL/ReconstructedParticleImpl.h"
#include "UTIL/LCTypedVector.h"

#include "TTree.h"
//#include "TRefArray.h"



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




// template<>
// void LCCol2Branch<ReconstructedParticleROOT>::fill( EVENT::LCEvent* evt ){
  
//   _tv->clear() ;
//   _tv->resize( 0 ) ;
  
  
//   try{
    
//     EVENT::LCCollection* col =  evt->getCollection( _name ) ;
    
//     int n = col->getNumberOfElements() ;
    
//     _tv->resize( n ) ;
    
//     for( int i=0; i<n ; ++i){
      
//       EVENT::ReconstructedParticle* p = dynamic_cast<EVENT::ReconstructedParticle*>( col->getElementAt(i) )  ;

//       (*_tv)[i]->Particle = p ;

//       unsigned ntrk = p->getTracks().size() ;

//       for( unsigned j=0;j<ntrk;j++){

// ---- TRefs work only for TObjects !!
// 	(*_tv)[i]->Tracks.Add(  p->getTracks()[j] ) ; 

//       }
//     }
    
//   } catch(const lcio::DataNotAvailableException& e){
    
//     std::cout << "  collection not found : " << _name << std::endl ;
//   }    
// }



#endif 
//=============================================================================
