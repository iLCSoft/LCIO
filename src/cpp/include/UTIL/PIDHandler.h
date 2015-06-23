#ifndef PIDHandler_h
#define PIDHandler_h 1

#include "UTIL/CollectionParameterMap.h"

#include "EVENT/LCCollection.h"
#include "Exceptions.h"

#include "IMPL/ParticleIDImpl.h"
#include "IMPL/ReconstructedParticleImpl.h"

#include <lcio.h>

#include <string>

using namespace lcio ;


namespace UTIL{
  
  
  /** Convenient class for setting and retrieving particle id information attached to
   *  ReconstructedParticles. 
   *  Use one instance of  PIDHandler for one LCCollection only.
   *  When used for setting particle id information the scope of the PIDHandler 
   *  must match that of the collection, as the collection parameters are updated automatically
   *  when the PIDHandler object goes out of scope.
   *  Reads (and updates) the collection parameters PIDAlgorithmTypeName, PIDAlgorithmTypeID 
   *  and the corresponding ParameterNames_PIDAlgorithmTypeName for every algorithm used. 
   * 
   *  @see ReconstructedParticle
   *  @see ParticleID
   *  @author F.Gaede, DESY
   *  @version $Id: PIDHandler.h,v 1.2 2010-01-21 16:28:25 gaede Exp $
   */
  class PIDHandler {

    typedef CollectionParameterMap CPM ;
    typedef std::map< int, StringVec > PNM ;
    typedef  std::map< CPM::map_type::mapped_type , CPM::map_type::key_type > CPMINV ;


  public:  
    
    /** Create PIDHandler for reading from the given collection - read the collection parameters
     *  PIDAlgorithmTypeName, PIDAlgorithmTypeID and the corresponding 
     *  ParameterNames_PIDAlgorithmTypeName if they exist.
     */
    PIDHandler( const LCCollection* col ) ;
    
    /** Create PIDHandler for setting PID information in the given collection 
     *  - read and update the collection parameters
     *  PIDAlgorithmTypeName, PIDAlgorithmTypeID and the corresponding 
     *  ParameterNames_PIDAlgorithmTypeName if they exist.
     */
    PIDHandler( LCCollection* col ) ;


    /** Update the collection parameter when going out of scope.
     */
    ~PIDHandler() ;

    /** Add a new algorithm and return the corresponding algorithm id.
     */
    int addAlgorithm( const std::string& algoName, const StringVec& parameterNames ) ;
    
    /** Return the typeID of algorithm algoName  - throws UnknownAlgorithm.
     */
    int getAlgorithmID( const std::string& algoName ) ;
    
    /** Return the name of the algorithm with id  - throws UnknownAlgorithm.
     */
    const std::string& getAlgorithmName(  int algoID ) ;

    /** The index of parameter pName for the algorithm with algorithmID - throws UnknownAlgoritm.
     */
    int getParameterIndex( int algorithmID, const std::string& pName ) ;
      
    /** Return the (first) ParticleID object for the given algorithm and particle (or cluster) - throws UnknownAlgorithm.
     *  Only use if you know there is only one PID object for the algorithms or if you simply want the most likely
     *  PID for this algorithm.
     */
    const ParticleID& getParticleID( LCObject* particle , int algorithmID ) ;
    

    /** Return all PID objects for a given algorithm - ordered with decreasing likelihood - throws UnknownAlgorithm.
     */
    ParticleIDVec getParticleIDs( LCObject* p , int id ) ;


    /** Set the particleID algorithm that is used for this particle's  kinematic variables 
     * - throws UnknownAlgorithm.
     */
    void setParticleIDUsed( IMPL::ReconstructedParticleImpl* particle , int algorithmID  ) ;


    /** The names of parameters for the algorithm with algorithmID - throws UnknownAlgoritm.
     */
    const StringVec&  getParameterNames( int algorithmID  ) ;
    
    /** Return the IDs of all known Algorithms.
     */
    const IntVec& getAlgorithmIDs() ;


    /** Set the particleID information for this particle (or cluster) - throws UnknownAlgorithm.
     */
    void setParticleID( LCObject* p ,
			int userType, 
			int PDG,
			float likelihood,
			int algorithmID, 
			const FloatVec& params
			) ;

  protected:

    int nextID() { return ++_maxID ; }
    void init( const LCCollection* col ) ;

    PIDHandler();
 
    LCCollection* _col ;
    CPM _cpm ;
    int _type ; 
    int _maxID ;
    PNM _pNames ; 
    CPMINV _cpmInv ;
    IntVec _ids ;

  } ; 
  

  /** Exception for unknown algorithms.
   */
  class UnknownAlgorithm : public Exception {
    
  protected:
    UnknownAlgorithm() {  /*no_op*/ ; } 
  public: 
    virtual ~UnknownAlgorithm() throw() { /*no_op*/; } 
    
    UnknownAlgorithm( std::string text ){
      message = "lcio::UnknownAlgorithm: " + text ;
    }
  }; 
 



} // namespace
#endif


