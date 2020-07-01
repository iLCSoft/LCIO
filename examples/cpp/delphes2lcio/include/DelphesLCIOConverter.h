#ifndef DelphesLCIOConverter_h
#define DelphesLCIOConverter_h


#include <string>
#include <map>
#include <functional>

class TTree;
class TClonesArray;
class TObject;

namespace IO{ class LCWriter ; }
namespace EVENT{
  class LCEvent ;
  class MCParticle;
  class ReconstructedParticle;
  class LCCollection;
}
namespace IMPL{
  class LCEventImpl ;
  class LCCollectionVec;
}

namespace UTIL{
  class LCRelationNavigator;
}

class DelphesLCIOConfig ;


/** \class DelphesLCIOConverter
 *
 *  Class handling output of ROOT tree in LCIO
 *
 *  \author F.Gaede, DESY
 *  \date June 2020
 *
 */
class DelphesLCIOConverter {
public:

  ///c'tor intializes an LCWriter if valid non-empty file name given
  DelphesLCIOConverter(const char* fileName = "" );

  /// d'tor
  ~DelphesLCIOConverter();

  /// read the configuration file with parameter maps containing collection and branch names etc.
  void readConfigFile( const char* fileName ) ;

  /// convert the Delphes tree to LCIO collections and add them to the event
  void convertTree2LCIO( TTree *tree , IMPL::LCEventImpl* evt ) ;

  /// create a new LCIO event and write it to the writer (if valid)
  void writeEvent( TTree* tree ) ;


  /** Helper function to convert convert a (isolated) particle reference collection. Assumes that the referenced
   *  ReconstructedParticles (PFOs) have been created and are in the relation nafigator. If pdg is given the PFO
   *  type is set to pdg*charge.
   */
  bool convertPFORefCollection( TClonesArray* tca, EVENT::LCCollection* col, UTIL::LCRelationNavigator& mcrecNav,
			       std::function<unsigned(TObject*)> uid, int pdg=-99 ) ;


  /** Helper function to convert convert a jet collection. Assumes that the referenced
   *  ReconstructedParticles (PFOs) have been created beforehand. If useDelphes4Vec != 0  the four vector from
   *  the Delphes jet is used otherwise it is computed from the constituent PFOs.
   */
  bool convertJetCollection( TClonesArray* tca, EVENT::LCCollection* col, int useDelphes4Vec, int storeYMerge ) ;

private:
  IO::LCWriter *_writer=nullptr;
  IMPL::LCCollectionVec *_evtSumCol=nullptr;

  std::map< unsigned, EVENT::MCParticle*> _mcpd2lmap ;
  std::map< unsigned, EVENT::ReconstructedParticle*> _recd2lmap ;

  DelphesLCIOConfig* _cfg ;

};

#endif /* DelphesLCIOConverter */
