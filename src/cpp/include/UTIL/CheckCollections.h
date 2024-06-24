#ifndef CheckCollections_h
#define CheckCollections_h 1

#include "lcio.h"

#include "UTIL/PIDHandler.h"

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
    
    typedef std::vector<std::pair<std::string,std::string>> Vector ;
    
  public: 
    
    /** Convenient c'tor.
     */
    CheckCollections() = default ;

    /** d'tor */
    virtual ~CheckCollections() = default ;

    /** Checks the file for missing collections - can be called repeadedly on different files.
    The quiet flag makes this function not produce any output.
     */
    void checkFile( const std::string& fileName, bool quiet=false) ;

    /** Checks all files for missing collections.
    The quiet flag makes this function not produce any output.
     */
    void checkFiles( const std::vector<std::string>& fileNames, bool quiet=false) ;

    
    /** dump result of check to stream. The minimal flag reduces the output of
     * this function.
     */
    void print(  std::ostream& os ,bool minimal=false) const ;
    

    /** Returns the collections that are not present in all events checked with checkFiles() with their names and types.
     */
    Vector getMissingCollections() const ;

    /** Returns the collections that are present in all events checked with checkFiles() with their names and types.
     */
    Vector getConsistentCollections() const ;

    /** Add a collection with (name,type) that should be added to events in patchEvent().
     */
    void addPatchCollection(std::string name, std::string type);

    /** Add all collections as Vector(name,type), e.g. retrieved from getMissingCollections()  that should be added to events in patchEvent().
     */
    void addPatchCollections(Vector cols);

    /** Add and empty collection to the event for any collection that is in patchCollections and not in the Event  
     */
    void patchCollections(EVENT::LCEvent* evt ) const ;

    /// Metadata for ParticleIDs that are handled via the PIDHandler. Necessary
    /// for consistency with EDM4hep, where ParticleID no longer lives in
    /// ReconstructedParticle and where the direction of the relation has been
    /// reversed.
    struct PIDMeta {
      std::string name{}; ///< algorithm name
      std::vector<std::string> paramNames{}; ///< parameter names
      uint32_t count{}; ///< How often this was found
    };

  private:

    void insertParticleIDMetas(const UTIL::PIDHandler& pidHandler, const std::string& recoName);

    unsigned _nEvents =0 ;
    std::unordered_map< std::string, std::pair< std::string, unsigned > > _map{} ;
    /// Map from ReconstructedParticle collection names to attached ParticleID
    /// meta information
    std::unordered_map<std::string, std::vector<PIDMeta>> _particleIDMetas{};
    Vector _patchCols{};

  }; // class

}
#endif 
//=============================================================================
