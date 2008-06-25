#ifndef UTIL_LCTOOLS_H
#define UTIL_LCTOOLS_H 1

#include <iostream>

#include "EVENT/LCEvent.h" 
#include "EVENT/MCParticle.h" 
#include "EVENT/LCParameters.h"
#include "EVENT/LCRunHeader.h"

namespace UTIL{
  
  /**Provides some basic functions for printing event data.
   * 
   * @author gaede
   * @version Mar 10, 2003
   * @see LCEvent
   */
  class LCTOOLS{
    
  public:


    /** Simple function to dump event contents to the screen.
     * Also demonstrates the access to event data through the EVENT interface.
     * This is only a short format - one line per collection.
     */
    static void dumpEvent(const EVENT::LCEvent* evt) ;

    /** Dumps the whole event to the screen, using the dedicated print functions
     * for the different collection types.
     */
    static void dumpEventDetailed(const EVENT::LCEvent* evt) ;

    /** Simple function to dump the run header  to the screen.
     */
    static void dumpRunHeader(const EVENT::LCRunHeader* run) ;

    /** Complete printout of all MCParticles in the collection.
     */
    static void printMCParticles(const EVENT::LCCollection* col ) ;

    /** Complete printout of all SimTrackerHits in the collection.
     */
    static void printSimTrackerHits(const EVENT::LCCollection* col ) ;

    /** Complete printout of all TPCHits in the collection.
     */
    static void printTPCHits(const EVENT::LCCollection* col ) ;

    /** Complete printout of all TrackerRawData objects in the collection.
     */
    static void printTrackerRawData(const EVENT::LCCollection* col ) ;

    /** Complete printout of all TrackerData objects in the collection.
     */
    static void printTrackerData(const EVENT::LCCollection* col ) ;

    /** Complete printout of all TrackerPulse objects in the collection.
     */
    static void printTrackerPulse(const EVENT::LCCollection* col ) ;

    /** Complete printout of all TrackerHits in the collection.
     */
    static void printTrackerHits(const EVENT::LCCollection* col ) ;

    /** Complete printout of all SimCalorimeterHits in the collection.
     */
    static void printSimCalorimeterHits(const EVENT::LCCollection* col ) ;

    /** Complete printout of  all CalorimeterHits in the collection.
     */
    static void printCalorimeterHits(const EVENT::LCCollection* col ) ;

    /** Complete printout of  all RawCalorimeterHits in the collection.
     */
    static void printRawCalorimeterHits(const EVENT::LCCollection* col ) ;

    /** Print LCFloatVec user extension vectors  in the  collection
     */
    static void printLCFloatVecs( const EVENT::LCCollection* col ) ;

    /** Print LCIntVecs user extension vectors in the collection
     */
    static void printLCIntVecs( const EVENT::LCCollection* col ) ;

    /** Print LCStrVecs user extension vectors in the collection
     */
    static void printLCStrVecs( const EVENT::LCCollection* col ) ;

    /** Complete printout of all Tracks in the collection
     */
    static void printTracks( const EVENT::LCCollection* col ) ;

    /** Complete printout of all Clusters in the collection
     */
    static void printClusters( const EVENT::LCCollection* col ) ;

    /** Complete printout of all Vertices in the collection
     */
    static void printVertices( const EVENT::LCCollection* col ) ;

    /** Complete printout of all ReconstructedParticles in the collection
     */
    static void printReconstructedParticles( const EVENT::LCCollection* col ) ;

    /** Complete printout of all LCGenericObjects in the collection
     */
    static void printLCGenericObjects( const EVENT::LCCollection* col ) ;

    /** Print the relation.
     */
    static void printRelation( const EVENT::LCCollection* col ) ;

    /** Print the parameters.
     */
    static void printParameters( const EVENT::LCParameters& params );

    /** Returns a string describing the simulator flag bits set for this particle.
     *  If no MCParticle given a string describing the bits is returned:<br>
     *  
     */
    static std::string getSimulatorStatusString(EVENT::MCParticle* mcp=0) ;

    static int printDaughterParticles(const EVENT::MCParticle* part, int index) ;

  }; // class

} // namespace UTIL

#endif /* ifndef UTILL_LCTOOLS_H */
