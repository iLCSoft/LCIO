#ifndef ProcessFlag_h
#define ProcessFlag_h

#include <map>
#include <iostream>

namespace EVENT{
  class LCCollection ;
}


namespace UTIL{

  /// enum class for flag bits in ProcessFlag
  enum struct ProcessorFlagBits{
    electrons,   ///< electrons in final state
    muons,       ///< muons in final state
    taus,        ///< taus in final state
    neutrinos,   ///< neutrinos in final state
    photons,     ///< photons in final state
    dquarks,     ///< dquarks in final state
    uquarks,     ///< uquarks in final state
    squarks,     ///< squarks in final state
    cquarks,     ///< cquarks in final state
    bquarks,     ///< bquarks in final state
    higgsbb,     ///< Higss to bbbar decay
    higgscc,     ///< Higss to ccbar decay
    higgsdu,     ///< Higss to ddbar or uubar decay
    higgsss,     ///< Higss to ssbar decay
    higgsgg,     ///< Higss to gg decay
    higgstautau, ///< Higss to tautau decay
    higgsmumu,   ///< Higss to mumu decay
    higgsWW,     ///< Higss to WW decay
    higgsZZ,     ///< Higss to ZZ decay
    higgsgaga,   ///< Higss to gamma gamma decay
    higgsgaZ,    ///< Higss to gamma Z decay
    higgsinv,    ///< Higss to invisible decay
    higgsother,  ///< Higss to other decay
    exotic,      ///< exotic process (SUSY etc)
    unknown      ///< unknwon process
  } ;

  /// Short name for ProcessorFlagBits
  typedef ProcessorFlagBits PF;

/** \class ProcessFlag
 *
 *  Helper class for defining the generated Monte Carlo physics process.
 *  The bits decode if certain particle types are present in the final state
 *  of the hard sub-process. Additionally, in events with a Higgs bosons, its
 *  decay is denoted, e.g. ProcessFlag::has( PF::higssbb ) is true for an
 *  event with a H->bbbar decay. See enum PF for individual flag bits.
 * 
 *  \author F.Gaede, DESY
 *  \date June 2020
 *
 */
  class ProcessFlag {

  public:

    ProcessFlag() = default ;
    ~ProcessFlag() = default ;

    ProcessFlag(const ProcessFlag& f) = default ;
    ProcessFlag& operator=(const ProcessFlag& f) = default ;

    /// construct from int 
    ProcessFlag(int flag) : _flag(flag) {}


    /** c'tor that takes a list of bit enums in an initialiser list, e.g.
     *  ProcessFlag( {PF::muons, PF::higssbb } )
     */
    ProcessFlag(const std::initializer_list<PF>& bits ) {
       for( auto bit : bits ) add(bit) ;
    }

    /// add an individual bit
    void add( PF bit ){
      _flag = _flag | ( 1 << (int) bit ) ; 
    } 

    /// add the bit for the final state particles with given PDG code - false if not known
    bool addFSParticles(int pdg){
      auto m = _mapFS.find( pdg ) ;
      if( m != _mapFS.end() ){
	add( m->second ) ;
	return true ;
      }
      return false ;
    }
    /// add the bit for the Higgs decaying to a particles with given PDG code - false if not known
    bool addHiggsDecay(int pdg){
      auto m = _mapH.find( pdg ) ;
      if( m != _mapH.end() ){
	add( m->second ) ;
	return true ;
      }
      return false ;
    }

    /// assign complete int flag
    ProcessFlag& operator=( int flag) { _flag = flag ; return *this ;}

    /// conparison - true if identical processes
    bool operator==(const ProcessFlag& f) const { return _flag == f._flag ; }

    /// true if this ProcessFlag contains everything in f
    bool contains(const ProcessFlag& f) const { return ( _flag & f._flag ) == f._flag ; }

  
    /// conversion to int
    operator int() const { return _flag ; }

    /// check if this bit is present
    bool has(PF bit) const { return (_flag & (1<< (int) bit)   ) ? true : false ; } 

  private:
    int _flag=0 ;

    std::map< int, PF > _mapFS =
    {
      { 11, PF::electrons },
      { 13, PF::muons     },
      { 15, PF::taus      },
      { 12, PF::neutrinos },
      { 14, PF::neutrinos },
      { 16, PF::neutrinos },
      { 22, PF::photons   },
      {  1, PF::dquarks   },
      {  2, PF::uquarks   },
      {  3, PF::squarks   },
      {  4, PF::cquarks   },
      {  5, PF::bquarks   }
    } ;
    std::map< int, PF > _mapH =
    {
      {  5, PF::higgsbb     },
      {  1, PF::higgsdu     },
      {  2, PF::higgsdu     },
      {  3, PF::higgsss     },
      {  4, PF::higgscc     },
      { 15, PF::higgstautau },
      { 13, PF::higgsmumu   },
      { 24, PF::higgsWW     },
      { 23, PF::higgsZZ     },
      { 22, PF::higgsgaga   },
      { 12, PF::higgsinv    },
      { 14, PF::higgsinv    },
      { 16, PF::higgsinv    },
      { 1000022, PF::higgsinv }
    } ;
 
  } ;


  inline std::ostream& operator<<(std::ostream& os, ProcessFlag flag ){

    os << std::hex << "0x" << (int) flag << std::dec << " -> |" ;

    if( flag.has( PF::electrons ) ) os << "electrons|" ;
    if( flag.has( PF::muons     ) ) os << "muons|" ;
    if( flag.has( PF::taus      ) ) os << "taus|" ;
    if( flag.has( PF::neutrinos ) ) os << "neutrinos|" ;
    if( flag.has( PF::photons   ) ) os << "photons|" ;
    if( flag.has( PF::dquarks   ) ) os << "dquarks|" ;
    if( flag.has( PF::uquarks   ) ) os << "uquarks|" ;
    if( flag.has( PF::squarks   ) ) os << "squarks|" ;
    if( flag.has( PF::cquarks   ) ) os << "cquarks|" ;
    if( flag.has( PF::bquarks   ) ) os << "bquarks|" ;
    if( flag.has( PF::higgsbb     ) ) os << "higgsbb|" ;
    if( flag.has( PF::higgsdu     ) ) os << "higgsdu|" ;
    if( flag.has( PF::higgsss     ) ) os << "higgsss|" ;
    if( flag.has( PF::higgscc     ) ) os << "higgscc|" ;
    if( flag.has( PF::higgstautau ) ) os << "higgstautau|" ;
    if( flag.has( PF::higgsmumu   ) ) os << "higgsmumu|" ;
    if( flag.has( PF::higgsWW     ) ) os << "higgsWW|" ;
    if( flag.has( PF::higgsZZ     ) ) os << "higgsZZ|" ;
    if( flag.has( PF::higgsgaga   ) ) os << "higgsgaga|" ;
    if( flag.has( PF::higgsgaZ    ) ) os << "higgsgaZ|" ;
    if( flag.has( PF::higgsinv    ) ) os << "higgsinv|" ;
    if( flag.has( PF::higgsother  ) ) os << "higgsother|" ;
    if( flag.has( PF::exotic      ) ) os << "exotic|" ;
    if( flag.has( PF::unknown     ) ) os << "unknown|" ;

    return os;
  }


  /** Helper function that decodes the MC truth process from an LCCollection with MCParticles.
   *  Assumes that the MCParticle collection has been created with a recent version of the Whizard
   *  event generator (since DBD). At most, the first maxParticles are evaluated for identifying 
   *  the hard sub-process.
   *  icase 1: Whizard 2.8.4: incoming particles of hard subprocess (i.e. after BS, ISR etc) have generatorStatus 3
   *      => use their daughters as hard final state
   *  icase 2: DBD files WITHOUT beam particles (e.g. 250 GeV DBD samples) have more than 2 PARENTLESS particles
   *      => use these as hard final state
   *  icase 3: DBD files WITH beam particles (e.g. non-Higgs 500 GeV DBD samples) have exactly 2 PARENTLESS particles
   *      => their daughters are the incoming particles of hard subprocess and the ISR and/or outgoing beam particles
   *      => select those daughters which have more than one daughter
   *      => take the daughters of these daughters of the parentless particles as hard final state
   *  else => only PF::unknown is set
   * 
   * \author F.Gaede, J.List, DESY
   * \date August 2020
   */
  ProcessFlag decodeMCTruthProcess(const EVENT::LCCollection* mcps , int maxParticle=10);

}
#endif



 











