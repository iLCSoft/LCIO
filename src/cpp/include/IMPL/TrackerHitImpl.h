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
 * @version Apr 7, 2004
 */

  class TrackerHitImpl : public EVENT::TrackerHit , public AccessChecked {

  public: 
    // C'tor
    TrackerHitImpl() ;
    
    /// Destructor.
    virtual ~TrackerHitImpl() ; 

    /** The hit  position in [mm].	
     */
    virtual const double* getPosition() const ;

    /**Covariance of the position (x,y,z)
     */
    virtual const EVENT::FloatVec & getCovMatrix() const ;

    /** The dE/dx of the hit in [GeV/mm].
     */ 	
    virtual float getdEdx() const ;

    /** The  time of the hit in [ns]. Is this needed ?
     */
    virtual float getTime() const ;

    /**Type of raw data hit, either one of<br>
     * LCIO::TPCHIT<br>
     * LCIO::SIMTRACKERHIT<br>
     */
    virtual const std::string & getType() const ;

    /**The raw data hit. Type is one 
     * of TPCHit, VTXHit,...
     * User has to check with cast.
     */
    virtual LCObject * getRawDataHit() const ;


    // setters 
    void setPosition( double pos[3]) ;
    void setCovMatrix( const EVENT::FloatVec& cov );
    void setCovMatrix( float cov[TRKHITNCOVMATRIX]  );
    void setdEdx( float dedx ) ;
    void setTime( float t ) ;
    void setTPCHit( EVENT::TPCHit* hit) ;



protected:
  
  double _pos[3] ;
  EVENT::FloatVec _cov ;
  float _dEdx ;
  float _time ;
  EVENT::LCObject* _rawHit ;


}; // class
}; // namespace IMPL
#endif /* ifndef IMPL_TRACKERHITIMPL_H */
