#ifndef EventSummary_h
#define EventSummary_h 1

#include "lcio.h"
#include "UTIL/LCFixedObject.h"


struct EvtSumI{
  const static int runnum = 0;
  const static int evtnum = 1;
  const static int pfonum = 2;
  const static int munum  = 3;
  const static int phonum = 4;
  const static int elnum  = 5;
  const static int jetnum = 6;
  const static int SIZE   = 7;
} ;

struct EvtSumF{
  const static int epfotot =0;
  const static int emcptot =1;
  const static int SIZE   = 2;
} ;

struct EvtSumD{
  const static int SIZE   = 0;
} ;

/** Event summary data stored as LCGenericObject.
 *  <p>
 *  
 *  
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
  

  // the class interface:
  void setRunNum(int runnum) { obj()->setIntVal(  EvtSumI::runnum , runnum); } 
  void setEventNum(int evtnum) { obj()->setIntVal(  EvtSumI::evtnum , evtnum); }
  void setPFONum(int pfonum) { obj()->setIntVal(  EvtSumI::pfonum , pfonum); }
  void setMuonNum (int munum ) { obj()->setIntVal(  EvtSumI::munum  , munum ); }
  void setPhotonNum(int phonum) { obj()->setIntVal(  EvtSumI::phonum , phonum); }
  void setElectronNum (int elnum ) { obj()->setIntVal(  EvtSumI::elnum  , elnum ); }
  void setJetNum (int jetnum ) { obj()->setIntVal(  EvtSumI::jetnum  , jetnum ); }

  void setEpfoTot(float epfotot ) { obj()->setFloatVal(  EvtSumF::epfotot  , epfotot); }
  void setEmcpTot(float emcptot ) { obj()->setFloatVal(  EvtSumF::emcptot  , emcptot); }

  int getRunNum() { return obj()->getIntVal(  EvtSumI::runnum ); } 
  int getEventNum() { return obj()->getIntVal(  EvtSumI::evtnum ); }
  int getPFONum() { return obj()->getIntVal(  EvtSumI::pfonum ); }
  int getMuonNum()  { return obj()->getIntVal(  EvtSumI::munum  ); }
  int getPhotonNum() { return obj()->getIntVal(  EvtSumI::phonum ); }
  int getElectronNum()  { return obj()->getIntVal(  EvtSumI::elnum  ); }
  /// number of jets if only one collection present, else -1
  int getJetNum()  { return obj()->getIntVal(  EvtSumI::jetnum  ); }
 
  float getEpfoTot() { return obj()->getFloatVal(  EvtSumF::epfotot ); }
  float getEmcpTot() { return obj()->getFloatVal(  EvtSumF::emcptot ); }

  void print(  std::ostream& os ) ;
  

  // -------- need to implement abstract methods from LCGenericObject

  const std::string getTypeName() const { 
    return"EventSummary" ;
  } 
  
  const std::string getDataDescription() const {
    return "i:runNum,i:eventNum,i:pfoNum,i:muonNum,i:photonNum,i:electronNum,i:jetNum,f:epfoTot,f:emcpTot" ;
  }

}; // class


#endif 
//=============================================================================
