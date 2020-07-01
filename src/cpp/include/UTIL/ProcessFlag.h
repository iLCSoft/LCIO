#ifndef ProcessFlag_h
#define ProcessFlag_h

#include <iostream>

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
    higgsdd,     ///< Higss to ddbar (or uu,ss) decay
    higgsgg,     ///< Higss to gg decay
    higgstautau, ///< Higss to tautau decay
    higgsmumu,   ///< Higss to mumu decay
    higgsWW,     ///< Higss to WW decay
    higgsZZ,     ///< Higss to ZZ decay
    higgsgaga,   ///< Higss to gamma gamma decay
    higgsgaZ,    ///< Higss to gamma Z decay
    higgsinv,    ///< Higss to invisible decay
    higgsother,  ///< Higss to other decay
    exotic       ///< exotic process (SUSY etc)
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

    /// construct from int 
    ProcessFlag(int flag) : _flag(flag) {}


    /** c'tor that takes a list of bit enums in an initialiser list, e.g.
     *  ProcessFlag( {PF::muons, PF::higss } )
     */
    ProcessFlag(const std::initializer_list<PF>& bits ) {
       for( auto bit : bits ) add(bit) ;
    }

    /// add an individual bit
    void add( PF bit ){
      _flag = _flag | ( 1 << (int) bit ) ; 
    } 

    /// assign complete int flag
    ProcessFlag& operator=( int flag) { _flag = flag ; return *this ;}

    /// conparison - true if identical processes
    bool operator==(ProcessFlag& f) { return _flag == f._flag ; }

    /// true if this ProcessFlag contains everything in f
    bool contains(ProcessFlag& f) { return ( _flag & f._flag ) == f._flag ; }

  
    /// conversion to int
    operator int() { return _flag ; }

    /// check if this bit is present
    bool has(PF bit) const { return (_flag & (1<< (int) bit)   ) ? true : false ; } 

  private:
    int _flag=0 ;
  
  } ;


  std::ostream& operator<<(std::ostream& os, ProcessFlag flag ){

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
    if( flag.has( PF::higgsdd     ) ) os << "higgsdd|" ;
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

    return os;
  }

}
#endif



 











