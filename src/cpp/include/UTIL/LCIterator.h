// -*- C++ -*-
#ifndef  UTIL_LCIterator_include
#define  UTIL_LCIterator_include

#include <string>
#include <sstream>
#include <typeinfo>
#include "EVENT/LCEvent.h"
#include "EVENT/LCCollection.h"
#include "lcio.h"

namespace UTIL {

  /** Simple convenient iterator class for LCCollections that saves some boiler plate code.
   *  LCIterators can be constructed either form an LCCollection or from the LCEvent and
   *  and a collection name.
   *  
   *  Examples:
   *  <pre>
   *   // ------ use with while():
   *   LCIterator<TrackerHit> it( evt, "VXDCollection" ) ;
   *   while( TrackerHit* hit = it.next()  ){
   * 
   *      hitMap[ hit->getCellID0() ] = hit ;
   *   }    
   * 
   *   // --- with for() :
   *   for(  LCIterator<Track> it( col ) ;  Track* trk = it.next()  ; ) {
   *
   *      std::cout << trk->getTrackState( TrackState::AtIP ) << std::endl  ;
   *   }
   *  </pre>
   *
   *  @author F.Gaede, DESY
   *  @version $Id:$ 
   */

  template <class T>
  class LCIterator{
  
    LCIterator<T>() {} 
  
  public:

    /** Constructor that reads the collection with the given name from the event -
     *  note: no exception is thrown if the collection is not in the event, 
     *  this will behave the same as an empty collection - use operator() to
     *  test, if the collection exists.
     */
    LCIterator<T>( EVENT::LCEvent* evt, const std::string& name ) : _i(0), _col(0) {
    
    
      try{
      
	_col = evt->getCollection( name ) ;
      
      } catch( EVENT::DataNotAvailableException& ) { }  
    
      _n = (_col ? _col->getNumberOfElements() : 0 ) ;
    
      if( _n > 0 ){
      
	T* t = dynamic_cast<T*>(  _col->getElementAt(0)  );

	if( t == 0 ){

	  std::stringstream s ;
	  s << " invalid iterator type  : " << typeid( t ).name() << " for collection " <<  name  << std::endl ; 
	  throw lcio::Exception( s.str() ) ;
	}
      }
    }
  
  
    /** Constructor for the given collection.
     */
    LCIterator<T>( EVENT::LCCollection* col) : _i(0) , _col( col ) {
    
      _n = (_col ? _col->getNumberOfElements() : 0 ) ;
    
      if( _n > 0 ){
      
	T* t = dynamic_cast<T*>(  _col->getElementAt(0)  );
      
	if( t == 0 ){
	
	  std::stringstream s ;
	  s << " invalid iterator type  : " << typeid( t ).name() << " for collection " << std::endl ; 
	  throw lcio::Exception( s.str() ) ;
	}
      }
    }
  
    /** Returns the next element as long as there is one, otherwise 0 is returned.
     */
    T* next(){
    
      if( _i < _n ) 
	return (T*)_col->getElementAt( _i++ )  ;
      //      return dynamic_cast<T*>( _col->getElementAt( _i++ ) ) ;
      else
	return 0 ;
    }
  
    /** Size of the collection.
     */
    int size() { return _n ; }
  

    /** Serves as a handle to the LCCollection itself, to provide access to the collection parameters etc.
     */
    EVENT::LCCollection* operator->() { return _col ; }

    /** Return pointer to LCCollection, e.g. for testing whether the collections was in the event.
     */
    EVENT::LCCollection* operator()() { return _col ; }

  private:
    int _n{0}, _i ;
    EVENT::LCCollection* _col ;
  } ;

} // namespace UTIL

#endif
