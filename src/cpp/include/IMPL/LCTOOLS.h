#ifndef IMPL_LCTOOLS_H
#define IMPL_LCTOOLS_H 1

#include <iostream>

#include "EVENT/LCEvent.h" 
#include "EVENT/MCParticle.h" 


namespace IMPL{
  
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

    /** Complete printout of all MCParticles in the collection.
     */
    static void printMCParticles(const EVENT::LCCollection* col ) ;

    /** Complete printout of all SimTrackerHits in the collection.
     */
    static void printSimTrackerHits(const EVENT::LCCollection* col ) ;

    /** Complete printout of all TPCHits in the collection.
     */
    static void printTPCHits(const EVENT::LCCollection* col ) ;

    /** Complete printout of all SimCalorimeterHits in the collection.
     */
    static void printSimCalorimeterHits(const EVENT::LCCollection* col ) ;

    /** Complete printout of  all CalorimeterHits in the collection.
     */
    static void printCalorimeterHits(const EVENT::LCCollection* col ) ;


    /** Print LCFloatVec user extension vectors  in the  collection
     */
    static void printLCFloatVecs( const EVENT::LCCollection* col ) ;

    /** Print LCIntVecs user extension vectors in the collection
     */
    static void printLCIntVecs( const EVENT::LCCollection* col ) ;


    /** Prints daughter particles recursively.
     */
    static int printDaughterParticles(const EVENT::MCParticle* part, int index) ;


  }; // class
  
}; // namespace IMPL

#endif /* ifndef IMPL_LCTOOLS_H */
