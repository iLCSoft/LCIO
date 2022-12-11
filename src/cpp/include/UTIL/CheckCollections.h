#ifndef CheckCollections_h
#define CheckCollections_h 1

#include "lcio.h"

#include <string>
#include <unordered_map>
#include <set>

namespace UTIL {

  
/** Utility class for checking and patching events with respect to collections that are not present 
 *  in every event of a set of files. 
 *  
 *  \author F.Gaede, DESY
 *  \date Dec 2022
 */
  class CheckCollections {
    
    typedef std::set<std::pair<std::string,std::string>> Set ;
    
  public: 
    
    /** Convenient c'tor.
     */
    CheckCollections() = default ;

    /** d'tor */
    virtual ~CheckCollections() = default ;

    /** Checks the file for missing collections - can be called repeadedly on different files.
     */
    void checkFile( const std::string& fileName ) ;

    /** Checks all files for missing collections.
     */
    void checkFiles( const std::vector<std::string>& fileNames ) ;

    
    /** dump result of check to stream */
    void print(  std::ostream& os ) const ;
    

    /** Returns the collections that are not present in all events checked with checkFiles() with their names and types.
     */
    Set getMissingCollections() const ;

    /** Returns the collections that are present in all events checked with checkFiles() with their names and types.
     */
    Set getConsistentCollections() const ;

    /** Add a collection with (name,type) that should be added to events in patchEvent().
     */
    void addPatchCollection(const std::string name, std::string type){
      _patchCols.emplace( name, type ) ;
    }

    /** Add a all collections as Set(name,type), e.g. retrieved from getMissingCollections()  that should be added to events in patchEvent().
     */
    void addPatchCollections(Set cols){
      _patchCols.merge( cols ) ;
    }

    /** Add and empty collection to the event for any collection that is in patchCollections and not in the Event  
     */
    void patchCollections(EVENT::LCEvent* evt ) const ;
    
  private:
    unsigned _nEvents =0 ;
    std::unordered_map< std::string, std::pair< std::string, unsigned > > _map{} ;
    Set _patchCols {} ;

  }; // class

}
#endif 
//=============================================================================
