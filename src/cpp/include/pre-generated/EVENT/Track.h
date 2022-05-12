// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef EVENT_TRACK_H
#define EVENT_TRACK_H 1

#include "EVENT/LCObject.h"
#include "EVENT/Track.h"
#include "EVENT/TrackState.h"
#include "EVENT/TrackerHit.h"
#include "LCIOSTLTypes.h"
#include "empty_ignore.h"

namespace EVENT {

class TrackState;


class Track ;
/**Vector of (pointers to) Tracks.*/
typedef std::vector<Track*> TrackVec ;
/** The LCIO track class. The helix parameterization follows that of the L3 experiment at LEP.
 *  A detailed description of the track parameters is given in the LC-Note LC-DET-2006-004 
 *  "Track Parameters in LCIO" <a href="http://www-flc.desy.de/lcnotes/notes/LC-DET-2006-004.pdf">(pdf)</a>.
 * 
 * @author gaede
 * @version $Id: Track.aid,v 1.15 2007-09-18 09:47:09 gaede Exp $
 */

class Track : public LCObject {

public: 
    /// Destructor.
    virtual ~Track() { /* nop */; }


    /** Useful typedef for template programming with LCIO */
    typedef Track lcobject_type ;

    /** Flagword that defines the type of track. Bits 0-15 can be used to denote the subdetectors
     *  that have contributed hits used in the track fit.  For the definition of the bits 
     *  check/Set the collection variables TrackTypeBitNames and TrackTypeBitIndices.
     *  </br>Bits 16-31 are used internally.
     */
    virtual int getType() const = 0;

    /** Impact paramter of the track in (r-phi).
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual float getD0() const = 0;

    /** Phi of the track at the reference point. @see getReferencePoint
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual float getPhi() const = 0;

    /** Omega is the signed curvature of the track in [1/mm].
     *  The sign is that of the particle's charge.
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual float getOmega() const = 0;

    /** Impact paramter of the track in (r-z).
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual float getZ0() const = 0;

    /** Lambda is the dip angle of the track in r-z at the reference point. @see getReferencePoint
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual float getTanLambda() const = 0;

    /** Covariance matrix of the track parameters. Stored as lower triangle matrix where
     *  the order of parameters is:   d0, phi, omega, z0, tan(lambda).
     *  So we have cov(d0,d0), cov( phi, d0 ), cov( phi, phi), ...
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual const FloatVec & getCovMatrix() const = 0;

    /** Reference point of the track parameters.
     *  The default for the reference point is the point of closest approach.
     *  Information is stored in the first TrackState of this Track, @see TrackState.
     */
    virtual const float* getReferencePoint() const = 0;

    ///** True if the reference point is the point of closest approach.
    // */
    //public boolean isReferencePointPCA() const ;
    /** Chi^2 of the track fit.
     */
    virtual float getChi2() const = 0;

    /** Number of degrees of freedom  of the track fit.
     */
    virtual int getNdf() const = 0;

    /** dEdx of the track.
     */
    virtual float getdEdx() const = 0;

    /** Error of dEdx.
     */
    virtual float getdEdxError() const = 0;

    /** Number of holes.
     */
    virtual int getNholes() const = 0;

    /** The radius of the innermost hit that has been used in the track fit.
     *  Helps to detect V0 tracks with small impact paramters or heavy mesons.
     *  To be used as convenient information or if hits are not stored in 
     *  the data set, e.g. DST or FastMC. 
     */
    virtual float getRadiusOfInnermostHit() const = 0;

    /** A vector that holds the number of hits in particular subdetectors.
     *  The mapping of indices to subdetectors is implementation dependent.
     *  To be used as convenient information or if hits are not stored in 
     *  the data set, e.g. DST or FastMC. 
     *  Check/set collection variable TrackSubdetectorNames for decoding the indices.
     */
    virtual const IntVec & getSubdetectorHitNumbers() const = 0;

    /** The tracks that have been combined to this track.
     */
    virtual const TrackVec & getTracks() const = 0;

    /** Returns track states associtated to this track. @see TrackState.
     */
    virtual const TrackStateVec & getTrackStates() const = 0;

    /** Returns track state closest to the given point. @see TrackState.
     */
    virtual const TrackState * getClosestTrackState(float x, float y, float z) const = 0;

    /** Returns track state for the given location - or NULL if not found. @see TrackState.
     *  location can be:  TrackState::AtIP, TrackState::AtFirstHit, TrackState::AtLastHit, TrackState::AtCalorimeter, TrackState::AtVertex, TrackState::AtOther
     */
    virtual const TrackState * getTrackState(int location) const = 0;

    /** Optionaly ( check/set flag(LCIO::TRBIT_HITS)==1)  return the hits that have been used 
     *  to create this track.
     */
    virtual const TrackerHitVec & getTrackerHits() const = 0;
}; // class
} // namespace EVENT
#endif /* ifndef EVENT_TRACK_H */
