#ifndef IMPL_SIMTRACKERHITIMPL_H
#define IMPL_SIMTRACKERHITIMPL_H 1


#include "EVENT/SimTrackerHit.h"
#include "EVENT/MCParticle.h"
#include "AccessChecked.h"


namespace IMPL {


/** Implementation of SimTrackerHit.
 *
 * @see SimTrackerHit
 * @author gaede
 * @version Mar 6, 2003
 */

  class SimTrackerHitImpl : public EVENT::SimTrackerHit, public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    SimTrackerHitImpl() ;
    
    /// Destructor.
    virtual ~SimTrackerHitImpl() ; 
    
    virtual int id() { return simpleUID() ; }

    /**Returns the detector specific (geometrical) cell id.
     */
    virtual int getCellID() const ;

    /** Returns the hit  position.	
     */
          
    virtual const double * getPosition() const ;

    /** Returns  the dE/dx of the hit.
     */ 	
    virtual float getdEdx() const ;

    /** Returns the  time of the hit. TO DO needs definition.
     */
    virtual float getTime() const ;


    /** Returns the MC particle that caused the hit.
     * @see MCParticle
     */
    virtual EVENT::MCParticle * getMCParticle() const ;


    // ---------- setters ------------------------
    /** Sets the cell id.
     */
    void setCellID( int id) ;

    /** Sets the position.
     */
    void setPosition( double pos[3])  ;

    /** Sets dE/dx.
     */
    void setdEdx( float dEdX )  ;

    /** Sets the time.
     */
    void setTime( float t )  ;

    /** Sets the MCParticle that caused the hit.
     */
    void setMCParticle( EVENT::MCParticle* particle)  ;


  protected:
    int _cellID ;
    double _pos[3] ;
    float _dEdx ;
    float _time ;
    EVENT::MCParticle* _particle ;

}; // class
}; // namespace IMPL
#endif /* ifndef IMPL_SIMTRACKERHITIMLP_H */
