  /**Some basic functions for dumping events.
   * 
   * @author gaede
   * @version Mar 10, 2003
   * @see LCEvent
   */
#ifndef IMPL_LCTOOLS_H
#define IMPL_LCTOOLS_H 1

#include <iostream>

#include "EVENT/LCEvent.h" 


namespace IMPL{
  
  class LCTOOLS{
    
  public:

    /** Simple function to dump event contents to the screen.
     * Also demonstrates the access to event data through the 'read only' 
     * const interface.
     */
    static void dumpEvent(const EVENT::LCEvent* evt) ;

  }; // class
  
}; // namespace IMPL

#endif /* ifndef IMPL_LCTOOLS_H */
