#ifndef RIOLCCollectionHandler_h
#define RIOLCCollectionHandler_h 1

#include "lcio.h"
#include "EVENT/LCEvent.h"

#include "IMPL/LCCollectionVec.h"

#include "UTIL/LCIOTypeInfo.h" //fixme: LCIOTypeInfo should not be in UTIL !?

#include "TTree.h"
#include <vector>


namespace RIO{
  /* Abstract interface for handling collections of different types.
  **/
  
  class RIOBranchHandler{
    
  public: 
    
    virtual void toBranch( const EVENT::LCEvent* evt )=0 ;
    
    virtual void fromBranch( EVENT::LCEvent* evt , int entryID )=0 ;
    
  } ;
  
  
  
  /** Branch handler class for (typed) LCCollections.
   */
  
  //  template <typename T>
  class RIOLCCollectionHandler : public RIOBranchHandler{
    
  protected:
    
    std::string _name ;
    std::string _type ;
    
    //     std::vector<T*>* _tv  ;
    //     IMPL::LCParametersImpl* _params ;
    
    TBranch* _br ;
    IMPL::LCCollectionVec* _tv ;
    
    
  public: 
    
    /** C'tor.
     */
    RIOLCCollectionHandler(const std::string& name, const std::string& type,  TTree* tree ) : 
      _name(name),
      _type(type) {
      
      std::cout << "  RIOLCCollectionHandler( " << name << ", " << tree << ")" << std::endl ;
      
      //      _tv = 0 ; //IMPL::LCCollectionVec ; // new std::vector<T*> ;      
      _tv = new IMPL::LCCollectionVec( type  )  ;
      
      _br = (TBranch*) tree->GetBranch( _name.c_str() ) ;
      
      if( _br != 0 ){  // branch allready exists -> update/append  mode 
	
	_br->SetAddress( &_tv ) ;
	
      } else {
	
	//FIXME: make split level and 'record size' parameters ....
	tree->Branch( _name.c_str()  , &_tv, 16000, 2 );
      }
      
      
      //       _params = 0 ; //new IMPL::LCParametersImpl ;
      
      //       std::string pName(name) ; 
      
      //       pName += "_param" ;
      
      //       br = (TBranch*) tree->GetBranch( pName.c_str() ) ;
      
      //       if( br != 0 ){  // branch allready exists -> update/append  mode 
      
      // 	br->SetAddress( &_params ) ;
      
      //       } else {
      
      // 	//FIXME: make split level and 'record size' parameters ....
      // 	tree->Branch( pName.c_str()  , &_params, 16000, 2 );
      //       }
      

    }
    
    

    virtual void toBranch( const EVENT::LCEvent* evt ){
      
//       _tv->clear() ;
      
//       try{
	
// 	EVENT::LCCollection* col =  evt->getCollection( _name ) ;
	
// 	int n = col->getNumberOfElements() ;
	
// 	_tv->resize( n ) ;
	
// 	for( int i=0; i<n ; ++i){
	  
// 	  (*_tv)[i] = dynamic_cast<T*>( col->getElementAt(i) )  ;
// 	}
	

// 	IMPL::LCParametersImpl* lcp =  dynamic_cast<IMPL::LCParametersImpl*> ( & col->parameters() ) ; 
// 	_params = lcp ;


// 	//FIXME: we also need to add the collection flags to the meta data branch ...


//       } catch(const lcio::DataNotAvailableException& e){

// 	_tv->resize( 0 ) ;
	
// 	std::cout << "  collection not found : " << _name << std::endl ;
//       }    

      try{
	
	EVENT::LCCollection* col =  evt->getCollection( _name ) ;
	
	_tv = dynamic_cast<IMPL::LCCollectionVec*> ( col ) ;
	
      } catch(const lcio::DataNotAvailableException& e){

	_tv = new IMPL::LCCollectionVec( _type ) ;
	
	std::cout << "  collection not found : " << _name << std::endl ;
      }    
      
      
    }
    
    
    virtual void fromBranch( EVENT::LCEvent* evt , int entryID){
      
//       // read branch (if requested) from file and add collection to the event
// // 	IMPL::LCParametersImpl* lcp =  dynamic_cast<IMPL::LCParametersImpl*> ( & col->parameters() ) ; 
// // 	_params = lcp ;
	
// 	IMPL::LCCollectionVec* col = new IMPL::LCCollectionVec( UTIL::lctypename<T>()  ) ;
	
// 	int n = _tv->size() ;

// 	col->resize( n ) ;


// 	for(int i=0 ; i < n ; ++i ) {

// 	  col->operator[](i) = _tv->operator[](i) ;
// 	}


//        	//	col->parameters() = _params ;

// 	evt->addCollection( col ,  _name ) ;


      int nbyte = _br->GetEntry( entryID );
     

      std::cout << "  read " << nbyte << " bytes from branch : " << _name << " of type " << _type << std::endl ;

      try{
	
	evt->addCollection( _tv ,  _name ) ;
	
      } catch(const lcio::EventException& e){
	// collection allready added
      }    
      
    }
      
    virtual ~RIOLCCollectionHandler() { 
      
      delete _tv ;
    }
    
    
    
  }; // class
  

} // end namespace

#endif 
//=============================================================================
