#ifndef EVENT_LCRUNHEADERIMPL_H
#define EVENT_LCRUNHEADERIMPL_H 1

#include <string>

#include "EVENT/LCIO.h"
#include "EVENT/LCRunHeader.h"
#include "AccessChecked.h"
#include "LCParametersImpl.h"

namespace IMPL {

/** Implementation of LCRunHeader.
 * 
 * @author gaede
 * @version Mar 11, 2003
 */
  class LCRunHeaderImpl : public EVENT::LCRunHeader, public AccessChecked {

  public: 

    LCRunHeaderImpl() ;

    /// Destructor.
    virtual ~LCRunHeaderImpl() ; 

    virtual int id() const { return simpleUID() ; }

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
    virtual const std::vector<std::string> * getActiveSubdetectors() const ;


    /** Parameters defined for this run.
     */
    virtual const EVENT::LCParameters & getParameters() const { return _params ; } 

    /** Parameters defined for this run.
     */
    virtual EVENT::LCParameters & parameters() { return _params ; } 


    // set methods

    /** Sets the run number.
     */
    void setRunNumber( int rn) ;

    /** Sets the detector name.
     */
    void setDetectorName(const std::string& dn) ;

    /** Sets the description of the run.
     */
    void setDescription(const std::string& dsc) ;

    /** Adds an active sub detector name.
     */
    void addActiveSubdetector(const std::string&  adn) ;


  protected:
    virtual void setReadOnly( bool readOnly ) ;

  protected:
    
    int _runNumber ;
    std::string _detectorName ;
    std::string _description ;
    std::vector<std::string> _activeSubdetectors ;

    LCParametersImpl _params ;

  }; // class
} // namespace IMPL
#endif /* ifndef IMPL_LCRUNHEADERIMPL_H */
