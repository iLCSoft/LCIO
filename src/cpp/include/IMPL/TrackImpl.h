#ifndef IMPL_TRACKIMPL_H
#define IMPL_TRACKIMPL_H 1


#include "EVENT/Track.h"
#include "AccessChecked.h"
#include <map>
#include <bitset>


#define BIT_ISREFERENCEPOINTDCA 31

typedef std::map< std::string , EVENT::IntVec* > IndexMap ; 

#define NCOVMATRIX 15

namespace IMPL {


/** Implementation of Track.
 *
 * @see Track
 * @author gaede
 * @version $Id: TrackImpl.h,v 1.9 2004-07-23 09:15:05 gaede Exp $
 */

  class TrackImpl : public EVENT::Track, public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    TrackImpl() ;
    
    /// Destructor.
    virtual ~TrackImpl() ; 
    
    virtual int id() { return simpleUID() ; }

//     /** Type of track, i.e. the subdetector(s) used to derive the track paramters, e.g. "TPC", "COMB", etc.
//      *  In order to save disc space working groups should try and define three to four letter acronyms for
//      *  their tracking detectors.
//      */
//     virtual const std::string & getType() const ;



    /** Flagword that defines the type of track. Bits 0-15 can be used to denote the subdetectors
     *  that have contributed hits used in the track fit. The definition of the  hits has to be done 
     *  elsewhere, e.g. in the run header. Bit 31 is used to encode isReferencePointPCA.
     */
    virtual  int getType() const  ;

    /** Returns true if the corresponding bit in the type word is set.
     */
    virtual bool testType(int bitIndex) const ;

    /** Impact paramter of the track
     *  in (r-phi).
     */
    virtual float getD0() const ;

    /** Phi of the track at reference point or PCA -- FIXME: is this at reference point or PCA ?
     */
    virtual float getPhi() const ;

    /** Omega is the signed curvature of the track in [1/mm].
     * The sign is that of the particle's charge.
     */
    virtual float getOmega() const ;

    /** Impact paramter of the track
     *  in (r-z).
     */
    virtual float getZ0() const ;

    /** Lambda is the dip angle of the track in r-z --- FIXME: at reference point ??? . 
     */
    virtual float getTanLambda() const ;

    /** Covariance matrix of the track parameters. Stored as lower triangle matrix where
     * the order of parameters is:   d0, phi, omega, z0, tan(lambda).
     * So we have cov(d0,d0), cov( phi, d0 ), cov( phi, phi), ...
     */
    virtual const EVENT::FloatVec & getCovMatrix() const ;

    /** Reference point of the track parameters.
     *  The default for the reference point is the point of closest approach.
     *  @see isReferencPointPCA() 
     */
    virtual const float* getReferencePoint() const ;

    /** True if the reference point is the point of closest approach.
     */
    virtual bool isReferencePointPCA() const ;

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

    /** The hits that have been used to create this track.
     */
    virtual const EVENT::TrackerHitVec & getTrackerHits() const ;
    

    // setters 
    virtual void  setTypeBit( int index , bool val=true) ;

    virtual void  setD0( float d0 ) ;
    virtual void  setPhi( float phi ) ;
    virtual void  setOmega( float omega ) ;
    virtual void  setZ0( float z0 ) ;
    virtual void  setTanLambda( float tanLambda ) ;

    virtual void  setCovMatrix( float* cov ) ;
    virtual void  setCovMatrix( const EVENT::FloatVec& cov ) ;

    virtual void  setReferencePoint( float* rPnt) ;
    virtual void  setIsReferencePointPCA( bool val ) ;

    virtual void  setChi2( float chi2 ) ;
    virtual void  setNdf( int ndf ) ;
    virtual void  setdEdx( float dEdx ) ;
    virtual void  setdEdxError( float dEdxError ) ;

    virtual void  addTrack( EVENT::Track* trk ) ;
    virtual void  addHit( EVENT::TrackerHit* hit) ;


    virtual void  setRadiusOfInnermostHit( float r )  ;

    /** Allows modification of the subdetectorHitNumbers, e.g. <br>
     *  track->subdetectorHitNumbers().resize(5) ; <br> 
     *  track->subdetectorHitNumbers()[4] = 42 ;
     */
    virtual EVENT::IntVec & subdetectorHitNumbers() ;

  protected:

    virtual void  setType( int  type ) ;

    //    std::string _type ;
    std::bitset<32> _type ;
    float _d0 ;
    float _phi ;
    float _omega ;
    float _z0 ;
    float _tanLambda ;

    EVENT::FloatVec _covMatrix ;
    float  _reference[3] ;
    bool _isReferencePointPCA ;

    float _chi2 ;
    int   _ndf ; 
    float _dEdx ;
    float _dEdxError ;
    float _radiusOfInnermostHit ;
    EVENT::IntVec _subdetectorHitNumbers ;

    EVENT::TrackVec _tracks ;
    EVENT::TrackerHitVec _hits ;

}; // class



}; // namespace IMPL
#endif /* ifndef IMPL_TRACKIMLP_H */
