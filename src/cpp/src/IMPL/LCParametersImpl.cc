#include "IMPL/LCParametersImpl.h"

#include "LCIOSTLTypes.h"
#include <algorithm>

using namespace EVENT ;

namespace IMPL{

  LCParametersImpl::LCParametersImpl(){
    
  }

  int LCParametersImpl::getIntVal(const std::string & key) const {
    
    IntVec &  iv =  _intMap[ key ] ;

    if( iv.size() == 0 )  return 0 ;

    return iv[0] ;
  }

  float LCParametersImpl::getFloatVal(const std::string & key) const {

    FloatVec &  fv =  _floatMap[ key ] ;

    if( fv.size() == 0 )  return 0 ;

    return fv[0] ;
  }

  const std::string & LCParametersImpl::getStringVal(const std::string & key) const {

    static std::string empty("") ;

    StringVec &  sv =  _stringMap[ key ] ;

    if( sv.size() == 0 )  return empty  ;

    return sv[0] ;
  }

  IntVec & LCParametersImpl::getIntVals(const std::string & key, IntVec & values) const {

    //    copy( _intMap[ key ].begin() , _intMap[ key ].end() , back_inserter( values )  ) ;
    IntVec& v =  _intMap[ key ] ;
    values.insert( values.end() , v.begin() , v.end() ) ;
    return values ;
  }

  FloatVec & LCParametersImpl::getFloatVals(const std::string & key, FloatVec & values) const {

    //    copy( _floatMap[ key ].begin() , _floatMap[ key ].end() , back_inserter( values )  ) ;
    FloatVec& v =  _floatMap[ key ] ;
    values.insert( values.end() , v.begin() , v.end() ) ;

    return values ;
  }

  StringVec & LCParametersImpl::getStringVals(const std::string & key, StringVec & values) const {

    //    copy( _stringMap[ key ].begin() , _stringMap[ key ].end() , back_inserter( values )  ) ;
    StringVec& v =  _stringMap[ key ] ;
    values.insert( values.end() , v.begin() , v.end() ) ;

    return values ;
  }


  const StringVec & LCParametersImpl::getIntKeys(StringVec & keys) const  {

     for( IntMap::iterator iter = _intMap.begin() ; iter !=  _intMap.end() ; iter++ ){
       keys.push_back( iter->first ) ; 
     }
// fg: select1st is non-standard but widely available - use code above on machines where it is missing
//    transform( _intMap.begin() , _intMap.end() , back_inserter( keys )  , select1st< IntMap::value_type >() ) ;

  return keys ;
  }

  const StringVec & LCParametersImpl::getFloatKeys(StringVec & keys) const  {
    
     for( FloatMap::iterator iter = _floatMap.begin() ; iter !=  _floatMap.end() ; iter++ ){
       keys.push_back( iter->first ) ; 
     }
// fg: select1st is non-standard but widely available - use code above on machines where it is missing
//    transform( _floatMap.begin() , _floatMap.end() , back_inserter( keys )  , select1st< FloatMap::value_type >() ) ;
    return keys ;
  }

  const StringVec & LCParametersImpl::getStringKeys(StringVec & keys) const  {

    for( StringMap::iterator iter = _stringMap.begin() ; iter !=  _stringMap.end() ; iter++ ){
      keys.push_back( iter->first ) ; 
    }
// fg: select1st is non-standard but widely available - use code above on machines where it is missing
//    transform( _stringMap.begin() , _stringMap.end() , back_inserter( keys )  , select1st< StringMap::value_type >() ) ;
    return keys ;
  }
  
  int LCParametersImpl::getNInt(const std::string & key) const {
    return _intMap[ key ].size() ;
  }

  int LCParametersImpl::getNFloat(const std::string & key) const {
    return _floatMap[ key ].size() ;
  }

  int LCParametersImpl::getNString(const std::string & key) const {
    return _stringMap[ key ].size() ;
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



  void LCParametersImpl::setValues(const std::string & key, EVENT::IntVec & values){
    
    checkAccess("LCParametersImpl::setValues") ;

//     if(  _intMap[ key ].size() > 0 ) _intMap[ key ].clear() ;
//     copy( values.begin() , values.end() , back_inserter(  _intMap[ key ] )  ) ;

    _intMap[ key ].assign(  values.begin() , values.end() ) ;
  }
  
  void LCParametersImpl::setValues(const std::string & key, EVENT::FloatVec & values){

    checkAccess("LCParametersImpl::setValues") ;

//     if(  _floatMap[ key ].size() > 0 ) _floatMap[ key ].clear() ;
//     copy( values.begin() , values.end() , back_inserter(  _floatMap[ key ] )  ) ;

    _floatMap[ key ].assign(  values.begin() , values.end() ) ;
  }
  
  void LCParametersImpl::setValues(const std::string & key, EVENT::StringVec & values){

    checkAccess("LCParametersImpl::setValues") ;

//     if(  _stringMap[ key ].size() > 0 ) _stringMap[ key ].clear() ;
//     copy( values.begin() , values.end() , back_inserter(  _stringMap[ key ] )  ) ;

    _stringMap[ key ].assign(  values.begin() , values.end() ) ;
  }

} ; // namespace 
