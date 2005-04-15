#ifndef SIO_SIOUnpack_H
#define SIO_SIOUnpack_H 1

#include "SIO_record.h" 


namespace SIO {

/** Helper class that sets the unpack flag of known SIO records
 *  as specified by the constructor. After an object of this class goes out of 
 *  scope the original setting is restored. This is needed to have multiple
 *  read streams (event overlay).
 *
 * @author gaede
 * @version $Id: SIOUnpack.h,v 1.2 2005-04-15 08:37:43 gaede Exp $
 */
  class SIOUnpack{
    
  public :
    static const int RUN = 1 ;
    static const int EVENTHDR = 2 ;
    static const int EVENT = 4 ;
    
    SIOUnpack( int recordFlag ) ;
    ~SIOUnpack() ;
    
  protected:
    SIOUnpack() {}
    
    bool _runFlag ;
    bool _evtHdrFlag ;
    bool _evtFlag ;
    
  } ;
  
} // namespace 
#endif // ifndef SIO_SIOUnpack_H
