#ifndef UTIL_LCStdHepRdr_H
#define UTIL_LCStdHepRdr_H 1

#include "IMPL/LCCollectionVec.h"
#include "UTIL/lStdHep.hh"
#include "EVENT/LCIO.h"
#include "Exceptions.h"

namespace IMPL{

class LCEventImpl ;

}

namespace UTIL{
  
  /**Basic utility for reading a binary stdhep file and filling
   * a LCCollectionVec with MCParticles containing the stdhep
   * file information.
   * 
   * @author cassell
   * @version $Id: LCStdHepRdr.h,v 1.4 2007-11-12 16:39:04 gaede Exp $
   */
  class LCStdHepRdrNew {
    
  public:

	/** Open the stdhep input file in the constructer
	 */
    LCStdHepRdrNew(const char* evfile) ;

    /// no copy constructor
    LCStdHepRdrNew(const LCStdHepRdrNew&) = delete ;
    /// no assignment operator
    LCStdHepRdrNew& operator=(const LCStdHepRdrNew&) = delete ;

	/** noop
	 */
	~LCStdHepRdrNew() ;


	/** Get number of events in the stdhep file.
     *  This number is read from the file header (no guarantee that it is correct)
    */
    long getNumberOfEvents() const {
      return _reader->numEvents() ;
    }

	/** Get total number of expected events in the whole set of stdhep files
     *  from which this stdhep file belongs to.
     *  This number is read from the file header (no guarantee that it is correct)
    */
    long getNumberOfTotalEventsExpected() const {
      return _reader->numEventsExpected() ;
    }

    /** Read an event and return an LCCollectionVec of MCParticles.
     * @deprecated please use updateEvent()
     */
	IMPL::LCCollectionVec * readEvent() ;

    /** Reads the next stdhep event and adds a new MCParticle collection to the
     *  the event with default name 'MCParticle'
     * @throw IO::EndOfDataException if no event in stdhep file
     */
    void updateNextEvent( IMPL::LCEventImpl* evt , const char* colName=EVENT::LCIO::MCPARTICLE ) ;


    /** Print the file header to the given ostream.
     */
    void printHeader(std::ostream& os = std::cout ) ; 


    /** Return the charge of the particle times 3  - code copied from HepPDT package.
     */
    int threeCharge( int pdgID ) const;

    void setWriteEventNumber(bool writeEventNumber) {
        _writeEventNumber = writeEventNumber;
    }

  private:
    
    lStdHep* _reader{};
    bool _writeEventNumber{false};
    

  }; // class

} // namespace UTIL

#endif /* ifndef UTIL_LCStdHepRdr_H */
