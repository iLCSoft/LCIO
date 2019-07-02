#include "SIO/SIOLCRelationHandler.h"

#include "EVENT/LCIO.h"
#include "IOIMPL/LCRelationIOImpl.h"
#include "IMPL/LCFlagImpl.h"

// -- sio headers
#include <sio/io_device.h>
#include <sio/version.h>

namespace SIO {

  SIOLCRelationHandler::SIOLCRelationHandler() :
    SIOObjectHandler( EVENT::LCIO::LCRELATION ) {
    /* nop */
  }

  //----------------------------------------------------------------------------

  void SIOLCRelationHandler::read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type /*vers*/ ) {
    auto rel = dynamic_cast<IOIMPL::LCRelationIOImpl*>( objP ) ;
    SIO_PNTR( device , &(rel->_from ) );
    SIO_PNTR( device , &(rel->_to ) ) ;
    if( IMPL::LCFlagImpl(_flag).bitSet( EVENT::LCIO::LCREL_WEIGHTED ) ) {
      SIO_SDATA( device , rel->_weight ) ;
    }
  }

  //----------------------------------------------------------------------------

  void SIOLCRelationHandler::write( sio::write_device& device, const EVENT::LCObject* obj ) {
    auto rel = dynamic_cast<const EVENT::LCRelation*>( obj ) ;
    auto from = rel->getFrom() ;
    SIO_PNTR( device, &from ) ;
    auto to = rel->getTo() ;
    SIO_PNTR( device, &to ) ;
    if( IMPL::LCFlagImpl(_flag).bitSet( EVENT::LCIO::LCREL_WEIGHTED ) ){
      SIO_SDATA( device , rel->getWeight() ) ;
    }
  }

  //----------------------------------------------------------------------------

  EVENT::LCObject *SIOLCRelationHandler::create() const {
    return new IOIMPL::LCRelationIOImpl() ;
  }

} // namespace
