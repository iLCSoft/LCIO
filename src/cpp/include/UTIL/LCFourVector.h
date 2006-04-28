// -*- C++ -*-
#ifdef USE_CLHEP  // only if CLHEP is available !

#ifndef UTIL_LCFourVector_H
#define UTIL_LCFourVector_H 1

#include "EVENT/LCObject.h"
#include "CLHEP/Vector/LorentzVector.h"


// CLHEP 1.9 and higher introduce a namespace:
namespace CLHEP{}  
using namespace CLHEP ;


namespace UTIL {
  
  
  /** Four vector used in LCIO.
   *  The cpp implementation is a CLHEP::HepLorentzVector.
   *  Instances of this class can be used as CLHEP::HepLorentzVector
   *  AND pointers to the corresponding LCIO class.
   *
   *  Currently available are:<br>
   *  - LCFourVector<MCParticle>/MCParticle4V<br>
   *  - LCFourVector<ReconstructedParticle>/ReconstructedParticle4V<br>
   * 
   * @author gaede 
   * @version Mar 12, 2004
   */
  template<class TT> 
  class LCFourVector :  public HepLorentzVector {

  protected: 
    TT* _lcObj ;
    
  public: 
    virtual ~LCFourVector() { /*no_op*/; } 
    
    /** Constructor for templated type,e.g. LCFourVector<MCParticle>( myMCParticle ).
     */
    inline LCFourVector( const TT* lcObj) : _lcObj(lcObj) {
    }
    
    
    /** Constructor for LCObject. 
     *  Can be used to save the dynamic_cast to the explicit type from an LCCollection element,
     *  e.g. LCFourVector<MCParticle>( col->getElement(i) ).
     */
    LCFourVector(EVENT::LCObject* lcObj){
      
      _lcObj = dynamic_cast< TT* >( lcObj ) ;
      
      if( _lcObj == 0 )
	throw EVENT::Exception("Dynamic cast failed for LCFourVector() !") ;
    }

    /** Instances of LCFourVector serve as a handle to the templated type.
     *  Methods of the original type, e.g. MCParticle can be accessed via
     *  the '->' operator. Methods of the four vector implementation (HepLorentzvector)
     *  are accessed via the '.' operator.<br>
     *  For Example:<br>
     *  LCFourVector<MCParticle>  particle4V( col->getElementAt(i) ) <br>
     *    particle4V->getMomentum()    //  LCIO object   <br>
     *    particle4V.m()               // mass from HepLorentzVector  <br>
     */
    TT* operator->() { return _lcObj ; } 


    /** Pointer to the LCObject that has been used to create the four vector.
     *  To be used when the object is needed as a function argument or optionally
     *  to call methods of the object, e.g. particle4V.lcObj()->getMomentum() ;
     */
    TT* lcObj() { return _lcObj ; } 


    
  protected:
    LCFourVector() {}  // no default c'tor
    
  }; // class

} // namespace UTIL


#include "UTIL/LCFourVector.icc"


#endif /* ifndef UTIL_LCFOURVECTOR_H */
#endif  // USE_CLHEP
