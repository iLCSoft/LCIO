#ifndef LCTypedVector_h
#define LCTypedVector_h 1

#include <vector>

#include "EVENT/LCObject.h"
#include "EVENT/LCCollection.h"
#include "Exceptions.h"

#include <sstream>
#include <typeinfo>


namespace UTIL{


  /** Templated helper class that is an std::vector<T*> of the elements in the LCCollection.
   *  This can be used, e.g. if the objects are to be passed to an algorithm in terms of
   *  iterators. For example:<p>
   *   &nbsp; LCTypedVector<SimTrackerHits> sim( evt->getCollection("tpc") ) ;<br>
   *   &nbsp; LCCollectionVec* trackerHits = new LCCollectionVec( LCIO::TRACKERHIT )  ;<br>
   *   &nbsp; std::transform( sim.begin(), sim.end(), std::back_inserter( *trackerHits ) , digitizer ) ;
   *  <p>Note that instantiating an LCTypedVector involves copying and casting of the pointers in 
   *  the collection. In a future release of LCIO this might not be neccessary any more.
   *  @author gaede
   *  @version $Id: LCTypedVector.h,v 1.3 2007-02-19 17:38:46 gaede Exp $
   */
  template <class T> 
  class LCTypedVector : public  std::vector<T*> {
    
  public:  
    LCTypedVector() = default ;
    LCTypedVector(const LCTypedVector& ) = delete ;
    LCTypedVector& operator=(const LCTypedVector& ) = delete ;
  
    LCTypedVector( EVENT::LCCollection* collection ) : _col( collection) {
      
      this->resize( _col->getNumberOfElements() ) ;
      
      for(int i=0;i<_col->getNumberOfElements();i++ ) {
	
	(*this)[i] = dynamic_cast<T*>( _col->getElementAt(i) ) ;
	
	// check the first element for the proper type
	if( i == 0 && (*this)[i] == 0  ){

	  std::stringstream str ;
	  str << "LCTypedVector: cannot convert " << _col->getTypeName() << " to " 
	      << typeid(T).name()  ;

	  throw EVENT::Exception(  str.str().c_str() ) ; 
	}
      }
    }
    
    EVENT::LCCollection* col() { return _col ; }
    
  protected:
    EVENT::LCCollection* _col ;
  } ; 
  
} // namespace
#endif


