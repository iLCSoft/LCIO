#ifndef MT_TYPES_H
#define MT_TYPES_H 1

// -- std headers
#include <memory>
#include <unordered_set>

namespace EVENT {
  class LCEvent ;
  class LCRunHeader ;
}

namespace MT {
  // forward declarations
  class LCReader;
  class LCReaderListener;
  
  // MT types
  typedef std::shared_ptr<EVENT::LCEvent>      LCEventPtr ;
  typedef std::shared_ptr<EVENT::LCRunHeader>  LCRunHeaderPtr ;
  typedef std::unordered_set<LCReaderListener*> LCReaderListenerList;
    
} // namespace MT

#endif /* ifndef MT_TYPES_H */
