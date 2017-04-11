// -*- C++ -*-
#ifndef IMPL_TrackerHitZCylinderImpl_H
#define IMPL_TrackerHitZCylinderImpl_H 1

#include <string>

#include "EVENT/TrackerHitZCylinder.h"
#include "IMPL/AccessChecked.h"

#define TRKHITZCYLNCOVMATRIX 6

namespace IMPL {

/** Implementation of the tracker hit on a cylindrical surface parallel to z. 
 * 
 * @author gaede
 * @version $Id: $
 */

  class TrackerHitZCylinderImpl : public EVENT::TrackerHitZCylinder , public AccessChecked {

  public: 
    // C'tor
    TrackerHitZCylinderImpl() ;
    
    /// Destructor.
    virtual ~TrackerHitZCylinderImpl() ; 


    virtual int id() const { return simpleUID() ; }

    /** Same name as in CalorimeterHit, even though there are no 'cells' in this case
     */
    virtual int getCellID0() const { return _cellID0; }

    /** Same name as in CalorimeterHit, even though there are no 'cells' in this case
     *  0 if information is not stored -  check the flag word (bit RTHZBIT_ID1) 
     *  of the collection. Default is to store only cellid0.
     */
    virtual int getCellID1() const { return _cellID1; }


    /** The hit  position in [mm].	
     */
    virtual const double* getPosition() const  {  return _pos ; }  ;

    /** Radius of cylinder.
     */
    //virtual const float getR() const {  return _r ; }  ;

    /** Center of cylinder in RPhi-plane - given as (x,y).
     */
    virtual const float* getCenter() const {  return _center ; }  ;

    /** Measurement error along RPhi */
    virtual float getdRPhi() const {  return _drphi ; }  ;

    /** Measurement error along z */
    virtual float getdZ() const {  return _dz ; }  ;


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
     *  Only store if the flag word (bit RTHZBIT_ID1) 
     *  of the collection is set. Default is to store only cellid0.
     */
    void setCellID1(int id1) ;

    void setType(int type) ;
    void setPosition( const double pos[3]) ;
    void setCenter( const float c[2] ) { setCenter( c[0] , c[1] ) ; };
    void setCenter( float xc, float yc);
    //void setR( float r );
    void setdRPhi( float drphi );
    void setdZ( float dz );
    void setEDep( float e ) ;
    void setEDepError( float e ) ;
    void setTime( float t ) ;
    void setQuality( int quality ) ;
    void setQualityBit( int bit , bool val=true ) ;
    //void setCovMatrix( const EVENT::FloatVec& cov );
    //void setCovMatrix( float cov[TRKHITZCYLNCOVMATRIX]  );


protected:
  
    int _cellID0 ;
    int _cellID1 ;

    int _type ;
    double _pos[3] = {0,0,0} ;
    float  _center[2]  = {0,0};
    float  _drphi ;
    float  _dz ;
    float _EDep ;
    float _EDepError ;
    float _time ;
    int _quality ;
    mutable EVENT::FloatVec _cov ;
    EVENT::LCObjectVec _rawHits ;
    

}; // class
} // namespace IMPL
#endif // #ifndef IMPL_TrackerHitZCylinderImpl_H

