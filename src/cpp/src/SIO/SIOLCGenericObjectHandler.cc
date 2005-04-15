#include "SIO/SIOLCGenericObjectHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCGenericObject.h"
#include "IOIMPL/LCGenericObjectIOImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"
#include "SIO/SIOLCParameters.h"

using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{
    
  unsigned int  SIOLCGenericObjectHandler::init( SIO_stream* stream, 
					SIO_operation op,
					LCCollection* col, 
					unsigned int vers ) {
    unsigned int status ; 

    if( op == SIO_OP_READ ) {


      SIO_DATA( stream ,  &_flag , 1  ) ;
      if( vers > SIO_VERSION_ENCODE( 1, 1)   ) 
	SIOLCParameters::read( stream ,  col->parameters() , vers ) ;
      
      col->setFlag( _flag ) ;
      _vers = vers ;
      
      _isFixedSize  =  LCFlagImpl(_flag).bitSet( LCIO::GOBIT_FIXED ) ;
      
      if( _isFixedSize ) { // need to read the size variables once
	
	SIO_DATA( stream , &_nInt  , 1  ) ;
	SIO_DATA( stream , &_nFloat  , 1  ) ;
	SIO_DATA( stream , &_nDouble  , 1  ) ;
      }
      

    } else if( op == SIO_OP_WRITE ) {
      

      _isFixedSize = true ;
      // need to check whether we have fixed size objects only
      unsigned int nObj = col->getNumberOfElements() ;
      for( unsigned int i=0 ; i < nObj ; i++ ){
	LCGenericObject* obj  =  dynamic_cast< LCGenericObject* >( col->getElementAt( i ) ) ; 
	if( !  obj->isFixedSize() ){ 
	  _isFixedSize = false ;
	  break ;
	}
      }
      LCFlagImpl flag( col->getFlag() ) ;
      
      LCGenericObject* gObj  =  0 ;
      if( col->getNumberOfElements() > 0 ) 
	  gObj = dynamic_cast< LCGenericObject* >( col->getElementAt( 0 ) ) ; 

      // if the collection doesn't have the TypeName/DataDescription parameters set,
      //  we use the ones from the first object
      if(  col->parameters().getStringVal( "TypeName" ).size() ==  0 && gObj != 0 )
	col->parameters().setValue( "TypeName", gObj->getTypeName() ) ;
      
      if( _isFixedSize ) {
	
	flag.setBit( LCIO::GOBIT_FIXED ) ;
	
	if(  col->parameters().getStringVal( "DataDescription" ).size() ==  0 && gObj != 0 )
	  col->parameters().setValue( "DataDescription", gObj->getDataDescription() ) ;
      }
      
      

      _flag = flag.getFlag() ;
      
      LCSIO_WRITE( stream, _flag  ) ;
      SIOLCParameters::write( stream , col->getParameters() ) ;
      
      if( _isFixedSize ) { // need to write the size variables once

	  if( gObj != 0 ){
	      _nInt = gObj->getNInt() ;
	      _nFloat = gObj->getNFloat() ;
	      _nDouble = gObj->getNDouble() ;
	  } else {
	      _nInt = 0 ;
	      _nFloat = 0 ;
	      _nDouble = 0 ;
	  }

	  SIO_DATA( stream , &_nInt  , 1  ) ;
	  SIO_DATA( stream , &_nFloat  , 1  ) ;
	  SIO_DATA( stream , &_nDouble  , 1  ) ;
	  
	// 	cout << " >>>>>>>  written nInt, nFloat, nDouble : " << _nInt << ", " << _nFloat << ", " << _nDouble  
	// << " for type " << col->getTypeName() << endl ;
      }
      
    }
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  

  unsigned int SIOLCGenericObjectHandler::read(SIO_stream* stream, 
					       LCObject** objP ){
    unsigned int status ; 

    LCGenericObjectIOImpl* obj  = new LCGenericObjectIOImpl ;	
    *objP = obj ;
    
    obj->_isFixedSize = _isFixedSize ;
    
    if( ! _isFixedSize ){ 
      
      SIO_DATA( stream ,  &_nInt , 1  ) ;
      SIO_DATA( stream ,  &_nFloat , 1  ) ;
      SIO_DATA( stream ,  &_nDouble , 1  ) ;
    } 
    
    
    obj->_intVec.resize( _nInt )  ; 
    obj->_floatVec.resize( _nFloat ) ; 
    obj->_doubleVec.resize( _nDouble ) ;
    
    // the 2003 C++ standard guarantes that vector uses  contigous memory ...
    SIO_DATA( stream , &(obj->_intVec[0])  , _nInt  ) ;      
    SIO_DATA( stream , &(obj->_floatVec[0])  , _nFloat  ) ;      
    SIO_DATA( stream , &(obj->_doubleVec[0])  , _nDouble  ) ;      
    
    SIO_PTAG( stream , obj  ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  
  unsigned int SIOLCGenericObjectHandler::write(SIO_stream* stream, 
						const LCObject* obj){
    
    unsigned int status ; 
    
    const LCGenericObject* gObj = dynamic_cast<const LCGenericObject*>(obj)  ;
    
    if( ! _isFixedSize ){ 

      _nInt = gObj->getNInt() ;
      _nFloat = gObj->getNFloat() ;
      _nDouble = gObj->getNDouble() ;

      LCSIO_WRITE( stream , _nInt  ) ;
      LCSIO_WRITE( stream , _nFloat ) ;
      LCSIO_WRITE( stream ,  _nDouble ) ;

    } 
    for( int i=0 ; i< _nInt ; i++){
      LCSIO_WRITE( stream , gObj->getIntVal( i ) ) ;
    }
    for( int i=0 ; i< _nFloat ; i++){
      LCSIO_WRITE( stream , gObj->getFloatVal( i ) ) ;
    }
    for( int i=0 ; i< _nDouble ; i++){
      //      LCSIO_WRITE( stream , gObj->getDoubleVal( i ) ) ;
      double dVal =  gObj->getDoubleVal( i ) ;
      SIO_DATA( stream , &dVal  , 1  ) ;
    }

    SIO_PTAG( stream , gObj  ) ;

    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
} // namespace
