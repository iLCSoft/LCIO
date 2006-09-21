#include "UTIL/IndexMap.h"

////////////////////////////////////////////////////////////////////////////////
// EXP: UNDER DEVELOPMENT!!!
////////////////////////////////////////////////////////////////////////////////

using namespace EVENT;

namespace UTIL{
  
  IndexMap::IndexMap(const EVENT::LCCollection* col, const std::string& key1, const std::string& key2): _col(col), _key1(key1), _key2(key2){
    _col->getParameters().getStringVals( key1, _strvec );
    _col->getParameters().getIntVals( key2, _intvec );

    throw Exception("  IndexMap still under development  - sorry !!!!  ") ;
  }

  IndexMap::~IndexMap(){}

  int IndexMap::encode(const std::string name){
    int type=-1;
    for( unsigned int i=0; i<_strvec.size(); i++ ){
      if(_strvec[i]==name)
	type = _intvec[i];
    }
    //type wasn't found and has to be added to collection
    if(type == -1){
      type = _strvec.size()+1;
      
      //add new type to the corresponding vectors
      _strvec.push_back( name );
      _intvec.push_back( type );

      //update col->parameters()
      const_cast<LCCollection*>(_col)->parameters().setValues(_key1,_strvec);
      const_cast<LCCollection*>(_col)->parameters().setValues(_key2,_intvec);
    }
    return type;
  }
  
  const std::string& IndexMap::decode(int type) const{
    for( unsigned int i=0; i<_intvec.size(); i++ ){
      if(_intvec[i]==type)
	return _strvec[i];
    }
    //type not found in collection
    static const std::string str="Unknown type!!!";
    return str;
  }

} // namespace
 
