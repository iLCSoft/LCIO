#ifndef ProcessFlag_h
#define ProcessFlag_h

#include <iostream>

namespace UTIL{

/// helper enum for flag bits
  enum struct PF{
    electrons=0, 
    muons,
    taus,
    neutrinos,
    photons,
    dquarks,
    uquarks,
    squarks,
    cquarks,
    bquarks,
    higgsbb,
    higgscc,
    higgsdd,
    higgsgg,
    higgstautau,
    higgsmumu,
    higgsWW,
    higgsZZ,
    higgsgaga,
    higgsgaZ,
    higgsinv,
    higgsother,
    exotic
  } ;


/** \class ProcessFlag
 *
 *  Helper class for defining the generated Monte Carlo physics process.
 *  The bits decode if certain particle types are present in the final state
 *  of the hard sub-process. Additionally, in events with a Higgs bosons, its
 *  decay is denoted, e.g. ProcessFlag::has( PF::higssbb ) is true for an
 *  event with a H->bbbar decay. 
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

    // template <std::size_t N>
    // ProcessFlag(const std::initializer_list<PF>& bits ) {
    //   for( auto bit : bits ) add(bit) ;
    // }

    /// construct with one bit
    ProcessFlag(PF bit0){
      add( bit0)  ;
    }

    /// construct with two bits
    ProcessFlag(PF bit0, PF bit1){
      add( bit0)  ;
      add( bit1)  ;
    }

    /// construct with three bits
    ProcessFlag(PF bit0, PF bit1, PF bit2 ){
      add( bit0)  ;
      add( bit1)  ;
      add( bit2)  ;
    }

    /// construct with four bits
    ProcessFlag(PF bit0, PF bit1, PF bit2, PF bit3){
      add( bit0)  ;
      add( bit1)  ;
      add( bit2)  ;
      add( bit3)  ;
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



 











