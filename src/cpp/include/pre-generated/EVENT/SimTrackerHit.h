// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef EVENT_SIMTRACKERHIT_H
#define EVENT_SIMTRACKERHIT_H 1

#include "EVENT/LCObject.h"
#include "empty_ignore.h"

namespace EVENT {

class MCParticle;

/** A generic simulated tracker hit. 
 * 
 * @author gaede
 * @version $Id: SimTrackerHit.aid,v 1.11 2010-06-17 12:28:59 engels Exp $
 */

class SimTrackerHit : public LCObject {

public: 
    /// Destructor.
    virtual ~SimTrackerHit() { /* nop */; }


    /** Useful typedef for template programming with LCIO */
    typedef SimTrackerHit lcobject_type ;

   /** Same name as in CalorimeterHit, even though there are no 'cells' in this case
     */
    virtual int getCellID0() const = 0;

    /** Same name as in CalorimeterHit, even though there are no 'cells' in this case
     *  Optional, check/set flag(LCIO::THBIT_ID1)==1.
     */
    virtual int getCellID1() const = 0;

    /** DEPRECATED: use @getCellID0()
     */
    virtual int getCellID() const = 0;

    /** Returns the hit  position in [mm].	
     */
    virtual const double* getPosition() const = 0;

    /** Returns  the dE/dx of the hit in [GeV].
     *  @deprecated
     *  @see getEDep()
     */
    virtual float getdEdx() const = 0;

    /** The energy deposited on the hit [GeV]
     */
    virtual float getEDep() const = 0;

    /** Returns the  time of the hit in [ns]. TO DO needs definition.
     */
    virtual float getTime() const = 0;

    /** Returns the MC particle that caused the hit.
     *
     * @see MCParticle
     */
    virtual MCParticle * getMCParticle() const = 0;

    /** Returns the 3-momentum of the particle at the hits position in [GeV] - 
     * optional, only if bit LCIO::THBIT_MOMENTUM is set.	
     */ 
    virtual const float* getMomentum() const = 0;

    /** The path length of the particle in the sensitive material that resulted in this hit.
     *  This is only stored together with momentum, i.e. if  LCIO::THBIT_MOMENTUM is set.
     */
    virtual float getPathLength() const = 0;

    /** The quality bit flag of the hit */
    virtual int getQuality() const = 0;

    // define the bit positions for the quality flag

    static const int BITOverlay = 31 ;
    static const int BITProducedBySecondary = 30 ;

    /** True if the hit has been overlayed by the simulation (or digitization)  program.
     */
    virtual bool isOverlay() const = 0;

    /** True if the particle has been created by a secondary particle that is not stored in the MCParticle collection.
     */
    virtual bool isProducedBySecondary() const = 0;
}; // class
} // namespace EVENT
#endif /* ifndef EVENT_SIMTRACKERHIT_H */
