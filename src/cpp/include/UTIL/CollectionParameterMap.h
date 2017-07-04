#ifndef CollectionParameterMap_h
#define CollectionParameterMap_h 1

#include "EVENT/LCCollection.h"

#include <map>
#include <string>

namespace UTIL{
  
  
  /** Helper class for setting and retrieving an std::map<string,int> as collection parameters.
   *  Depending on which constructor was used the collection parameters will be updated if
   *  an object of this class goes out of scope. The corresponding collection parameters will
   *  hold the current state of the map. If the a const LCCollection* was specified (read use case)
   *  no parameters will be updated.
   *
   *  @see LCCollection::getParameters()
   *  @see LCParameters
   *  @author F.Gaede, DESY
   *  @version $Id: CollectionParameterMap.h,v 1.1 2008-05-30 13:26:16 gaede Exp $
   */
  class CollectionParameterMap {

  public:  
    CollectionParameterMap() = default ;
    CollectionParameterMap(const CollectionParameterMap& ) = delete ;
    CollectionParameterMap& operator=(const CollectionParameterMap& ) = delete ;
    
    typedef  std::map< std::string, int >  map_type ;

    /** Create CollectionParameterMap for given collection - read the collection parameters
     *  keyName and valueName if they exist.
     */
    CollectionParameterMap( const std::string& keyName ,  const std::string& valueName,  EVENT::LCCollection* col ) ;
    
    /** Create CollectionParameterMap for given collection - read the collection parameters
     *  keyName and valueName if they exist.
     */
    CollectionParameterMap( const std::string& keyName ,  const std::string& valueName,  const EVENT::LCCollection* col ) ;

    /** Update the collection parameters keyName and valueName when going out of scope.
     */
    ~CollectionParameterMap() ;

    /** The std::map< std::string, int >.
     */
    map_type& map() { return _map ; }


  protected:

    
    void init( const EVENT::LCCollection* col ) ;
 
    std::string _keyName{} ;
    std::string _valueName{} ;
    EVENT::LCCollection* _col{NULL} ;
    map_type _map{} ;


  } ; 
  
} // namespace
#endif


