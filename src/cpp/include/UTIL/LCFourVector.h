// -*- C++ -*-
#ifdef CLHEP  // only if CLHEP is available !

#ifndef UTIL_LCFOURVECTOR_H
#define UTIL_LCFOURVECTOR_H 1

#include "DATA/LCObject.h"
#include "CLHEP/Vector/LorentzVector.h"


namespace UTIL {
  
  
  /** Four vector used in LCIO.
   *  The cpp implementation is a CLHEP::HepLorentzVector
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
    
    /** Constructor for templated type,e.g. LCFourVector<McParticle>( myMCParticle ).
     */
    LCFourVector( TT* lcObj) : _lcObj(lcObj) {
    }
    
    
    /** Constructor for LCObject. 
     *  Can be used to save the dynamic_cast to the explicit type from an LCCollection element,
     *  e.g. LCFourVector<McParticle>( col->getElement(i) ).
     */
    LCFourVector(DATA::LCObject* lcObj){
      
      _lcObj = dynamic_cast< TT* >( lcObj ) ;
      
      if( _lcObj == 0 )
	throw EVENT::Exception("Dynamic cast failed for LCFourVector() !") ;
    }

    /** Instances of LCFourVector serve as a handle to the templated type.
     *  Methods of the original type, e.g. MCParticle can be accessed via
     *  the '->' operator. Methods of the four vector implementation (HepLorentzvector)
     *  are accessed via the '.' operator.<br>
     *  For Example:<br>
     *  LCFourVector<McParticle>  particle4V( col->getElementAt(i) ) <br>
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

}; // namespace UTIL


#include "UTIL/LCFourVector.icc"


#endif /* ifndef UTIL_LCFOURVECTOR_H */
#endif
