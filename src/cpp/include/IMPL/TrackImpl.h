#ifndef IMPL_TRACKIMPL_H
#define IMPL_TRACKIMPL_H 1


#include "EVENT/Track.h"
#include "AccessChecked.h"
#include <map>

typedef std::map< std::string , DATA::IntVec* > IndexMap ; 

#define NCOVMATRIX 15

namespace IMPL {


/** Implementation of Track.
 *
 * @see Track
 * @author gaede
 * @version Mar 6, 2003
 */

  class TrackImpl : public EVENT::Track, public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    TrackImpl() ;
    
    /// Destructor.
    virtual ~TrackImpl() ; 
    
    /** Type of track, i.e. the subdetector(s) used to derive the track paramters: <br>
     *  TrackData::UNKNOWN <br> 
     *  TrackData::VTX <br> 
     *  TrackData::FTD <br> 
     *  TrackData::SIT <br> 
     *  TrackData::TPC <br> 
     *  TrackData::MUON <br> 
     *  TrackData::FULL <br> 
     */
    virtual int getType() const;

    /** The magnitude of the track's momentum,
     *  signed by the charge.
     */
    virtual float getMomentum() const;

    /** Theta of the  track.
     */
    virtual float getTheta() const;

    /** Phi of the track.
     */
    virtual float getPhi() const;

    /** Impact paramter of the track
     *  in (r-phi).
     */
    virtual float getD0() const;

    /** Impact paramter of the track
     *  in (r-z).
     */
    virtual float getZ0() const;


    /** Covariance matrix of the track parameters.
     */
    virtual const DATA::FloatVec& getCovMatrix() const;

    /** Reference point of the track parameters.
     */
    virtual const float* getReferencePoint() const;

    /** Chi^2 of the track fit.
     */
    virtual float getChi2() const;

    /** dEdx of the track.
     */
    virtual float getdEdx() const;

    /** Error of dEdx.
     */
    virtual float getdEdxError() const;

    /** Returns the names of the hit collections that have been
     *  used to create the track.
     */
    virtual const DATA::StringVec& getHitCollectionNames() const;

    /** Returns all hit indices for the given collection name.
     */
    virtual const DATA::IntVec& getHitIndicesForCollection(const std::string & colName) const;


    /** The tracks (as TrackData objects) that have been combined to this track.
     */
    virtual const DATA::TrackDataVec& getTracksData() const ;

    /** The tracks (as Track objects) that have been combined to this track.
     */
    virtual const EVENT::TrackVec & getTracks() const ;
    
    // setters 
    virtual void  setType( int type ) ;
    virtual void  setMomentum( float momentum ) ;
    virtual void  setTheta( float theta ) ;
    virtual void  setPhi( float phi ) ;
    virtual void  setD0( float d0 ) ;
    virtual void  setZ0( float z0 ) ;

    virtual void  setCovMatrix( float* cov ) ;
    virtual void  setCovMatrix( const DATA::FloatVec& cov ) ;

    virtual void  setReferencePoint( float* rPnt) ;
    virtual void  setChi2( float chi2 ) ;
    virtual void  setdEdx( float dEdx ) ;
    virtual void  setdEdxError( float dEdxError ) ;
    virtual void  addHitIndex( const std::string& colName, int index ) ;   
    virtual void  addTrack(const EVENT::Track* trk ) ;

  protected:
    int _type ;
    float _p ;
    float _theta;
    float _phi ;
    float _d0 ;
    float _z0 ;
    DATA::FloatVec _covMatrix ;
    float  _reference[3] ;
    float _chi2 ;
    float _dEdx ;
    float _dEdxError ;
    mutable  DATA::StringVec _hitCollectionNames ;
    mutable IndexMap _indexMap ;
    //    DATA::TrackDataVec _tracks ;
    EVENT::TrackVec _tracks ;

}; // class



}; // namespace IMPL
#endif /* ifndef IMPL_TRACKIMLP_H */
