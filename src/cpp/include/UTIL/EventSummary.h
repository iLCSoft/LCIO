#ifndef EventSummary_h
#define EventSummary_h 1

#include "lcio.h"
#include "UTIL/LCFixedObject.h"

#include "UTIL/ProcessFlag.h"


namespace UTIL {

  /// enum class defining valid int values in EventSummary
  enum struct EventSummaryIntValues {
    runnum    , ///< run   number
    evtnum    , ///< event number
    chpfonum  , ///< number of charged PFOs
    neupfonum , ///< number of neutral PFOs
    elnum     , ///< number of isloted electrons
    munum     , ///< number of isloted muons
    taunum    , ///< number of isloted taus
    phonum    , ///< number of isloted photons
    jetnum    , ///< number of jets - if only one jet collection, -1 else
    bcpfonum  , ///< number of PFOs in the BeamCal
    trknum    , ///< number of tracks - if present
    clunum    , ///< number of clusters - if present
    mcproc    , ///< generated Monte Carlo ProcessFlag
    SIZE        ///< internal use only
  } ;

  /// Short name for EventSummaryIntValues
  typedef EventSummaryIntValues ESI ;

  /// enum class defining valid float values in EventSummary
  enum struct EventSummaryFloatValues {
    epfotot , ///< total reconstructed energy
    emcptot , ///< total visible true energy (exluding neutrinos)
    thrust  , ///< thrust value of the event
    spher   , ///< spericity value of the event
    emiss   , ///< missing energy
    pxmiss  , ///< missing momentum in x
    pymiss  , ///< missing momentum in y
    pzmiss  , ///< missing momentum in z
    y12     , ///< y12 flip value for exclusive jet clustering
    y23     , ///< y23 flip value for exclusive jet clustering
    y34     , ///< y34 flip value for exclusive jet clustering
    y45     , ///< y45 flip value for exclusive jet clustering
    y56     , ///< y56 flip value for exclusive jet clustering
    y67     , ///< y67 flip value for exclusive jet clustering
    y78     , ///< y78 flip value for exclusive jet clustering
    SIZE  ///< internal use only
  } ;

  /// Short name for EventSummaryFloatValues
  typedef EventSummaryFloatValues ESF ;

  /// helper struct with double values for EventSummary
  enum struct EventSummaryDoubleValues {
    SIZE  ///< internal use only
  } ;
  /// Short name for EventSummaryDoubleValues
  typedef EventSummaryFloatValues ESD ;



/** Utility class for storing an event summary as an LCGenericObject.
 *  
 *  \author F.Gaede, DESY
 *  \date June 2020
 */
  class EventSummary : public UTIL::LCFixedObject<int(ESI::SIZE),int(ESF::SIZE),int(ESD::SIZE)> {
  
  public: 
  
    /** Convenient c'tor.
     */
    EventSummary() = default ;

    /** 'Copy constructor' needed to interpret LCCollection read from file/database.
     */
    EventSummary(LCObject* o) : UTIL::LCFixedObject<int(ESI::SIZE),int(ESF::SIZE),int(ESD::SIZE)>( o ){ }

    /** Important for memory handling*/
    virtual ~EventSummary() { /* no op*/  }
  
// ------  the class interface ------------------------------------

    /** return the specified int value - see EventSummaryIntValues for possible values, for example
     * getI( ESI::chpfonum ) returns the number of charged PFOs in the event.
     */
    int getI( ESI intval) { return obj()->getIntVal( (int) intval ); }

    /** return the specified float value - see EventSummaryFloatValues for possible values, for example
     * getF( ESF::thrust ) returns the thrust value of the event.
     */
    float getF( ESF floatval) { return obj()->getFloatVal( (int) floatval ); }

    /** return the specified double value - see EventSummaryDoubleValues for possible values, not
     *  used currently.
     */
    double getD( ESD doubleval) { return obj()->getDoubleVal( (int) doubleval ); }

    /** set the specified int value - see EventSummaryIntValues for possible values, for example
     * set( ESI::chpfonum , 42 ) sets the number of charged PFOs in the event.
     */
    void setI( ESI intval, int val )  { obj()->setIntVal( (int) intval , val); }

    /** set the specified float value - see EventSummaryFloatValues for possible values, for example
     * set( ESF::thrust , .7 ) sets the thrust value the event.
     */
    void setF( ESF floatval, float val )  { obj()->setFloatVal( (int) floatval , val); }

    /** set the specified double value - see EventSummaryDoubleValues for possible values, not used
     *  currently
     */
    void setD( ESD doubleval, double val )  { obj()->setDoubleVal( (int) doubleval , val); }

    // -----------------------------------------------------------

    void print(  std::ostream& os ) ;
  

    // -------- need to implement abstract methods from LCGenericObject

    const std::string getTypeName() const { 
      return"EventSummary" ;
    } 
  
    const std::string getDataDescription() const {
      return "i:runnum,i:evtnum,i:chpfonum,i:neupfonum,i:elnum,i:munum,i:taunum,i:phonum,i:jetnum,i:bcpfonum,i:trknum,i:clunum,i:mcproc,f:epfotot,f:emcptot,f:thrust,f:spher,f:emiss,f:pxmiss,f:pymiss,f:pzmiss,f:y12,f:y23,f:y34,f:y45,f:y56,f:y67,f:y78" ;
    }


  }; // class

}
#endif 
//=============================================================================
