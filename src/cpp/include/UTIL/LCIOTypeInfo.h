#ifndef LCIOTypeInfo_h
#define LCIOTypeInfo_h 1

#include "EVENT/LCIO.h"
#include "EVENT/LCObject.h"
#include "EVENT/LCCollection.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/CalorimeterHit.h"
#include "EVENT/RawCalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"
#include "EVENT/TPCHit.h"
#include "EVENT/TrackerRawData.h"
#include "EVENT/TrackerData.h"
#include "EVENT/TrackerPulse.h"
#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCFloatVec.h"
#include "EVENT/LCIntVec.h"
#include "EVENT/LCStrVec.h"
#include "EVENT/Track.h"
#include "EVENT/Cluster.h"
#include "EVENT/ReconstructedParticle.h"
#include "EVENT/LCGenericObject.h"
#include "EVENT/LCRelation.h"
#include "EVENT/Vertex.h"

#include <vector>
#include <sstream>
#include <typeinfo>


namespace UTIL{

  /** Template that returns the LCIO type name as used in the LCCollctions (and files), e.g. <br>
   *  std::cout << lctypename<MCParticle>() << std::endl ; <br>
   *  This can be used in user templates to create a new collection of the template type.
   */
  template <class T>
  const char*  lctypename() ;

  /** Specialization for implemention classes.
   */
  template <class T>
  const char*  lctypename() {
    return lctypename< typename T::lcobject_type >() ;
  }

  // specializations for all known LCObject subclasses 
  // - yes this is not OO at its best ...
  template<> const char* lctypename<EVENT::MCParticle>() ; 
  template<> const char* lctypename<EVENT::SimCalorimeterHit>() ; 
  template<> const char* lctypename<EVENT::CalorimeterHit>()   ;  
  template<> const char* lctypename<EVENT::RawCalorimeterHit>()  ;
  template<> const char* lctypename<EVENT::SimTrackerHit>()    ;  
  template<> const char* lctypename<EVENT::TrackerRawData>()  ;   
  template<> const char* lctypename<EVENT::TrackerData>()  ;      
  template<> const char* lctypename<EVENT::TrackerPulse>() ; 
  template<> const char* lctypename<EVENT::TrackerHit>() ; 
  template<> const char* lctypename<EVENT::LCStrVec>()  ;
  template<> const char* lctypename<EVENT::LCFloatVec>() ; 
  template<> const char* lctypename<EVENT::LCIntVec>()  ;
  template<> const char* lctypename<EVENT::Track>()  ;
  template<> const char* lctypename<EVENT::Cluster>()  ;
  template<> const char* lctypename<EVENT::ReconstructedParticle>()  ;
  template<> const char* lctypename<EVENT::LCRelation>()  ;
  template<> const char* lctypename<EVENT::LCGenericObject>()  ;
  template<> const char* lctypename<EVENT::Vertex>()  ;
  
  
  /** Template that returns the LCIO type name as used in the LCCollctions (and files) based on 
   *  the object's type, e.g. <br>
   *  MCParticle* mcp ; <br>
   *  std::cout << lctypename( mcp ) << std::endl ; <br>
   */
  template <class T>
  const char*  lctypename(const T* t) {
    return lctypename< typename T::lcobject_type >() ;
  }


  /** Specialization for polymorphic LCObject pointers - slow. E.g.: <br>
   * 
   *  LCObject* obj = new McParticle ; <br>
   *  std::cout << lctypename( obj ) << std::endl ; <br>
   */
  template<> const char* lctypename<EVENT::LCObject>(const EVENT::LCObject* o) ;




//   /** Provides some type information for LCIO classes.
//    * 
//    *  @author gaede
//    *  @version Id:$
//    */
//   template <class T> 
//   class LCIOTypeInfo {
    
//     const T* _t ;
//   public:  

//     typedef T lcobject_type ;

//     LCIOTypeInfo(const T* t) : _t(t) {}

//     /** Allow conversions to base class'  LCIOTypeInfo 
//      */
//     template<class T2> 
//     operator LCIOTypeInfo<T2>() { return  LCIOTypeInfo<T2>( _t ) ; } 
    
//     const char* lctypename() {
//       return UTIL::lctypename<T>() ;
//     }
    
//   private:
//     void ensure_T_is_LCObject() { const EVENT::LCObject* o = t ; } 
//   } ; 
  

} // namespace
#endif
