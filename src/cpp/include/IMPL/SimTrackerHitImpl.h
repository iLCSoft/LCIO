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
    
    virtual int id() const { return simpleUID() ; }

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


    /** Returns the 3-momentum of the particle at the hits position in [GeV] - 
     * optional, only if bit LCIO::THBIT_MOMENTUM is set.	
     */ 
    virtual const float* getMomentum() const ;

    /** The path length of the particle in the sensitive material that resulted in this hit.
     *  This is only stored together with momentum, i.e. if  LCIO::THBIT_MOMENTUM is set.
     */
    virtual float getPathLength() const ;

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

    /** Sets the momentum of the particle at the hit's position.
     */
    void setMomentum( float p[3] ) ;

    /** Sets the momentum of the particle at the hit's position.
     */
    void setMomentum( float px, float py, float pz )  ;

	/* Set the path length.
	 */	
    void setPathLength(float pathLength);

  protected:
    int _cellID ;
    double _pos[3] ;
    float _dEdx ;
    float _time ;
    EVENT::MCParticle* _particle ;
    float _p[3] ;
    float _pathLength ;

}; // class
} // namespace IMPL
#endif /* ifndef IMPL_SIMTRACKERHITIMLP_H */
