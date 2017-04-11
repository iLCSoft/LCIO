#ifndef SIO_SIOLCGENERICOBJECTHANDLER_H
#define SIO_SIOLCGENERICOBJECTHANDLER_H 1

#include "SIO/SIOObjectHandler.h"

#include "EVENT/LCEvent.h"

namespace SIO {
    
    

  /** Implementation of SIOObjectHandler to handle IO of LCGenericObjects.
   *
   * @author gaede
   * @version $Id: SIOLCGenericObjectHandler.h,v 1.3 2005-04-15 08:37:42 gaede Exp $
   */
  class SIOLCGenericObjectHandler : public SIOObjectHandler {

  public:
	
    /** Reads lcio LCGenericObject objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio LCGenericObject objects to an SIO stream.
     */
	
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;

  virtual unsigned int  init( SIO_stream* stream, SIO_operation op,  
			      EVENT::LCCollection* col  , unsigned int vers ) ; 


  protected:
    int _nInt{0} ;
    int _nFloat{0} ;
    int _nDouble{0} ;
    bool _isFixedSize{false} ;

  }; // class
    
} // namespace
#endif /* ifndef SIO_SIOLCGENERICOBJECTHANDLER_H */
