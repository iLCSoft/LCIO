#include "IMPL/LCParametersImpl.h"

#include "LCIOSTLTypes.h"
#include <algorithm>

using namespace EVENT ;

namespace IMPL{

  LCParametersImpl::LCParametersImpl(){
    
  }

  int LCParametersImpl::getIntVal(const std::string & key) const {
    
    IntMap::iterator it = _intMap.find( key ) ;

    if( it == _intMap.end() )  return 0 ;

    IntVec &  iv =  it->second ;

    return iv[0] ;
  }

  float LCParametersImpl::getFloatVal(const std::string & key) const {

    FloatMap::iterator it = _floatMap.find( key ) ;

    if( it == _floatMap.end() )  return 0 ;

    FloatVec &  fv =  it->second ;

    return fv[0] ;
  }

  const std::string & LCParametersImpl::getStringVal(const std::string & key) const {

    static std::string empty("") ;
    
    StringMap::iterator it = _stringMap.find( key ) ;
    
    if( it == _stringMap.end() )  return empty ;
    
    StringVec &  sv =  it->second ;
    
    return sv[0] ;
  }

  IntVec & LCParametersImpl::getIntVals(const std::string & key, IntVec & values) const {

    IntMap::iterator it = _intMap.find( key ) ;

    if( it != _intMap.end() ) {
      values.insert( values.end() , it->second.begin() , it->second.end() ) ;
    }

    return values ;
  }

  FloatVec & LCParametersImpl::getFloatVals(const std::string & key, FloatVec & values) const {

    FloatMap::iterator it = _floatMap.find( key ) ;

    if( it != _floatMap.end() ) {
      values.insert( values.end() , it->second.begin() , it->second.end() ) ;
    }
    return values ;
  }

  StringVec & LCParametersImpl::getStringVals(const std::string & key, StringVec & values) const {

    StringMap::iterator it = _stringMap.find( key ) ;

    if( it != _stringMap.end() ) {
      values.insert( values.end() , it->second.begin() , it->second.end() ) ;
    }
    return values ;
  }


  const StringVec & LCParametersImpl::getIntKeys(StringVec & keys) const  {

     for( IntMap::iterator iter = _intMap.begin() ; iter !=  _intMap.end() ; iter++ ){
       keys.push_back( iter->first ) ; 
     }
// fg: select1st is non-standard 
//    transform( _intMap.begin() , _intMap.end() , back_inserter( keys )  , select1st< IntMap::value_type >() ) ;

  return keys ;
  }

  const StringVec & LCParametersImpl::getFloatKeys(StringVec & keys) const  {
    
     for( FloatMap::iterator iter = _floatMap.begin() ; iter !=  _floatMap.end() ; iter++ ){
       keys.push_back( iter->first ) ; 
     }
// fg: select1st is non-standard
//    transform( _floatMap.begin() , _floatMap.end() , back_inserter( keys )  , select1st< FloatMap::value_type >() ) ;
    return keys ;
  }

  const StringVec & LCParametersImpl::getStringKeys(StringVec & keys) const  {

    for( StringMap::iterator iter = _stringMap.begin() ; iter !=  _stringMap.end() ; iter++ ){
      keys.push_back( iter->first ) ; 
    }
// fg: select1st is non-standard
//    transform( _stringMap.begin() , _stringMap.end() , back_inserter( keys )  , select1st< StringMap::value_type >() ) ;
    return keys ;
  }
  
  int LCParametersImpl::getNInt(const std::string & key) const {

    IntMap::iterator it = _intMap.find( key ) ;

    if( it == _intMap.end() )
      return 0 ;
    else
      return it->second.size() ;
  }

  int LCParametersImpl::getNFloat(const std::string & key) const {

    FloatMap::iterator it = _floatMap.find( key ) ;

    if( it == _floatMap.end() )  
      return 0 ;
    else
      return it->second.size() ;
  }

  int LCParametersImpl::getNString(const std::string & key) const {

    StringMap::iterator it = _stringMap.find( key ) ;

    if( it == _stringMap.end() )  
      return 0 ;
    else
      return it->second.size() ;
  }

  void LCParametersImpl::setValue(const std::string & key, int value){
    checkAccess("LCParametersImpl::setValue") ;
//     if(  _intMap[ key ].size() > 0 ) 
    _intMap[ key ].clear() ;
    _intMap[ key ].push_back( value ) ;
  }

  void LCParametersImpl::setValue(const std::string & key, float value){
    checkAccess("LCParametersImpl::setValue") ;
//     if(  _floatMap[ key ].size() > 0 ) 
    _floatMap[ key ].clear() ;
    _floatMap[ key ].push_back( value ) ;
  }

  void LCParametersImpl::setValue(const std::string & key, const std::string & value) {
    checkAccess("LCParametersImpl::setValue") ;
//     if(  _stringMap[ key ].size() > 0 ) 
    _stringMap[ key ].clear() ;
    _stringMap[ key ].push_back( value ) ;

  }



  void LCParametersImpl::setValues(const std::string & key,const EVENT::IntVec & values){
    
    checkAccess("LCParametersImpl::setValues") ;

//     if(  _intMap[ key ].size() > 0 ) _intMap[ key ].clear() ;
//     copy( values.begin() , values.end() , back_inserter(  _intMap[ key ] )  ) ;

    _intMap[ key ].assign(  values.begin() , values.end() ) ;
  }
  
  void LCParametersImpl::setValues(const std::string & key,const  EVENT::FloatVec & values){

    checkAccess("LCParametersImpl::setValues") ;

//     if(  _floatMap[ key ].size() > 0 ) _floatMap[ key ].clear() ;
//     copy( values.begin() , values.end() , back_inserter(  _floatMap[ key ] )  ) ;

    _floatMap[ key ].assign(  values.begin() , values.end() ) ;
  }
  
  void LCParametersImpl::setValues(const std::string & key, const EVENT::StringVec & values){

    checkAccess("LCParametersImpl::setValues") ;

//     if(  _stringMap[ key ].size() > 0 ) _stringMap[ key ].clear() ;
//     copy( values.begin() , values.end() , back_inserter(  _stringMap[ key ] )  ) ;

    _stringMap[ key ].assign(  values.begin() , values.end() ) ;
  }

} // namespace 
