#ifndef LCIO_OPERATORS_H
#define LCIO_OPERATORS_H 1

//#include "EVENT/LCCollection.h"
#include "EVENT/Vertex.h"
#include "EVENT/ReconstructedParticle.h"

//#include "UTIL/IndexMap.h"

#include <iostream>
#include <iomanip>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// override some operators to enable one to send objects to the output stream (e.g. cout << vertexObj << endl;
// 
// EXP: UNDER DEVELOPMENT!!!
//
// @author engels
// @version $Id: Operators.h,v 1.1.2.3 2006-09-11 13:05:30 engels Exp $
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
  std::ostream& operator<<( std::ostream& out, const EVENT::Vertex* v);
  
  /** operator for short output of a vertex object (e.g. cout << lcshort(vertexObj) << endl; ) */
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Vertex>& sV);
 
  //EXP: INDEX MAP - UNDER DEVELOPMENT
  //std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Vertex, EVENT::LCCollection>& sV);
  
  //deprecated
  //std::string brief( const EVENT::Vertex* v);
  
  /** for printing the header of a vertex object (e.g. cout << header(vertexObj) << endl; ) */
  const std::string& header( const EVENT::Vertex* v);
  
  /** for printing the tail of a vertex object (e.g. cout << tail(vertexObj) << endl; ) */
  const std::string& tail( const EVENT::Vertex* v);

}//namespace

#endif /* ifndef LCIO_OPERATORS_H */
