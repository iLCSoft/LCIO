#ifndef SIO_SIOOBJECTHANDLER_H
#define SIO_SIOOBJECTHANDLER_H 1

#include "EVENT/LCObject.h"
//#include "SIO_functions.h"
#include "SIO_block.h"

class SIO_stream ;


namespace SIO{


/**  Interface for all lcio object SIO-handlers, has to be implemented  
 * for all event entities (hits, tracks, clusters,...).
 * 
 * @author gaede
 * @version Mar 6, 2003
 */
class SIOObjectHandler {
  
public:
  
  /** Reads lcio objects from an SIO stream.
   */
  virtual unsigned int read(SIO_stream* stream, 
			    EVENT::LCObject** objP,  
			    unsigned int flag,
			    unsigned int vers) =0 ;
  
  /** Writes lcio objects to an SIO stream.
   */
  virtual unsigned int write(SIO_stream* stream, 
			     const EVENT::LCObject* obj,
			     unsigned int flag) =0 ;
  


  /**Initialize the handler based in the collection flag
   */
  virtual void init( unsigned int flag ) { /*no_op */ } 

//  protected:
//   /**Helper method to compute the version number as float, i.e.
//    *  v_f = major + minor/10.
//    */
//   inline float version2float( unsigned int vers) {
// //     float major =  float(SIO_VERSION_MAJOR(vers)) ;
// //     float minor = float(SIO_VERSION_MINOR(vers)) ;
// //     return major + minor / 10. ;
//     return float(SIO_VERSION_MAJOR(vers)) + 
//       float(SIO_VERSION_MINOR(vers)) / 10. ; 
//   }

}; // class

}; // namespace 
#endif /* ifndef hep_lcio_event_SIOOBJECTHANDLER_H */
