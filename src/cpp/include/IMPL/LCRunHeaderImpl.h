// -*- C++ -*-
/** Implementation of LCRunHeader.
 * 
 * @author gaede
 * @version Mar 11, 2003
 */
#ifndef EVENT_LCRUNHEADERIMPL_H
#define EVENT_LCRUNHEADERIMPL_H 1

#include <string>

#include "EVENT/LCIO.h"
#include "EVENT/LCRunHeader.h"


namespace IMPL {

  class LCRunHeaderImpl : public EVENT::LCRunHeader {

  public: 

    LCRunHeaderImpl() ;

    /// Destructor.
    virtual ~LCRunHeaderImpl() ; 

    /** Returns the run number.
     */
    virtual int getRunNumber() const ;

    /** Returns the name of the detector setup used in the simulation.
     */
    virtual const std::string & getDetectorName() const ;

    /** Description of the simulation conditions, e.g. physics channels.
     */		 
    virtual const std::string & getDescription() const ;

    /** Returns the names of the active subdetectors
     *  used in the simulation.
     */ 
    virtual const EVENT::StringVec * getActiveSubdetectors() const ;

    // set methods

    /** Sets the run number.
     */
    void setRunNumber( int rn) ;

    /** Sets the detector name.
     */
    void setDetectorName(const string& dn) ;

    /** Sets the description of the run.
     */
    void setDescription(const string& dsc) ;

    /** Adds an active sub detector name.
     */
    void addActiveSubdetector(const string&  adn) ;


  protected:
    
    int _runNumber ;
    string _detectorName ;
    string _description ;
    EVENT::StringVec _activeSubdetectors ;

  }; // class
}; // namespace IMPL
#endif /* ifndef IMPL_LCRUNHEADERIMPL_H */
