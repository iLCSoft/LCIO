#ifndef IMPL_TRACKSTATEIMPL_H
#define IMPL_TRACKSTATEIMPL_H 1


#include "EVENT/TrackState.h"
#include "AccessChecked.h"
#include <map>


#define TRKSTATENCOVMATRIX 15
#define TRKSTATENREFSIZE 3

namespace IMPL {

/** Implementation of the LCIO TrackState class.
 *
 * @see TrackState
 * @author gaede, engels
 * @version $Id:$
 */


  class TrackStateImpl : public EVENT::TrackState, public AccessChecked {

  public: 

    /** Default constructor, initializes values to 0.
     */
    TrackStateImpl() ;
    TrackStateImpl(int location, float d0, float phi, float omega, float z0, float tanLambda, const float* covMatrix, const float* reference) ;
    TrackStateImpl(int location, float d0, float phi, float omega, float z0, float tanLambda, const EVENT::FloatVec& covMatrix, const float* reference) ;
    /** Copy constructor which takes as an argument an EVENT::TrackState reference */
    TrackStateImpl(const EVENT::TrackState &p );


    
    /// Destructor.
    virtual ~TrackStateImpl() ; 
    

    virtual int id() const { return simpleUID() ; }


    /** The location of the track state.
     *  Location can be set to: AtIP, AtFirstHit, AtLastHit, AtCalorimeter, AtVertex, AtOther
     */
    virtual int getLocation() const ;

    /** Impact paramter of the track
     *  in (r-phi).
     */
    virtual float getD0() const ;

    /** Phi of the track at reference point.
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

    /** Lambda is the dip angle of the track in r-z at the reference point.
     */
    virtual float getTanLambda() const ;

    /** Covariance matrix of the track parameters. Stored as lower triangle matrix where
     * the order of parameters is:   d0, phi, omega, z0, tan(lambda).
     * So we have cov(d0,d0), cov( phi, d0 ), cov( phi, phi), ...
     */
    virtual const EVENT::FloatVec & getCovMatrix() const ;

    /** Reference point of the track parameters, e.g. the origin at the IP, or the position
     *  of the first/last hits or the entry point into the calorimeter.
     */
    virtual const float* getReferencePoint() const ;
   

    // setters 
    virtual void  setLocation( int location ) ;
    virtual void  setD0( float d0 ) ;
    virtual void  setPhi( float phi ) ;
    virtual void  setOmega( float omega ) ;
    virtual void  setZ0( float z0 ) ;
    virtual void  setTanLambda( float tanLambda ) ;

    virtual void  setCovMatrix( const float* cov ) ;
    virtual void  setCovMatrix( const EVENT::FloatVec& cov ) ;

    virtual void  setReferencePoint( const float* rPnt) ;


  protected:

    int _location{} ; // location defined by TrackStateLocationEnum
    float _d0 ;
    float _phi ;
    float _omega ;
    float _z0 ;
    float _tanLambda ;

    EVENT::FloatVec _covMatrix ;
    float  _reference[TRKSTATENREFSIZE]{} ;

}; // class

} // namespace IMPL
#endif /* ifndef IMPL_TRACKSTATEIMLP_H */

