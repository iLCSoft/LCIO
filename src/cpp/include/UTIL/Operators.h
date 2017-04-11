#ifndef LCIO_OPERATORS_H
#define LCIO_OPERATORS_H 1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define operators for convenient printing of LCIO objects.
//
//
// @author J Engels, H. Hoelbe
// @version $Id: Operators.h,v 1.8.2.1 2011-03-04 14:09:07 engels Exp $
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "EVENT/ReconstructedParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/MCParticle.h"
#include "EVENT/SimTrackerHit.h"
#include "EVENT/TrackerHit.h"
#include "EVENT/TrackerHitPlane.h"
#include "EVENT/TrackerHitZCylinder.h"
#include "EVENT/Track.h"
#include "EVENT/TrackState.h"
#include "EVENT/Cluster.h"
#include "EVENT/CalorimeterHit.h"
#include "EVENT/LCFlag.h"
#include "EVENT/LCIntVec.h"
#include "EVENT/LCRunHeader.h"
#include "EVENT/RawCalorimeterHit.h"
//#include "EVENT/TPCHit.h"
#include "EVENT/TrackerPulse.h"
#include "EVENT/LCFloatVec.h"
#include "EVENT/LCObject.h"
#include "EVENT/TrackerRawData.h"
#include "EVENT/LCCollection.h"
#include "EVENT/LCGenericObject.h"
#include "EVENT/LCParameters.h"
#include "EVENT/TrackerData.h"
#include "EVENT/LCEvent.h"
#include "EVENT/LCIO.h"
#include "EVENT/LCRelation.h"
#include "EVENT/ParticleID.h"
#include "EVENT/Vertex.h"
//#include "EVENT/LCStrVec.h"


#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>


namespace UTIL{

  //template to enable the use of the operator << for a "one-line" output of an object.
  //since there are two ostream& << operators for sending an object to the output stream this
  //template works like a "wrapper class" so that the correct operator can be triggered for the
  //respective detailed/short output
/*
  template <class T> struct lcio_short{
    const T* obj;
    lcio_short(const T* t) : obj(p) {}
  };
*/
  template <class T> 
    class lcio_short{
        public:
            const T* obj;
            const EVENT::LCCollection* col;
            lcio_short(const T* t, const EVENT::LCCollection* b=NULL) : obj(t), col(b) {}
            //lcio_short(const T* t) : obj(t) {col = NULL;}
    };


  //template to enable a "one-line" output 
  template <class T> lcio_short<T> lcshort( const T* t, const EVENT::LCCollection* b=NULL){ return lcio_short<T>(t,b); }

  /** operator for detailed output of a vertex object (e.g. cout << vertexObj << endl; ) */
  /** for printing the header of a vertex object (e.g. cout << header(vertexObj) << endl; ) */
  /** for printing the tail of a vertex object (e.g. cout << tail(vertexObj) << endl; ) */

  const std::string& header(const EVENT::Vertex *);
  const std::string& tail(const EVENT::Vertex *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Vertex>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::Vertex &v);


//hauke 2010 (start)
  const std::string& header( const EVENT::MCParticle *);
  const std::string& tail( const EVENT::MCParticle *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::MCParticle>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::MCParticle &);


  const std::string& header( const EVENT::TrackerHit *);
  const std::string& tail( const EVENT::TrackerHit *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerHit>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::TrackerHit &);

  const std::string& header( const EVENT::TrackerHitPlane *);
  const std::string& tail( const EVENT::TrackerHitPlane *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerHitPlane>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::TrackerHitPlane &);

  const std::string& header( const EVENT::TrackerHitZCylinder *);
  const std::string& tail( const EVENT::TrackerHitZCylinder *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerHitZCylinder>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::TrackerHitZCylinder &);

  const std::string& header( const EVENT::SimTrackerHit *);
  const std::string& tail( const EVENT::SimTrackerHit *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::SimTrackerHit>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::SimTrackerHit &);

  const std::string& header( const EVENT::CalorimeterHit *);
  const std::string& tail( const EVENT::CalorimeterHit *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::CalorimeterHit>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::CalorimeterHit &);

  const std::string& header( const EVENT::SimCalorimeterHit *);
  const std::string& tail( const EVENT::SimCalorimeterHit *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::SimCalorimeterHit>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::SimCalorimeterHit &);

  const std::string& header( const EVENT::ReconstructedParticle *);
  const std::string& tail( const EVENT::ReconstructedParticle *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::ReconstructedParticle> & );
  std::ostream& operator<<( std::ostream& out, const EVENT::ReconstructedParticle &);

  const std::string& header( const EVENT::Track *);
  const std::string& tail( const EVENT::Track *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Track> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::Track &);

  const std::string& header( const EVENT::TrackState *);
  const std::string& tail( const EVENT::TrackState *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackState> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::TrackState &);

  const std::string& header( const EVENT::Cluster *);
  const std::string& tail( const EVENT::Cluster *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Cluster> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::Cluster &);

  const std::string& header( const EVENT::LCRelation *);
  const std::string& tail( const EVENT::LCRelation *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCRelation> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCRelation &);

  const std::string& header( const EVENT::LCFloatVec *);
  const std::string& tail( const EVENT::LCFloatVec *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCFloatVec> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCFloatVec &);

  template <class T> 
        const std::string & header(){return header((T*)(0)); }
  template <class T> 
        const std::string & tail(){return tail((T)(0)); }


  template <class T>
  class LCIO_LONG{
        public:
            LCIO_LONG() = default ;
            LCIO_LONG(const LCIO_LONG& ) = default ;
            LCIO_LONG& operator=(const LCIO_LONG& ) = default ;
            LCIO_LONG(const T& o, const EVENT::LCCollection* c){ obj=&o; col=c;};
            const T *object(void) const {return(obj);};
            const EVENT::LCCollection *collection(void) const {return(col);};
        private: 
            const T *obj{NULL};
            const EVENT::LCCollection *col{NULL};
  };

  //test:
  template <class T>
  LCIO_LONG<T> lcio_long(const T& o, const EVENT::LCCollection* c){return(LCIO_LONG<T>(o,c));}


  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::CalorimeterHit> l);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Cluster> l);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Track> l);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackState> l);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::ReconstructedParticle> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::SimCalorimeterHit> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerHit> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerHitPlane> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerHitZCylinder> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::SimTrackerHit> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::MCParticle> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Vertex> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCRelation> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCFloatVec> ll);


  const std::string& header( const EVENT::LCCollection *);
  const std::string& tail( const EVENT::LCCollection *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCCollection> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCCollection &);

  const std::string& header( const EVENT::LCEvent *);
  const std::string& tail( const EVENT::LCEvent *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCEvent> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCEvent &);

  const std::string& header( const EVENT::LCFlag *);
  const std::string& tail( const EVENT::LCFlag *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCFlag> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCFlag &);

  const std::string& header( const EVENT::LCGenericObject *, const EVENT::LCCollection *v = NULL);
  const std::string& tail( const EVENT::LCGenericObject *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCGenericObject> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCGenericObject &);

  const std::string& header( const EVENT::LCIntVec *);
  const std::string& tail( const EVENT::LCIntVec *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCIntVec> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCIntVec &);

  const std::string& header( const EVENT::LCObject *);
  const std::string& tail( const EVENT::LCObject *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCObject> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCObject &);

  const std::string& header( const EVENT::LCParameters *);
  const std::string& tail( const EVENT::LCParameters *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCParameters> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCParameters &);

  const std::string& header( const EVENT::LCRunHeader *);
  const std::string& tail( const EVENT::LCRunHeader *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCRunHeader> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCRunHeader &);

/*
  const std::string& header( const EVENT::LCStrVec &);
  const std::string& tail( const EVENT::LCStrVec &);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCStrVec> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCStrVec &);
*/

  const std::string& header( const EVENT::ParticleID *);
  const std::string& tail( const EVENT::ParticleID *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::ParticleID> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::ParticleID &);

  const std::string& header( const EVENT::RawCalorimeterHit *);
  const std::string& tail( const EVENT::RawCalorimeterHit *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::RawCalorimeterHit> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::RawCalorimeterHit &);

//  const std::string& header( const EVENT::TPCHit &);
//  const std::string& tail( const EVENT::TPCHit &);
//  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TPCHit> &);
//  std::ostream& operator<<( std::ostream& out, const EVENT::TPCHit &);

  const std::string& header( const EVENT::TrackerData *);
  const std::string& tail( const EVENT::TrackerData *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerData> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::TrackerData &);

  const std::string& header( const EVENT::TrackerPulse *);
  const std::string& tail( const EVENT::TrackerPulse *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerPulse> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::TrackerPulse &);

  const std::string& header( const EVENT::TrackerRawData *);
  const std::string& tail( const EVENT::TrackerRawData *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerRawData> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::TrackerRawData &);

  const std::string& header( const EVENT::LCIO *);
  const std::string& tail( const EVENT::LCIO *);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCIO> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCIO &);



/*
  template <class T>
  std::ostream& operator<<( std::ostream& out, const T obj) {
    out << lcio_long<T>( obj , NULL ) ;
    return out;
  }
*/

  
  template <class T>
  std::string toString(const T* obj) {
    std::stringstream ss ;
    ss << *obj ;
    return ss.str() ;
  }

}//namespace

#endif /* ifndef LCIO_OPERATORS_H */
