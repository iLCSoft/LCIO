// -*- C++ -*-
#ifndef IMPL_TrackerHitPlaneImpl_H
#define IMPL_TrackerHitPlaneImpl_H 1

#include <string>

#include "EVENT/TrackerHitPlane.h"
#include "IMPL/AccessChecked.h"

#define TRKHITPLANENCOVMATRIX 6

namespace IMPL {

/** Implementation of the planar tracker hit. 
 * 
 * @author gaede
 * @version $Id: $
 */

  class TrackerHitPlaneImpl : public EVENT::TrackerHitPlane , public AccessChecked {

  public: 
    // C'tor
    TrackerHitPlaneImpl() ;
    
    /// Destructor.
    virtual ~TrackerHitPlaneImpl() ; 


    virtual int id() const { return simpleUID() ; }


    /** Same name as in CalorimeterHit, even though there are no 'cells' in this case
     */
    virtual int getCellID0() const { return _cellID0; }

    /** Same name as in CalorimeterHit, even though there are no 'cells' in this case
     *  0 if information is not stored -  check the flag word (bit RTHPBIT_ID1) 
     *  of the collection. Default is to store only cellid0.
     */
    virtual int getCellID1() const { return _cellID1; }


    /** The hit  position in [mm].	
     */
    virtual const double* getPosition() const  {  return _pos ; }  ;

    /** Direction of first measurement - given as (theta, phi).
     *  Defines spanning vector of measurement plane.	
     */
    virtual const float* getU() const { return _u ; }

    /** Direction of second measurement - given as (theta, phi).
     *  Defines spanning vector of measurement plane.	
     */
    virtual const float* getV() const  { return _v ; }

    /** Error along u */
    virtual float getdU() const { return _du ; }

    /** Error along v */
    virtual float getdV() const  { return _dv ; }


    /**Covariance of the position (x,y,z)
     */
    virtual const EVENT::FloatVec & getCovMatrix() const ;

    /** The dE/dx of the hit in [GeV/mm].
     *  @deprecated
     *  @see getEDep()
     */ 	
    virtual float getdEdx() const ;

    /** The deposited energy of the hit [GeV]
     */
    virtual float getEDep() const { return _EDep ; }

    /** The error measured on EDep [GeV]
     */
    virtual float getEDepError() const { return _EDepError ; }

    /** The  time of the hit in [ns]. Is this needed ?
     */
    virtual float getTime() const  { return _time ; } ;

    /** Type of hit. Mapping of integer types to type names
     * through collection parameters "TrackerHitTypeNames"
     * and "TrackerHitTypeValues".
     */
    virtual int getType() const { return _type ; }


    /** The quality bit flag of the hit.
     */
    virtual int getQuality() const { return _quality ; }

    /** The raw data hits. 
     * Check getType() to get actual data type.
     */
    virtual const EVENT::LCObjectVec & getRawHits() const { return _rawHits ; }


    /** Use to manipulate the raw hits.
     */
    virtual EVENT::LCObjectVec & rawHits() { return _rawHits ; }


    // setters 

    /** Sets the first cell id;
     */
    void setCellID0(int id0) ;

    /** Sets the second cell id;
     *  Only store if the flag word (bit RTHPBIT_ID1) 
     *  of the collection is set. Default is to store only cellid0.
     */
    void setCellID1(int id1) ;
    void setType(int type) ;
    void setPosition( const double pos[3]) ;
    void setU( const float u[2] ) { setU( u[0] , u[1] ) ; }
    void setU( float theta, float phi);
    void setV( const float v[2] ) { setV( v[0] , v[1] ) ; };
    void setV( float theta, float phi);
    void setdU( float du );
    void setdV( float dv );
    void setEDep( float e ) ;
    void setEDepError( float e ) ;
    void setTime( float t ) ;
    void setQuality( int quality ) ;
    void setQualityBit( int bit , bool val=true ) ;

protected:
  
    int _cellID0 ;
    int _cellID1 ;

    int    _type ;
    double  _pos[3] = {0,0,0};
    float  _u[2] = {0,0};
    float  _v[2] = {0,0};
    float  _du ;
    float  _dv ;
    float _EDep ;
    float _EDepError ;
    float _time ;
    int _quality ;
    mutable EVENT::FloatVec _cov ;
    EVENT::LCObjectVec _rawHits ;
    

}; // class
} // namespace IMPL
#endif // #ifndef IMPL_TrackerHitPlaneImpl_H 
