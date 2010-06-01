// -*- C++ -*-
#ifndef IMPL_TRACKERHITIMPL_H
#define IMPL_TRACKERHITIMPL_H 1

#include <string>

#include "EVENT/TrackerHit.h"
#include "IMPL/AccessChecked.h"
#include "EVENT/TPCHit.h"

#define TRKHITNCOVMATRIX 6

namespace IMPL {

/** Implementation of the  generic tracker hit. 
 * 
 * @author gaede
 * @version $Id: TrackerHitImpl.h,v 1.13 2010-06-01 10:19:37 engels Exp $
 */

  class TrackerHitImpl : public EVENT::TrackerHit , public AccessChecked {

  public: 
    // C'tor
    TrackerHitImpl() ;
    
    /// Destructor.
    virtual ~TrackerHitImpl() ; 


    virtual int id() const { return simpleUID() ; }

    /** The hit  position in [mm].	
     */
    virtual const double* getPosition() const ;

    /**Covariance of the position (x,y,z)
     */
    virtual const EVENT::FloatVec & getCovMatrix() const ;

    /** The dE/dx of the hit in [GeV/mm].
     *
     *  DEPRECATED. use getCharge() instead.
     *
     */ 	
    virtual float getdEdx() const ;

    /** The  time of the hit in [ns]. Is this needed ?
     */
    virtual float getTime() const ;

//     /**Type of raw data hit, either one of<br>
//      * LCIO::TPCHIT<br>
//      * LCIO::SIMTRACKERHIT<br>
//      */
//     virtual const std::string & getType() const ;

    /** Type of hit. Mapping of integer types to type names
     * through collection parameters "TrackerHitTypeNames"
     * and "TrackerHitTypeValues".
     */
    virtual int getType() const ;

    /** The charge of the hit.
     */
    virtual float getCharge() const { return _charge ; }

    /** The charge error
     */
    virtual float getChargeError() const { return _chargeError ; }

    /** The quality bit flag of the hit.
     */
    virtual int getQuality() const { return _quality ; }

    /** The raw data hits. 
     * Check getType() to get actual data type.
     */
    virtual const EVENT::LCObjectVec & getRawHits() const ;


    /** Use to manipulate the raw hits.
     */
    virtual EVENT::LCObjectVec & rawHits() ;


    // setters 
    void setType(int type) ;
    void setPosition( double pos[3]) ;
    void setCovMatrix( const EVENT::FloatVec& cov );
    void setCovMatrix( float cov[TRKHITNCOVMATRIX]  );
    //void setdEdx( float dedx ) ;
    void setTime( float t ) ;
    void setCharge( float charge ) ;
    void setChargeError( float error ) ;
    void setQuality( int quality ) ;
    void setQualityBit( int bit , bool val=true ) ;


protected:
  
    int _type ;
    double _pos[3] ;
    EVENT::FloatVec _cov ;
    //float _dEdx ;
    float _time ;
    float _charge ;
    float _chargeError ;
    int _quality ;
    EVENT::LCObjectVec _rawHits ;
    

}; // class
} // namespace IMPL
#endif /* ifndef IMPL_TRACKERHITIMPL_H */
