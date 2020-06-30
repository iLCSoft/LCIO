#ifndef EventSummary_h
#define EventSummary_h 1

#include "lcio.h"
#include "UTIL/LCFixedObject.h"

#include "UTIL/ProcessFlag.h"


namespace UTIL {

  /// helper struct with int values for EventSummary
  struct EvtSumI{
    const static int runnum    = 0;
    const static int evtnum    = 1;
    const static int chpfonum  = 2;
    const static int neupfonum = 3;
    const static int elnum     = 4;
    const static int munum     = 5;
    const static int taunum    = 6;
    const static int phonum    = 7;
    const static int jetnum    = 8;
    const static int SIZE      = 9;
  } ;

  /// helper struct with float values for EventSummary
  struct EvtSumF{
    const static int epfotot =0;
    const static int emcptot =1;
    const static int SIZE   = 2;
  } ;

  /// helper struct with double values for EventSummary
  struct EvtSumD{
    const static int SIZE   = 0;
  } ;

/** Utility class for storing an event summary as an LCGenericObject.
 *  
 *  \author F.Gaede, DESY
 *  \date June 2020
 */
  class EventSummary : public UTIL::LCFixedObject<EvtSumI::SIZE,EvtSumF::SIZE,EvtSumD::SIZE> {
  
  public: 
  
    /** Convenient c'tor.
     */
    EventSummary() = default ;

    /** 'Copy constructor' needed to interpret LCCollection read from file/database.
     */
    EventSummary(LCObject* o) : UTIL::LCFixedObject<EvtSumI::SIZE,EvtSumF::SIZE,EvtSumD::SIZE>(o) { }

    /** Important for memory handling*/
    virtual ~EventSummary() { /* no op*/  }
  

   // ------  the class interface ------------------------------------
    /// run number
    int getRunNum() { return obj()->getIntVal(  EvtSumI::runnum ); } 

    /// event number
    int getEventNum() { return obj()->getIntVal(  EvtSumI::evtnum ); }

    /// number of charged PFOs
    int getChargedPFONum() { return obj()->getIntVal(  EvtSumI::chpfonum ); }

    /// number of neutral PFOs
    int getNeutralPFONum() { return obj()->getIntVal(  EvtSumI::neupfonum ); }

    /// number of isolated electrons
    int getElectronNum()  { return obj()->getIntVal(  EvtSumI::elnum  ); }

    /// number of isolated muons
    int getMuonNum()  { return obj()->getIntVal(  EvtSumI::munum  ); }

    /// number of isolated taus
    int getTauNum()  { return obj()->getIntVal(  EvtSumI::taunum ); }

    /// number of isolated photons
    int getPhotonNum() { return obj()->getIntVal(  EvtSumI::phonum ); }

    /// number of jets if only one collection present, else -1
    int getJetNum()  { return obj()->getIntVal(  EvtSumI::jetnum  ); }
 
    /// total reconstructed energy
    float getEpfoTot() { return obj()->getFloatVal(  EvtSumF::epfotot ); }

    /// total visible MC truth energy
    float getEmcpTot() { return obj()->getFloatVal(  EvtSumF::emcptot ); }

    // -------- setters -------------------------
    void setRunNum(int runnum) { obj()->setIntVal(  EvtSumI::runnum , runnum); } 
    void setEventNum(int evtnum) { obj()->setIntVal(  EvtSumI::evtnum , evtnum); }
    void setChargedPFONum(int pfonum) { obj()->setIntVal(  EvtSumI::chpfonum , pfonum); }
    void setNeutralPFONum(int pfonum) { obj()->setIntVal(  EvtSumI::neupfonum , pfonum); }
    void setElectronNum (int elnum ) { obj()->setIntVal(  EvtSumI::elnum  , elnum ); }
    void setMuonNum (int munum ) { obj()->setIntVal(  EvtSumI::munum  , munum ); }
    void setTauNum (int taunum ) { obj()->setIntVal(  EvtSumI::taunum  , taunum ); }
    void setPhotonNum(int phonum) { obj()->setIntVal(  EvtSumI::phonum , phonum); }
    void setJetNum (int jetnum ) { obj()->setIntVal(  EvtSumI::jetnum  , jetnum ); }

    void setEpfoTot(float epfotot ) { obj()->setFloatVal(  EvtSumF::epfotot  , epfotot); }
    void setEmcpTot(float emcptot ) { obj()->setFloatVal(  EvtSumF::emcptot  , emcptot); }


    void print(  std::ostream& os ) ;
  

    // -------- need to implement abstract methods from LCGenericObject

    const std::string getTypeName() const { 
      return"EventSummary" ;
    } 
  
    const std::string getDataDescription() const {
      return "i:runNum,i:eventNum,i:pfoNum,i:muonNum,i:photonNum,i:electronNum,i:jetNum,f:epfoTot,f:emcpTot" ;
    }

  }; // class

}
#endif 
//=============================================================================
