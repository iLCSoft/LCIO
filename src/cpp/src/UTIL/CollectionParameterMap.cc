#include "UTIL/CollectionParameterMap.h"
#include "EVENT/LCParameters.h"
#include<sstream>

#include <iostream>


namespace UTIL{

  CollectionParameterMap::CollectionParameterMap( const std::string& keyName ,  
						  const std::string& valueName,  
						  LCCollection* col ) :
    _keyName( keyName ) ,
    _valueName( valueName ) ,
    _col( col ) {

    init( col ) ;

  }

  CollectionParameterMap::CollectionParameterMap( const std::string& keyName ,  
						  const std::string& valueName,  
						  const LCCollection* col ) :
    _keyName( keyName ) ,
    _valueName( valueName ) ,
    _col( 0 ) { // if const we don't update

    init( col ) ;
  }

  void CollectionParameterMap::init( const LCCollection* col ){
    
    StringVec keys ;
    
    col->getParameters().getStringVals( _keyName , keys ) ;
    
    IntVec values ;
    
    col->getParameters().getIntVals( _valueName , values ) ;
    
    
    // ------ do some checks: -------------------------
    if( keys.size() != values.size() ) {
      
      std::stringstream sstr ;
      
      sstr << " CollectionParameterMap::init() - inconsistent parameter sizes for "
	   <<  _keyName  << " [" << keys.size() << "] and " 
	   <<  _valueName << " [" <<  values.size() << "] " ;
      
      throw Exception( sstr.str() ) ;
    }
    
    unsigned nKeys =  keys.size() ;
    for(unsigned i=0;i<nKeys;++i){
      
      _map.insert( std::make_pair( keys[i] , values[i] ) ) ;
    }
    
  }


  CollectionParameterMap::~CollectionParameterMap() {


    if( _col != 0 ){
      
      StringVec keys ;
      IntVec values ;
      
      for(map_type::iterator it= _map.begin() ; it!=_map.end() ;++it){
	
	keys.push_back( it->first ) ;
	values.push_back( it->second ) ;
	
	//       std::cout << "   CollectionParameterMap::~CollectionParameterMap  - " 
	// 		<<    it->first << " mapped to : " << it->second
	// 		<< std::endl ;
      }
      
      _col->parameters().setValues( _keyName , keys ) ;
      _col->parameters().setValues( _valueName , values ) ;
      
      
      //       std::cout << "   CollectionParameterMap::~CollectionParameterMap  - " 
      // 		<< " setValues(  " <<  _keyName << " ,...) and " << keys.size()
      // 		<< " setValues(  " <<  _valueName << " ,...) "<< values.size()
      // 		<< std::endl ;
    }
    
  }




}
