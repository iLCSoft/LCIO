#ifndef SIO_SIOLCPARAMETERS_H
#define SIO_SIOLCPARAMETERS_H 1

#include "IMPL/LCParametersImpl.h"

class SIO_stream ;

namespace SIO {
    
    
/** IO of LCParameters.
 *
 * @author gaede
 * @version $Id: SIOLCParameters.h,v 1.4 2005-04-15 08:37:42 gaede Exp $
 */
  class SIOLCParameters : public IMPL::LCParametersImpl{
	
  public:
	
    /** Reads objects from an SIO stream.
     */
    static unsigned int read(SIO_stream* stream, 
			     LCParameters& params,  
			     unsigned int vers)  ;
    
    /** Writes lcio objects to an SIO stream.
     */
    static unsigned int write(SIO_stream* stream, 
			      const LCParameters& params) ;
    
  }; // class
} // namespace

#endif /* ifndef SIO_SIOLCPARAMETERS_H */
