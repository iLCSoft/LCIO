// -*- C++ -*-
#ifndef EVENT_TRACKERHITIMPL_H
#define EVENT_TRACKERHITIMPL_H 1


#include "EVENT/TrackerHit.h"

namespace IMPL {


/** Implementation of TrackerHit.
 *
 * @see TrackerHit
 * @author gaede
 * @version Mar 6, 2003
 */

  class TrackerHitImpl : public EVENT::TrackerHit {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    TrackerHitImpl() ;
    
    /// Destructor.
    virtual ~TrackerHitImpl() ; 
    
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
     */
    virtual const EVENT::MCParticle * getMCParticle() const ;


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
    void setMCParticle( const EVENT::MCParticle* particle)  ;


  protected:
    int _cellID ;
    double _pos[3] ;
    float _dEdx ;
    float _time ;
    const EVENT::MCParticle* _particle ;

}; // class
}; // namespace IMPL
#endif /* ifndef EVENT_TRACKERHITIMLP_H */
