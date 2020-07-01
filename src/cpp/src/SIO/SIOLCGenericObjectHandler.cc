#include "SIO/SIOLCGenericObjectHandler.h"

#include "EVENT/LCIO.h"
#include "EVENT/LCGenericObject.h"
#include "IOIMPL/LCGenericObjectIOImpl.h"
#include "IMPL/LCFlagImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOLCGenericObjectHandler::SIOLCGenericObjectHandler() :
    SIOObjectHandler( EVENT::LCIO::LCGENERICOBJECT ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOLCGenericObjectHandler::initReading( sio::read_device &device, EVENT::LCCollection *collection, sio::version_type vers ) {
    SIOObjectHandler::initReading( device, collection, vers ) ;
    _isFixedSize = IMPL::LCFlagImpl(_flag).bitSet( EVENT::LCIO::GOBIT_FIXED ) ;
    if( _isFixedSize ) { // need to read the size variables once
      SIO_SDATA( device , _nInt ) ;
      SIO_SDATA( device , _nFloat ) ;
      SIO_SDATA( device , _nDouble ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOLCGenericObjectHandler::initWriting( sio::write_device &device, EVENT::LCCollection *collection ) {
    _isFixedSize = true ;
    // need to check whether we have fixed size objects only
    unsigned int nObj = collection->getNumberOfElements() ;
    for( unsigned int i=0 ; i < nObj ; i++ ) {
      auto obj = dynamic_cast< EVENT::LCGenericObject* >( collection->getElementAt( i ) ) ;
      if( !  obj->isFixedSize() ) {
        _isFixedSize = false ;
        break ;
      }
    }
    IMPL::LCFlagImpl colflag( collection->getFlag() ) ;
    EVENT::LCGenericObject* gObj = nullptr ;
    if( collection->getNumberOfElements() > 0 ) {
      gObj = dynamic_cast< EVENT::LCGenericObject* >( collection->getElementAt( 0 ) ) ;
    }
    // if the collection doesn't have the TypeName/DataDescription parameters set,
    //  we use the ones from the first object
    if(  collection->parameters().getStringVal( "TypeName" ).empty() && nullptr != gObj ) {
      collection->parameters().setValue( "TypeName", gObj->getTypeName() ) ;
    }
    if( _isFixedSize ) {
      colflag.setBit( EVENT::LCIO::GOBIT_FIXED ) ;
      if( collection->parameters().getStringVal( "DataDescription" ).empty() && nullptr != gObj ) {
        collection->parameters().setValue( "DataDescription", gObj->getDataDescription() ) ;
      }
    }
    _flag = colflag.getFlag() ;
    collection->setFlag( _flag ) ;
    SIOObjectHandler::initWriting( device, collection ) ;
    if( _isFixedSize ) { // need to write the size variables once
      if( gObj != 0 ){
        _nInt = gObj->getNInt() ;
        _nFloat = gObj->getNFloat() ;
        _nDouble = gObj->getNDouble() ;
      }
      else {
        _nInt = 0 ;
        _nFloat = 0 ;
        _nDouble = 0 ;
      }
      SIO_SDATA( device , _nInt ) ;
      SIO_SDATA( device , _nFloat ) ;
      SIO_SDATA( device , _nDouble ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOLCGenericObjectHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type /*vers*/ ) {
    auto gobj = dynamic_cast<IOIMPL::LCGenericObjectIOImpl*>( objP ) ;
    gobj->_isFixedSize = _isFixedSize ;
    if( ! _isFixedSize ) {
      SIO_SDATA( device , _nInt ) ;
      SIO_SDATA( device , _nFloat ) ;
      SIO_SDATA( device , _nDouble ) ;
    }
    gobj->_intVec.resize( _nInt )  ;
    gobj->_floatVec.resize( _nFloat ) ;
    gobj->_doubleVec.resize( _nDouble ) ;
    SIO_DATA( device , &(gobj->_intVec[0]), _nInt  ) ;
    SIO_DATA( device , &(gobj->_floatVec[0]), _nFloat  ) ;
    SIO_DATA( device , &(gobj->_doubleVec[0]), _nDouble  ) ;
    SIO_PTAG( device , gobj  ) ;
  }

  //----------------------------------------------------------------------------

  void SIOLCGenericObjectHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto gObj = dynamic_cast<const EVENT::LCGenericObject*>( obj ) ;
    if( ! _isFixedSize ) {
      _nInt = gObj->getNInt() ;
      _nFloat = gObj->getNFloat() ;
      _nDouble = gObj->getNDouble() ;
      SIO_SDATA( device , _nInt  ) ;
      SIO_SDATA( device , _nFloat ) ;
      SIO_SDATA( device , _nDouble ) ;
    }

    for( int i=0 ; i< _nInt ; i++) {
      SIO_SDATA( device , gObj->getIntVal( i ) ) ;
    }
    for( int i=0 ; i< _nFloat ; i++) {
      SIO_SDATA( device , gObj->getFloatVal( i ) ) ;
    }
    for( int i=0 ; i< _nDouble ; i++) {
      SIO_SDATA( device , gObj->getDoubleVal( i ) ) ;
    }
    SIO_PTAG( device , gObj ) ;
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOLCGenericObjectHandler::create() const {
    return new IOIMPL::LCGenericObjectIOImpl() ;
  }

} // namespace
