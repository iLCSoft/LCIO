#ifndef UTIL_LCStdHepRdr_H
#define UTIL_LCStdHepRdr_H 1

#include "IMPL/LCCollectionVec.h"
#include "UTIL/lStdHep.hh"

namespace UTIL{
  
  /**Basic utility for reading a binary stdhep file and filling
   * a LCCollectionVec with MCParticles containing the stdhep
   * file information.
   * 
   * @author cassell
   * @version $Id: LCStdHepRdr.h,v 1.3 2007-11-08 14:32:23 gaede Exp $
   */
  class LCStdHepRdr{
    
  public:

	/** Open the stdhep input file in the constructer
	 */
    LCStdHepRdr(const char* evfile) ;

	/** noop
	 */
	~LCStdHepRdr() ;

    /** Read an event and return a LCCollectionVec of MCParticles.
     */
	IMPL::LCCollectionVec * readEvent() ;


    /** Return the charge of the particle times 3  - code copied from HepPDT package.
     */
    int threeCharge( int pdgID ) const ;

  private:
    
	lStdHep* _reader;
    

  }; // class

} // namespace UTIL

#endif /* ifndef UTIL_LCStdHepRdr_H */
