// -*- C++ -*-
#ifndef IMPL_TRACKERHITIMPL_H
#define IMPL_TRACKERHITIMPL_H 1

#include <string>

#include "EVENT/TrackerHit.h"
#include "IMPL/AccessChecked.h"
//#include "EVENT/TPCHit.h"

#define TRKHITNCOVMATRIX 6

namespace IMPL {

/** Implementation of the  generic tracker hit. 
 * 
 * @author gaede
 * @version $Id: TrackerHitImpl.h,v 1.15 2010-06-17 12:29:00 engels Exp $
 */

  class TrackerHitImpl : public EVENT::TrackerHit , public AccessChecked {

  public: 
    // C'tor
    TrackerHitImpl() ;
    
    /// Destructor.
    virtual ~TrackerHitImpl() ; 


    virtual int id() const { return simpleUID() ; }

    /** Same name as in CalorimeterHit, even though there are no 'cells' in this case
     */
    virtual int getCellID0() const ;

    /** Same name as in CalorimeterHit, even though there are no 'cells' in this case
     *  0 if information is not stored -  check the flag word (bit RTHBIT_ID1) 
     *  of the collection. Default is to store only cellid0.
     */
    virtual int getCellID1() const ;

    /** The hit  position in [mm].	
     */
    virtual const double* getPosition() const ;

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

    /** Sets the first cell id;
     */
    void setCellID0(int id0) ;

    /** Sets the second cell id;
     *  Only store if the flag word (bit RTHBIT_ID1) 
     *  of the collection is set. Default is to store only cellid0.
     */
    void setCellID1(int id1) ;
  
    void setType(int type) ;
    void setPosition( const double pos[3]) ;
    void setCovMatrix( const EVENT::FloatVec& cov );
    void setCovMatrix( const float cov[TRKHITNCOVMATRIX]  );
    void setdEdx( float dedx ) ; // DEPRECATED. renamed to setEDep()
    void setEDep( float e ) ;
    void setEDepError( float e ) ;
    void setTime( float t ) ;
    void setQuality( int quality ) ;
    void setQualityBit( int bit , bool val=true ) ;


protected:

    int _cellID0{0} ;
    int _cellID1{0} ;

    int _type{0} ;
    double _pos[3] = {0.,0.,0.} ;
    EVENT::FloatVec _cov{} ;
    float _EDep{0} ;
    float _EDepError{0} ;
    float _time{0} ;
    int _quality{0} ;
    EVENT::LCObjectVec _rawHits{} ;
    

}; // class
} // namespace IMPL
#endif /* ifndef IMPL_TRACKERHITIMPL_H */
