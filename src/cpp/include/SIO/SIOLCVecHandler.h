#ifndef SIO_SIOINTVECHANDLER_H
#define SIO_SIOINTVECHANDLER_H 1

// -- lcio headers
#include "SIO/SIOObjectHandler.h"
#include "UTIL/LCIOTypeInfo.h"
#include "EVENT/LCStrVec.h"
#include "EVENT/LCIntVec.h"
#include "EVENT/LCFloatVec.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {


  /** Implementation of SIOObjectHandler to handle IO of IntVecs.
   *
   * @author gaede
   * @version $Id: SIOIntVecHandler.h,v 1.4 2005-04-15 08:37:42 gaede Exp $
   */
  template <typename VECTYPE>
  class SIOLCVecHandler : public SIOObjectHandler {
  public:
    /// Constructor
    SIOLCVecHandler() ;

    /// Reads lcio objects from an SIO stream.
    void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) override ;

    /// Writes lcio objects to an SIO stream.
    void write( sio::write_device& device, const EVENT::LCObject* obj ) override ;

    /// Factory method to create an object of the type of the collection
    EVENT::LCObject *create() const override ;
  }; // class

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

  typedef SIOLCVecHandler<EVENT::LCIntVec>     SIOIntVecHandler ;
  typedef SIOLCVecHandler<EVENT::LCFloatVec>   SIOFloatVecHandler ;
  typedef SIOLCVecHandler<EVENT::LCStrVec>     SIOStrVecHandler ;

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

  template <typename VECTYPE>
  inline SIOLCVecHandler<VECTYPE>::SIOLCVecHandler() :
    SIOObjectHandler( UTIL::lctypename<VECTYPE>() ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  template <typename VECTYPE>
  inline void SIOLCVecHandler<VECTYPE>::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) {
    auto vec = dynamic_cast<VECTYPE*>( objP ) ;
    // read the vector
    // SIO_SDATA( device, vec ) ;
    
    int nElements ;
    SIO_SDATA( device, nElements ) ;
    vec->reserve( nElements ) ;
    for( int i=0 ; i<nElements ; i++ ) {
      typename VECTYPE::value_type x ;
      SIO_SDATA( device , x ) ;
      vec->push_back( x ) ;
    }
    
    // pointer tag
    if( vers > SIO_VERSION_ENCODE( 1, 2 ) ) {
      SIO_PTAG( device , vec ) ;
    }
  }

  //----------------------------------------------------------------------------

  template <typename VECTYPE>
  inline void SIOLCVecHandler<VECTYPE>::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto vec = dynamic_cast<const VECTYPE*>( obj ) ;
    // write the vector
    // SIO_SDATA( device, vec ) ;
    int nElements = vec->size() ;
    SIO_SDATA( device, nElements ) ;
    for( int i=0 ; i<nElements ; i++ ) {
      SIO_SDATA( device, (*vec)[i] ) ;
    }    
    // pointer tag
    SIO_PTAG( device , vec ) ;
  }

  //----------------------------------------------------------------------------

  template <typename VECTYPE>
  inline EVENT::LCObject *SIOLCVecHandler<VECTYPE>::create() const {
    return new VECTYPE() ;
  }


} // namespace

#endif /* ifndef SIO_SIOINTVECHANDLER_H */
