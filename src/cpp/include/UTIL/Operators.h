#ifndef LCIO_OPERATORS_H
#define LCIO_OPERATORS_H 1

//#include "EVENT/LCCollection.h"
#include "EVENT/Vertex.h"
#include "EVENT/ReconstructedParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/MCParticle.h"
#include "EVENT/SimTrackerHit.h"
//#include "IMPL/MCParticleImpl.h"
#include "EVENT/TrackerHit.h"
#include "EVENT/ReconstructedParticle.h"
#include "EVENT/Track.h"
#include "EVENT/Cluster.h"


//#include "UTIL/IndexMap.h"

#include <iostream>
#include <iomanip>
#include <string>


//hauke
#include "EVENT/LCRelation.h"
#include "EVENT/LCFloatVec.h"

#include "EVENT/LCEvent.h"
//#include "EVENT/MCParticle.h"
//#include "EVENT/LCParameters.h"
//#include "EVENT/LCRunHeader.h"
//end hauke


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// override some operators to enable one to send objects to the output stream (e.g. cout << vertexObj << endl;
// 
// EXP: UNDER DEVELOPMENT!!!
//
// @author engels
// @version $Id: Operators.h,v 1.4 2010-06-22 13:49:55 gaede Exp $
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace UTIL{

  //template to enable the use of the operator << for a "one-line" output of an object.
  //since there are two ostream& << operators for sending an object to the output stream this
  //template works like a "wrapper class" so that the correct operator can be triggered for the
  //respective detailed/short output
  template <class T> struct lcio_short{
  const T* obj;
  lcio_short(const T* t) : obj(t) {}
  };

  //template to enable a "one-line" output of a vertex object
  template <class T> lcio_short<T> lcshort( const T* t){ return lcio_short<T>(t); }
 
/*
  // EXP: INDEX MAP - UNDER DEVELOPMENT
  
  template <class T, class T2> struct lcio_short{
  const T* obj;
  const T2* cobj;
  lcio_short(const T* t, const T2* t2) : obj(t),cobj(t2) {}
  };

  //template to enable a "one-line" output of a vertex object
  template <class T, class T2> lcio_short<T, T2> lcshort( const T* t, const T2* t2){ return lcio_short<T, T2>(t, t2); }
*/


  /** operator for detailed output of a vertex object (e.g. cout << vertexObj << endl; ) */
  /** for printing the header of a vertex object (e.g. cout << header(vertexObj) << endl; ) */
  /** for printing the tail of a vertex object (e.g. cout << tail(vertexObj) << endl; ) */

  const std::string& header(const EVENT::Vertex &);
  const std::string& tail(const EVENT::Vertex &);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Vertex>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::Vertex &v);


//hauke 2010 (start)
  const std::string& header( const EVENT::MCParticle &);
  const std::string& tail( const EVENT::MCParticle &);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::MCParticle>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::MCParticle &);


  const std::string& header( const EVENT::TrackerHit &);
  const std::string& tail( const EVENT::TrackerHit &);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerHit>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::TrackerHit &);

  const std::string& header( const EVENT::SimTrackerHit &);
  const std::string& tail( const EVENT::SimTrackerHit &);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::SimTrackerHit>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::SimTrackerHit &);

  const std::string& header( const EVENT::CalorimeterHit &);
  const std::string& tail( const EVENT::CalorimeterHit &);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::CalorimeterHit>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::CalorimeterHit &);

  const std::string& header( const EVENT::SimCalorimeterHit &);
  const std::string& tail( const EVENT::SimCalorimeterHit &);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::SimCalorimeterHit>& sV);
  std::ostream& operator<<( std::ostream& out, const EVENT::SimCalorimeterHit &);

  const std::string& header( const EVENT::ReconstructedParticle &);
  const std::string& tail( const EVENT::ReconstructedParticle &);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::ReconstructedParticle> & );
  std::ostream& operator<<( std::ostream& out, const EVENT::ReconstructedParticle &);

  const std::string& header( const EVENT::Track &);
  const std::string& tail( const EVENT::Track &);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Track> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::Track &);

  const std::string& header( const EVENT::Cluster &);
  const std::string& tail( const EVENT::Cluster &);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Cluster> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::Cluster &);

  const std::string& header( const EVENT::LCRelation &);
  const std::string& tail( const EVENT::LCRelation &);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCRelation> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCRelation &);

  const std::string& header( const EVENT::LCFloatVec &);
  const std::string& tail( const EVENT::LCFloatVec &);
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCFloatVec> &);
  std::ostream& operator<<( std::ostream& out, const EVENT::LCFloatVec &);

  template <class T> 
        const std::string & header(){return header((T)(0)); }
  template <class T> 
        const std::string & tail(){return tail((T)(0)); }


  template <class T>
  class LCIO_LONG{
        public:
            //lcio_long(const T& o, EVENT::LCCollection* c);
            LCIO_LONG(const T& o, const EVENT::LCCollection* c){ obj=&o; col=c;};
            const T *object(void) const {return(obj);};
            const EVENT::LCCollection *collection(void) const {return(col);};
        private: 
            const T *obj;
            const EVENT::LCCollection *col;
  };

  //test:
  template <class T>
  LCIO_LONG<T> lcio_long(const T& o, const EVENT::LCCollection* c){return(LCIO_LONG<T>(o,c));}


  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::CalorimeterHit> l);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Cluster> l);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Track> l);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::ReconstructedParticle> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::SimCalorimeterHit> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerHit> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::SimTrackerHit> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::MCParticle> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Vertex> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCRelation> ll);
  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCFloatVec> ll);



/*
  template <class T>
  std::ostream& operator<<( std::ostream& out, const T obj) {
    out << lcio_long<T>( obj , NULL ) ;
    return out;
  }
*/








//hauke 2010 (end)




//  std::ostream& operator<<( std::ostream& out, const EVENT::CalorimeterHit*  c);


  //std::ostream& operator<<( std::ostream& out, const EVENT::TrackerHit*);
//  std::ostream& operator<<( std::ostream& out, const EVENT::MCParticleImpl*);
  
  /** operator for short output of a vertex object (e.g. cout << lcshort(vertexObj) << endl; ) */
 
  //EXP: INDEX MAP - UNDER DEVELOPMENT
  //std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Vertex, EVENT::LCCollection>& sV);
  

  //deprecated
  //std::string brief( const EVENT::Vertex* v);
  

}//namespace

#endif /* ifndef LCIO_OPERATORS_H */
