
#include "DelphesLCIOConfig.h"

#include <fstream>
#include <sstream>
#include <iostream>

/// return list of map names for extra jet collection (containing substring 'ExtraJet' )
std::vector<std::string> DelphesLCIOConfig::getExtraJetMapNames() const {

  std::vector<std::string> ejmapNames ;

  for( auto& it : _maps ){

    if( ( it.first.find( "ExtraJet" ) != std::string::npos) )
      ejmapNames.push_back( it.first ) ;
  }
  return ejmapNames ;
}

/// return list of map names for extra PFO collection (containing substring 'ExtraPFO' )
std::vector<std::string> DelphesLCIOConfig::getExtraPFOMapNames() const {

  std::vector<std::string> ejmapNames ;

  for( auto& it : _maps ){

    if( ( it.first.find( "ExtraPFO" ) != std::string::npos) )
      ejmapNames.push_back( it.first ) ;
  }
  return ejmapNames ;
}

// ---------------------------------------
/// convert string to int
int DelphesLCIOConfig::toInt(const std::string& val) const {
  int i;
  std::stringstream s(val) ;  s >> i  ;
  if( s.fail() )
    throw std::runtime_error( std::string("\nDelphesLCIOConfig: cannot convert to int:  ") + val ) ;
  return i;
}

/// convert string to float
float DelphesLCIOConfig::toFloat(const std::string& val) const {
  float f;
  std::stringstream s(val) ; s >> f  ;
  if( s.fail() )
    throw std::runtime_error( std::string("\nDelphesLCIOConfig: cannot convert to float:  ") + val ) ;

  return f;
}
// ---------------------------------------

std::string DelphesLCIOConfig::getMapParameter(const std::string& key, const std::string& mapName ) const {

  auto mit = _maps.find( mapName )  ;

  if( mit == _maps.end() )
    throw std::runtime_error( std::string("\nDelphesLCIOConfig: no configuration map found with name: ")
			      + mapName ) ;

  const ConfMap& m = mit->second ; 
    
  auto it = m.find( key ) ;

  if( it == m.end() )
    throw std::runtime_error( std::string("\nDelphesLCIOConfig: key : ") + key
			      +  std::string(" not found in map: ") + mapName ) ;

  return it->second ;
}
// ---------------------------------------

std::string DelphesLCIOConfig::getMapParameterSave(const std::string& key, const ConfMap& m) const {

  auto it = m.find( key ) ;

  return ( it != m.end() ? it->second : std::string("")  ) ;
}

// ---------------------------------------

DelphesLCIOConfig::FloatMap DelphesLCIOConfig::getFloatMap(const std::string& mapName) const {

  FloatMap fmap ;

  auto mit = _maps.find( mapName )  ;

  if( mit != _maps.end() ){

    for(auto it : mit->second ){

      fmap[ it.first ] = toFloat( it.second ) ;
    }
  }

  return fmap ;
}


DelphesLCIOConfig::IntMap DelphesLCIOConfig::getIntMap(const std::string& mapName) const {

  IntMap imap ;

  auto mit = _maps.find( mapName )  ;

  if( mit != _maps.end() ){

    for(auto it : mit->second ){

      imap[ it.first ] = toInt( it.second ) ;
    }
  }

  return imap ;

}


DelphesLCIOConfig::StringMap DelphesLCIOConfig::getStringMap(const std::string& mapName) const {

  StringMap smap ;

  auto mit = _maps.find( mapName )  ;

  if( mit != _maps.end() ){

    for(auto it : mit->second ){

      smap[ it.first ] = it.second ;
    }
  }

  return smap ;

}

// ---------------------------------------

std::string DelphesLCIOConfig::toString() const {

  std::stringstream s ;
  s << "\n# ------- DelphesLCIOConfig parameter maps : --------------------- \n\n" ;
  for( const auto& it : _maps ){
    s << it.first << "\n" ;
    const auto& m = it.second ;
    for( auto& kv : m ){
      s << kv.first << "  " << kv.second  << "\n" ;
    }
    s << "# --------------------------------" << "\n" ;
  }
  return s.str() ;
}


// ---------------------------------------

void DelphesLCIOConfig::readConfigFile(const std::string& fileName ) {

  std::cout << " DelphesLCIOConfig read new configuration from file: " << fileName << std::endl ;

  std::ifstream in(fileName);

  if(!in){
    std::cerr << "DelphesLCIOConfig: cannot open the File : " << fileName <<std::endl;
    exit(1) ;
  }


  //std::cout << " --- old config : " << toString() << std::endl ;

  _maps.clear() ;


  std::string line ;
  std::string mapName ;

  ConfMap cmap ;
  
  while (std::getline(in, line)) {

    // ignore empty lines and commented lines
    if( line.empty() || line[0] == '#' )
      continue ;

    // split line on white space
    std::vector<std::string> strings ;
    std::istringstream iss(line);
    for(std::string s; iss >> s; )
      strings.push_back(s);

    if( strings.size() == 1 ){ // new config map

      if( !cmap.empty() && !mapName.empty() )
	_maps[ mapName ] = cmap ;

      cmap.clear() ;
      mapName = strings[0] ;

    } else if(strings.size() == 2 ){ // new map entry 

      cmap[ strings[0] ] = strings[1] ;

    } else {
      std::cout << " wrong number of strings on line " << std::endl ;
      for(auto s : strings )
	std::cout << "   " << s << " | " ;
      std::cout << "\n" ;
    }
  }

  if( !cmap.empty() && !mapName.empty() )
    _maps[ mapName ] = cmap ;
  
  //std::cout << " --- new config : " << toString() << std::endl ;

  in.close();
}
// ---------------------------------------
