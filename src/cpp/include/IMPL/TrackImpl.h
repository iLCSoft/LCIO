#ifndef IMPL_TRACKIMPL_H
#define IMPL_TRACKIMPL_H 1


#include "EVENT/Track.h"
#include "EVENT/TrackState.h"
#include "AccessChecked.h"
#include <map>
#include <bitset>


#define BIT_ISREFERENCEPOINTDCA 31

typedef std::map< std::string , EVENT::IntVec* > IndexMap ; 


namespace IMPL {

/** Implementation of the LCIO track class. The helix parameterization follows that of the L3 experiment at LEP.
 *  A detailed description of the track parameters is given in the LC-Note LC-DET-2006-004 
 *  "Track Parameters in LCIO" <a href="http://www-flc.desy.de/lcnotes/notes/LC-DET-2006-004.pdf">(pdf)</a>.
 *
 * @see Track
 * @author gaede
 * @version $Id: TrackImpl.h,v 1.15 2007-09-18 09:47:10 gaede Exp $
 */

  class TrackImpl : public EVENT::Track, public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    TrackImpl() ;
    
    /** Copy constructor - creates shallow copy, i.e. all data members are copied but pointers to other LCObjects
     *  i.e. TrackerHits and Tracks are preserved.
     */
    TrackImpl( const TrackImpl& ) ; 
    const TrackImpl& operator = ( const TrackImpl& o ) ; // operator =
    

    /// Destructor.
    virtual ~TrackImpl() ; 
    
    virtual int id() const { return simpleUID() ; }

    /** Flagword that defines the type of track. Bits 0-15 can be used to denote the subdetectors
     *  that have contributed hits used in the track fit. The definition of the  hits has to be done 
     *  elsewhere, e.g. in the run header. Before LCIO 2.0 bit 31 was used to encode isReferencePointPCA (now deprecated).
     */
    virtual  int getType() const  ;


    /** Impact paramter of the track in (r-phi).
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual float getD0() const ;

    /** Phi of the track at reference point.
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual float getPhi() const ;

    /** Omega is the signed curvature of the track in [1/mm].
     *  The sign is that of the particle's charge.
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual float getOmega() const ;

    /** Impact paramter of the track in (r-z).
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual float getZ0() const ;

    /** Lambda is the dip angle of the track in r-z at the reference point.
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual float getTanLambda() const ;

    /** Covariance matrix of the track parameters. Stored as lower triangle matrix where
     *  the order of parameters is:   d0, phi, omega, z0, tan(lambda).
     *  So we have cov(d0,d0), cov( phi, d0 ), cov( phi, phi), ...
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual const EVENT::FloatVec & getCovMatrix() const ;

    /** Reference point of the track parameters.
     *  The default for the reference point is the point of closest approach.
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual const float* getReferencePoint() const ;

    ///** True if the reference point is the point of closest approach.
    // */
    //virtual bool isReferencePointPCA() const ;

    /** Chi^2 of the track fit.
     */
    virtual float getChi2() const ;

    /** Number of degrees of freedom  of the track fit.
     */
    virtual int getNdf() const ;

    /** dEdx of the track.
     */
    virtual float getdEdx() const;

    /** Error of dEdx.
     */
    virtual float getdEdxError() const;

    /** The radius of the innermost hit that has been used in the track fit.
     *  Helps to detect V0 tracks with small impact paramters or haevy mesons.
     *  To be used as convenient information or if hits are not stored in 
     *  the data set, e.g. DST or FastMC. 
     */
    virtual float getRadiusOfInnermostHit() const  ;

    /** A vector that holds the number of hits in particular subdetectors.
     *  The mapping of indices to subdetectors is implementation dependent.
     *  To be used as convenient information or if hits are not stored in 
     *  the data set, e.g. DST or FastMC. 
     *  TODO:  Provide way to store mapping in event/run header.
     */
    virtual const EVENT::IntVec & getSubdetectorHitNumbers() const ;


    /** The tracks (as Track objects) that have been combined to this track.
     */
    virtual const EVENT::TrackVec & getTracks() const ;


    /** Returns track states associtated with this track. @see TrackState.
     */
    virtual const EVENT::TrackStateVec & getTrackStates() const ;


    /** Returns track state closest to the given point. @see TrackState.
     */
    virtual const EVENT::TrackState* getClosestTrackState( float x, float y, float z ) const ;


    /** Returns track state for the given location - or NULL if not found. @see TrackState.
     *  location can be set to: AtIP, AtFirstHit, AtLastHit, AtCalorimeter, AtVertex, AtOther
     */
    virtual const EVENT::TrackState* getTrackState( int location ) const ;


    /** Optionaly ( check/set flag(LCIO::TRBIT_HITS)==1)  return the hits that have been used 
     *  to create this track.
     */
    virtual const EVENT::TrackerHitVec & getTrackerHits() const ;
    

    // setters 
    virtual void  setTypeBit( int index , bool val=true) ;

    virtual void  setD0( float d0 ) ;                           // stored in TrackState
    virtual void  setPhi( float phi ) ;                         // stored in TrackState
    virtual void  setOmega( float omega ) ;                     // stored in TrackState
    virtual void  setZ0( float z0 ) ;                           // stored in TrackState
    virtual void  setTanLambda( float tanLambda ) ;             // stored in TrackState
    virtual void  setCovMatrix( const float* cov ) ;            // stored in TrackState
    virtual void  setCovMatrix( const EVENT::FloatVec& cov ) ;  // stored in TrackState
    virtual void  setReferencePoint( const float* rPnt) ;       // stored in TrackState

    //virtual void  setIsReferencePointPCA( bool val ) ;

    virtual void  setChi2( float chi2 ) ;
    virtual void  setNdf( int ndf ) ;
    virtual void  setdEdx( float dEdx ) ;
    virtual void  setdEdxError( float dEdxError ) ;

    virtual void  addTrack( EVENT::Track* trk ) ;
    virtual void  addTrackState( EVENT::TrackState* trkstate ) ;
    virtual void  addHit( EVENT::TrackerHit* hit) ;

    // direct access to the track state vector 
    virtual  EVENT::TrackStateVec & trackStates() ;


    virtual void  setRadiusOfInnermostHit( float r )  ;

    /** Allows modification of the subdetectorHitNumbers, e.g. <br>
     *  track->subdetectorHitNumbers().resize(5) ; <br> 
     *  track->subdetectorHitNumbers()[4] = 42 ;
     */
    virtual EVENT::IntVec & subdetectorHitNumbers() ;

  protected:

    virtual void  setType( int  type ) ;

    std::bitset<32> _type{0} ;

   
    float _chi2{0} ;
    int   _ndf{0} ; 
    float _dEdx{0} ;
    float _dEdxError{0} ;
    float _radiusOfInnermostHit{0} ;
    EVENT::IntVec _subdetectorHitNumbers{} ;

    EVENT::TrackVec _tracks{} ;
    EVENT::TrackerHitVec _hits{} ;

    EVENT::TrackStateVec _trackStates{} ;



}; // class



} // namespace IMPL
#endif /* ifndef IMPL_TRACKIMLP_H */
