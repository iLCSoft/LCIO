#ifndef DelphesLCIOConfig_h
#define DelphesLCIOConfig_h


#include <map>
#include <string>
#include <vector>


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
  

  /// return parameter for default MCParticle collection
  std::string getMCPParameter(const std::string& key)      const { return getMapParameter( key , "MCParticleMap" ) ; }

  /// return parameter for default PFO collection
  std::string getPFOParameter(const std::string& key)      const { return getMapParameter( key , "PFOMap" ) ; }

  /// return parameter for default Jet collection
  std::string getJetParameter(const std::string& key)      const { return getMapParameter( key , "JetMap" ) ; }

  /// return parameter for default Muon collection
  std::string getMuonParameter(const std::string& key)     const { return getMapParameter( key , "MuonMap" ) ; }

  /// return parameter for default Photon collection
  std::string getPhotonParameter(const std::string& key)   const { return getMapParameter( key , "PhotonMap" ) ; }

  /// return parameter for default Electron collection
  std::string getElectronParameter(const std::string& key) const { return getMapParameter( key , "ElectronMap" ) ; }

  /// return list of map names for extra jet collection (containing substring 'ExtraJet' )
  std::vector<std::string> getExtraJetMapNames() const ;

  /// convert string to int
  int toInt(const std::string& val) const ;

  /// convert string to float
  int toFloat(const std::string& val) const ;

  /// return the parameter for the given key from the named map - throws if either does not exist
  std::string getMapParameter(const std::string& key, const std::string& mapName ) const ;


  /// return a string with the configuration parameter maps
  std::string toString() const ;

  /// read a new configuration from the given file
  void readConfigFile(const std::string& fileName ) ;

  std::string getMapParameterSave(const std::string& key, const ConfMap& m) const ;
  //===============================================================

private:


  std::map< std::string, ConfMap > _maps =
  {
    // ---- default collections for mini-DST

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
	{ "pdgCharged"        , "211" },
	{ "massCharged"       , "0.1395701835" },
	{ "branchNameNHadron" , "EFlowNeutralHadron" },
	{ "pdgNHadron"        , "2112" },
	{ "massNHadron"       , "0.9395654133" },
	{ "branchNamePhoton"  , "EFlowPhoton" }
      }
    },

    { "JetMap" ,
      {
	{ "lcioName"       , "Jets" },
	{ "branchName"     , "Jet" },
	{ "useDelphes4Vec" , "0" }
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
    },

    // ----- additional jet collections ----------------

    { "ExtraJetMap2" , { { "lcioName", "Durham2Jets" }, { "branchName", "Jet_N2" }, { "useDelphes4Vec" , "0" }, { "storeYMerge" , "1" } } },
    { "ExtraJetMap3" , { { "lcioName", "Durham3Jets" }, { "branchName", "Jet_N3" }, { "useDelphes4Vec" , "0" }, { "storeYMerge" , "0" } } },
    { "ExtraJetMap4" , { { "lcioName", "Durham4Jets" }, { "branchName", "Jet_N4" }, { "useDelphes4Vec" , "0" }, { "storeYMerge" , "0" } } },
    { "ExtraJetMap5" , { { "lcioName", "Durham5Jets" }, { "branchName", "Jet_N5" }, { "useDelphes4Vec" , "0" }, { "storeYMerge" , "0" } } },
    { "ExtraJetMap6" , { { "lcioName", "Durham6Jets" }, { "branchName", "Jet_N6" }, { "useDelphes4Vec" , "0" }, { "storeYMerge" , "0" } } }


  } ;

};

#endif
