#include "SIO/SIOTPCHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/TPCHit.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"
#include "IOIMPL/TPCHitIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int SIOTPCHitHandler::read(SIO_stream* stream, 
				      LCObject** objP){
    unsigned int status ; 
	
    // create a new object :
    TPCHitIOImpl* hit  = new TPCHitIOImpl ;
    *objP = hit ;
	
    SIO_DATA( stream ,  &(hit->_cellID) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_time) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_charge) , 1  ) ;
    SIO_DATA( stream ,  &(hit->_quality) , 1  ) ;

    if( LCFlagImpl(_flag).bitSet( LCIO::TPCBIT_RAW ) ){

      SIO_DATA( stream ,  &(hit->_rawSize)  , 1 ) ;

      hit->initRawArray( hit->_rawSize ) ; // reserve enough space for raw data

      SIO_DATA( stream ,  hit->_rawArray  , hit->_rawSize  ) ;

    }

    // read a pointer tag for reference to TPC hits from generic hits

    if( _vers > SIO_VERSION_ENCODE( 1, 2) ){    // the logic of the pointer bit has been inverted in v1.3
      
      if( ! LCFlagImpl(_flag).bitSet( LCIO::TPCBIT_NO_PTR )  )
	SIO_PTAG( stream , dynamic_cast<const TPCHit*>(hit) ) ;
      
    }else{
      
      if( LCFlagImpl(_flag).bitSet( LCIO::TPCBIT_NO_PTR )  )    
	SIO_PTAG( stream , dynamic_cast<const TPCHit*>(hit) ) ;
      
    }
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
    
    
  unsigned int SIOTPCHitHandler::write(SIO_stream* stream, 
				       const LCObject* obj){
    
    unsigned int status ; 

    const TPCHit* hit = dynamic_cast<const TPCHit*>(obj)  ;

    LCSIO_WRITE( stream, hit->getCellID()  ) ;
    LCSIO_WRITE( stream, hit->getTime()  ) ;
    LCSIO_WRITE( stream, hit->getCharge()  ) ;
    LCSIO_WRITE( stream, hit->getQuality()  ) ;

    if( LCFlagImpl(_flag).bitSet( LCIO::TPCBIT_RAW ) ){
      
      LCSIO_WRITE( stream, hit->getNRawDataWords()  ) ;

      for(int i=0;i<hit->getNRawDataWords();i++){
	LCSIO_WRITE( stream, hit->getRawDataWord(i)  ) ;
      }
    }

    //  add a pointer tag for reference to TPC hits from generic hits
    if( ! LCFlagImpl(_flag).bitSet( LCIO::TPCBIT_NO_PTR ) ){
      SIO_PTAG( stream , hit ) ;
    }
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
