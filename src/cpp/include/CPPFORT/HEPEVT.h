// -*- C++ -*-
/**Interface to StdHep fortran COMMON /HEPEVT/
 * 
 * @author H. Vogt
 * @version Sep 30, 2003
 */


#ifndef HEPEVT_H
#define HEPEVT_H 1

#include "EVENT/LCIO.h"
#include "EVENT/LCEvent.h" 
#include "EVENT/MCParticle.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/MCParticleImpl.h"

#include "lcio.h"
#include "Exceptions.h"
#include "hepevt0.h"
#include "hepevt1.h"

using namespace lcio;

namespace HEPEVTIMPL{

  class HEPEVT{
    
  public:

    /** StdHep fortran interface to read from or write to the COMMON /HEPEVT/.
     * fromHepEvt reads from COMMON /HEPEVT/.
     * toHepEvt fills COMMON /HEPEVT/.
     */
    static void fromHepEvt(EVENT::LCEvent* evt) ;
    static void toHepEvt(const EVENT::LCEvent* evt) ;

  }; // class

} // namespace HEPEVTIMPL
  
#endif /* ifndef HEPEVT_H */

