#ifndef DelphesLCIOConfig_h
#define DelphesLCIOConfig_h


#include <map>
#include <string>
#include <sstream>


typedef std::map< std::string, std::string > ConfMap ;



/** \class DelphesLCIOConfig
 *
 *  Configuration helper class for Delphes to LCIO conversion
 *
 *  \author F.Gaede, DESY
 *  \date June 2020
 *
 */

class DelphesLCIOConfig{

public:
  
  std::string getMCPParameter(const std::string& key)      const { return getValue( key , "MCParticleMap" ) ; }
  std::string getPFOParameter(const std::string& key)      const { return getValue( key , "PFOMap" ) ; }
  std::string getJetParameter(const std::string& key)      const { return getValue( key , "JetMap" ) ; }
  std::string getMuonParameter(const std::string& key)     const { return getValue( key , "MuonMap" ) ; }
  std::string getPhotonParameter(const std::string& key)   const { return getValue( key , "PhotonMap" ) ; }
  std::string getElectronParameter(const std::string& key) const { return getValue( key , "ElectronMap" ) ; }


  int toInt(const std::string& val){
    std::stringstream s(val) ;
    int i;
    s >> i  ;
    return i;
  }
  int toFloat(const std::string& val){
    std::stringstream s(val) ;
    float f;
    s >> f  ;
    return f;
  }

private:
  
  std::string getValue(const std::string& key, const std::string& mapName ) const {

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

  std::string getValueSave(const std::string& key, const ConfMap& m) const {

    auto it = m.find( key ) ;

    return ( it != m.end() ? it->second : std::string("")  ) ;
  }

  //===============================================================

  ConfMap _mcpMap = {
    { "lcioName"   , "MCParticle" },
    { "branchName" , "Particle" }
  }; 


  ConfMap _pfoMap = {
    { "lcioName"          , "PFOs" },
    { "branchNameCharged" , "EFlowTrack" },
    { "branchNameNHad"    , "EFlowNeutralHadron" },
    { "branchNamePhoton"  , "EFlowPhoton" },
  }; 

  ConfMap _jetMap ;
  ConfMap _muonMap ;
  ConfMap _photonMap ;
  ConfMap _electronMap ;

  //===============================================================

  std::map< std::string, ConfMap > _maps =
  {
    { "MCParticleMap" ,
      {
	{ "lcioName"   , "MCParticle" },
	{ "branchName" , "Particle" }
      }   
    },

    { "PFOMap" ,
      {
	{ "lcioName"          , "PFOs" },
	{ "branchNameCharged" , "EFlowTrack" },
	{ "branchNameNHadron" , "EFlowNeutralHadron" },
	{ "branchNamePhoton"  , "EFlowPhoton" }
      }
    },

    { "JetMap" ,
      {
	{ "lcioName"   , "Jets" },
	{ "branchName" , "Jet" }
      }   
    },

    { "MuonMap" ,
      {
	{ "lcioName"   , "Muons" },
	{ "branchName" , "Muon" },
	{ "pdg" , "-13" }
      }   
    },

    { "ElectronMap" ,
      {
	{ "lcioName"   , "Electrons" },
	{ "branchName" , "Electron" },
	{ "pdg" , "-11" }
      }   
    },

    { "PhotonMap" ,
      {
	{ "lcioName"   , "Photons" },
	{ "branchName" , "Photon" },
	{ "pdg" , "22" }
      }   
    }
  } ;

};

#endif
