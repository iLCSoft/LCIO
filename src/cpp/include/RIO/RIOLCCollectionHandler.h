#ifndef RIOLCCollectionHandler_h
#define RIOLCCollectionHandler_h 1

#include "lcio.h"
#include "EVENT/LCEvent.h"

#include "IMPL/LCCollectionVec.h"
#include "UTIL/LCTypedVector.h"

#include "TTree.h"


namespace RIO{
  /* Abstract interface for handling collections of different types.
  **/
  
  class RIOBranchHandler{
    
  public: 
    
    virtual void toBranch( const EVENT::LCEvent* evt )=0 ;
    
    virtual void fromBranch( EVENT::LCEvent* evt )=0 ;
    
  } ;
  
  
  
  /** Branch handler class for (typed) LCCollections.
   */
  
  template <typename T>
  class RIOLCCollectionHandler : public RIOBranchHandler{
    
  protected:
    
    std::string _name ;
    std::vector<T*>* _tv  ;
    IMPL::LCParametersImpl* _params ;
    
  public: 
    
    /** C'tor.
     */
    RIOLCCollectionHandler(const char* name,  TTree* tree ) : 

      _name(name) {
      
       std::cout << "  RIOLCCollectionHandler( " << name << ", " << tree << ")" << std::endl ;

      _tv = new std::vector<T*> ;      

      TBranch* br = (TBranch*) tree->GetBranch( name ) ;
      
      if( br != 0 ){  // branch allready exists -> update/append  mode 
	
	br->SetAddress( &_tv ) ;
	
      } else {
	
	//FIXME: make split level and 'record size' parameters ....
	tree->Branch( name  , &_tv, 16000, 2 );
      }
      

      _params = 0 ; //new IMPL::LCParametersImpl ;
      
      std::string pName(name) ; 

      pName += "_param" ;

      br = (TBranch*) tree->GetBranch( pName.c_str() ) ;
      
      if( br != 0 ){  // branch allready exists -> update/append  mode 
	
	br->SetAddress( &_params ) ;
	
      } else {
	
	//FIXME: make split level and 'record size' parameters ....
	tree->Branch( pName.c_str()  , &_params, 16000, 2 );
      }
      






    }
    
    

    virtual void toBranch( const EVENT::LCEvent* evt ){
      
      _tv->clear() ;
      
      try{
	
	EVENT::LCCollection* col =  evt->getCollection( _name ) ;
	
	int n = col->getNumberOfElements() ;
	
	_tv->resize( n ) ;
	
	for( int i=0; i<n ; ++i){
	  
	  (*_tv)[i] = dynamic_cast<T*>( col->getElementAt(i) )  ;
	}
	

	IMPL::LCParametersImpl* lcp =  dynamic_cast<IMPL::LCParametersImpl*> ( & col->parameters() ) ; 
	_params = lcp ;


	//FIXME: we also need to add the collection flags to the meta data branch ...


      } catch(const lcio::DataNotAvailableException& e){

	_tv->resize( 0 ) ;
	
	std::cout << "  collection not found : " << _name << std::endl ;
      }    
      
      
      //    std::cout << "  size of _tv " << _tv->size()  << " for " << _name  << std::endl ;
      
    }
    
    
    virtual void fromBranch( EVENT::LCEvent* evt ){
      
      // read branch (if requested) from file and add collection to the event
    }    
    
    
    
    virtual ~RIOLCCollectionHandler() { 
      
      delete _tv ;
    }
    
    
    
  }; // class
  

} // end namespace

#endif 
//=============================================================================
