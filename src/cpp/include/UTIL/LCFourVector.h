// -*- C++ -*-
#ifdef CLHEP  // only if CLHEP is available !

#ifndef UTIL_LCFOURVECTOR_H
#define UTIL_LCFOURVECTOR_H 1

#include "DATA/LCObject.h"
#include "CLHEP/Vector/LorentzVector.h"

// namespace EVENT{
//   class MCParticle ;
//   class MCParticleImpl ;
// };

namespace UTIL {
  
  
  /** Four vector used in LCIO.
   * The cpp implementation is a CLHEP::HepLorentzVector
   * 
   * @author gaede 
   * @version Mar 12, 2004
   */
  template<class TT> 
  //  class LCFourVector {
  class LCFourVector :  public HepLorentzVector {

  protected: 
    TT* _lcObj ;
    
  public: 
    virtual ~LCFourVector() { /*no_op*/; } 
    
    LCFourVector( TT* lcObj) : _lcObj(lcObj) {
    }
    
    LCFourVector(DATA::LCObject* lcObj){
      
      _lcObj = dynamic_cast< TT* >( lcObj ) ;
      
      if( _lcObj == 0 )
	throw EVENT::Exception("Dynamic cast failed for LCFourVector() !") ;
    }

    TT* operator->() { return _lcObj ; } 

    TT* lcObj() { return _lcObj ; } 


    
  protected:
    LCFourVector() {}  // no default c'tor
    
  }; // class


//   template<> 
//   class LCFourVector<EVENT::MCParticle> :  public HepLorentzVector {
//   public:
//     LCFourVector( EVENT::MCParticle* mcPart ) ;
//     LCFourVector( DATA::LCObject* lcObj ) ;
//   } ;


//   typedef LCFourVector<EVENT::MCParticle> MCParticle4V ;
//   typedef LCFourVector<EVENT::MCParticleImpl> MCParticleI4V ;
  


}; // namespace UTIL


#include "UTIL/LCFourVector.icc"


#endif /* ifndef UTIL_LCFOURVECTOR_H */
#endif
