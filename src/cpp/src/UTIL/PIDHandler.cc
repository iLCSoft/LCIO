#include "UTIL/PIDHandler.h"
#include "Exceptions.h"

#include "IMPL/ReconstructedParticleImpl.h"
#include "IMPL/ClusterImpl.h"

#include <sstream>

using namespace IMPL;

namespace UTIL{


  enum ObjectType {
    Cluster = 1 ,
    ReconstructedParticle  
  } ;

  
  PIDHandler::PIDHandler( LCCollection* col ) : 
    _col( col) , 
    _cpm( "PIDAlgorithmTypeName",  "PIDAlgorithmTypeID" ,  col ),
    _type(-1) , 
    _maxID(-1) {
    
    init( col ) ;
  }
  
  
  PIDHandler::PIDHandler(  const LCCollection* col ) : 
    _col( 0 ) , // not needed 
    _cpm( "PIDAlgorithmTypeName",  "PIDAlgorithmTypeID" ,  col ),
    _type(-1) , 
    _maxID(-1) {
    
    init( col ) ;
  }
  

 void  PIDHandler::init(  const LCCollection* col ) {
    

    // ---- get the information on existing ParticleID objects ----------


    for( CPM::map_type::iterator it = _cpm.map().begin() ; it != _cpm.map().end() ; ++it) {
      
      int id = it->second ;
     
      const std::string& aName =  it->first ;

      if( id > _maxID )
	_maxID = id ;

      // ensure id is unique for collection
      PNM::iterator nit = _pNames.find( id ) ;

      if( nit != _pNames.end() ){

	std::stringstream sstr ;

	sstr << " PIDHandler::PIDHandler() - duplicate algorithm type IDs: "
	     <<  aName << " [" << id << "] " ;
	
	throw Exception( sstr.str() ) ;

      }

      // get parameter names

      StringVec pNames ;

      col->getParameters().getStringVals( std::string( "ParameterNames_" + aName   ) , pNames ) ;

      _pNames[ id ] = pNames ;  // save a copy of parameter names for algorithm with id

      _ids.push_back( id ) ;

      //   save inverse map for lookup algoName from algoID

      _cpmInv.insert( std::make_pair( id  , aName )  ) ;

    } 

    //------------------------------------------------

    // determine LCCollection type 
    std::string colType = col->getTypeName()  ;
    
    //  arghhh ...
    if( colType == LCIO::RECONSTRUCTEDPARTICLE ) {
      
      _type = ReconstructedParticle ;
      
    } else if( colType == LCIO::CLUSTER ){
      
      _type = Cluster ;
      
    } else {
      
      throw Exception( "PIDHandler::PIDHandler() " 
		       " collection type is neither Cluster nor ReconstructedParticle ") ;
    }
  }
  
  


  PIDHandler::~PIDHandler() {
  

    if (_col != 0 ) {
      // save the collection parameters
      
      for( PNM::iterator pnm = _pNames.begin() ; pnm != _pNames.end() ; ++pnm) {
	
	int id = pnm->first ;
	
	
	_col->parameters().setValues( std::string( "ParameterNames_" + _cpmInv[id]  ) , 
				      pnm->second ) ;
      }
    }
  }
  
  int PIDHandler::addAlgorithm( const std::string& algoName, const StringVec& pNames ) {
    
    CPM::map_type& map = _cpm.map() ;
    
    CPM::map_type::iterator it = map.find( algoName ) ; 
    
    if( it != map.end() ){
      
      std::stringstream sstr ;
      
      sstr << " PIDHandler::addAlgorithm() - duplicate algorithm name: "
	   <<  algoName ; 
      
      throw Exception( sstr.str() ) ;
    }
    
    int id  =  nextID() ;

    map.insert( std::make_pair( algoName , id ) ) ;
    
    // inverse map:
    _cpmInv.insert( std::make_pair( id  , algoName )  ) ;

    // save parameter names
    _pNames[ id ] = pNames ;      

    _ids.push_back( id ) ;

    return id ;
  }
  
  int PIDHandler::getAlgorithmID( const std::string& algoName ) {

    CPM::map_type::iterator it = _cpm.map().find( algoName ) ; 
    
    if( it == _cpm.map().end() ){

      throw UnknownAlgorithm(  algoName ) ;
    }

    return it->second ;
  }
  
  const std::string& PIDHandler::getAlgorithmName(  int algoID ) {
    
    CPMINV::iterator it = _cpmInv.find( algoID ) ; 

    if( it == _cpmInv.end() ){
      
      throw UnknownAlgorithm(  std::string(""+algoID) ) ;
    }
    
    return it->second ;
  }
  


  int PIDHandler::getParameterIndex( int algorithmID, const std::string& name ) {
    
    
    PNM::iterator nit = _pNames.find( algorithmID ) ;
    
    if( nit == _pNames.end() ){

      throw UnknownAlgorithm( std::string(""+algorithmID) ) ;
    }
    // brute force search:
    
    const StringVec& names =  nit->second ;

    unsigned n = names.size() ;

    for(unsigned i=0 ; i<n ; ++i){

      if( names[i] == name ) 

	return i ;
    }

    return -1 ; // or better throw sth. ?
  }
  
  
  const StringVec&  PIDHandler::getParameterNames( int id  ) {

    PNM::iterator nit = _pNames.find( id ) ;
    
    if( nit == _pNames.end() ){

      throw UnknownAlgorithm( std::string(""+id ) ) ;
    }

    return nit->second ;
  }
    
  const IntVec& PIDHandler::getAlgorithmIDs() {

    return _ids ;
  }



  void PIDHandler::setParticleIDUsed(  ReconstructedParticleImpl* p , int id  ) {

    PNM::iterator nit = _pNames.find( id ) ;
    
    if( nit == _pNames.end() ){

      throw UnknownAlgorithm( std::string(""+id ) ) ;
    }

    ParticleID* pid = 0 ;
    
//     const ParticleIDVec* idv = 0 ;	 
//     if( _type == ReconstructedParticle  ){
      
//       idv = &( static_cast< ReconstructedParticleImpl* >(p)->getParticleIDs()  ) ; 
//     }
//     else if( _type == Cluster  ){
      
//       idv = &( static_cast< ClusterImpl* >(p)->getParticleIDs()  ) ; 
//     }
//     const ParticleIDVec& pidV = *idv ;	 

    const ParticleIDVec& pidV = p->getParticleIDs() ;
    
    unsigned nPid = pidV.size() ;
    
    for(unsigned i=0; i<nPid; ++i ) {
      
      if( pidV[i]->getAlgorithmType() == id ) {
	
	pid = pidV[i]  ;
	break ;
	
      }
    }
    
    if( pid == 0 ) {

      throw UnknownAlgorithm( std::string("pid object not found in particle for algorithmId: "+id ) ) ;
    }

    p->setParticleIDUsed( pid ) ;

  }


  const ParticleID& PIDHandler::getParticleID( LCObject* p , int id ) {

    PNM::iterator nit = _pNames.find( id ) ;
    
    if( nit == _pNames.end() ){

      throw UnknownAlgorithm( std::string(""+id ) ) ;
    }

    
    const ParticleIDVec* idv = 0 ;	 

    if( _type == ReconstructedParticle  ){
      
      idv = &( static_cast< ReconstructedParticleImpl* >(p)->getParticleIDs()  ) ; 
    }
    else if( _type == Cluster  ){
      
      idv = &( static_cast< ClusterImpl* >(p)->getParticleIDs()  ) ; 
    }

    const ParticleIDVec& pidV = *idv ;	 

    unsigned nPid = pidV.size() ;
    
    for(unsigned i=0; i<nPid; ++i ) {

      if( pidV[i]->getAlgorithmType() == id ) 
	
	return *pidV[i] ;

    }
	
    // not returned, i.e. we need to create a new pid objects

    ParticleIDImpl* pid = new ParticleIDImpl ;


    if( _type == ReconstructedParticle  ){
      
      static_cast< ReconstructedParticleImpl* >(p)->addParticleID( pid ) ; 
    }
    else if( _type == Cluster  ){
      
      static_cast< ClusterImpl* >(p)->addParticleID( pid )  ; 
    }


    return *pid ;


  }
  
  void PIDHandler::setParticleID( LCObject* p ,
				  int userType, 
				  int PDG,
				  float likelihood,
				  int id, 
				  const FloatVec& params ) {
    

    PNM::iterator nit = _pNames.find( id ) ;
    
    if( nit == _pNames.end() ){
      
      throw UnknownAlgorithm( std::string(""+id ) ) ;
    }
    

    // ---- check paramaters size -----
    unsigned nParam =  params.size()  ;

    if( nParam != _pNames[ id ].size() ) {
      
      std::stringstream sstr ;
      
      sstr << " PIDHandler::setParticleID() - wrong parmeter size specified: "
	   <<  nParam << " - expected " << _pNames[ id ].size()  ; 
      
      throw Exception( sstr.str() ) ;

    }

    ParticleIDImpl* pid = 0 ;
    
    const ParticleIDVec* idv = 0 ;	 
    
    if( _type == ReconstructedParticle  ){
      
      idv = &( static_cast< ReconstructedParticleImpl* >(p)->getParticleIDs()  ) ; 
    }
    else if( _type == Cluster  ){
      
      idv = &( static_cast< ClusterImpl* >(p)->getParticleIDs()  ) ; 
    }
    
    const ParticleIDVec& pidV = *idv ;	 
    
    unsigned nPid = pidV.size() ;
    
    for(unsigned i=0; i<nPid; ++i ) {
      
      if( pidV[i]->getAlgorithmType() == id ) {
	
	pid = static_cast<ParticleIDImpl*>( pidV[i] ) ;
	break ;
	
      }
    }
    
    // if nothing found we create a new object
    
    bool isNewPID = false ;
    
    if( pid == 0 ) {

      pid = new ParticleIDImpl ;

      isNewPID = true ;

    }

    // ---- now set the PID data ------------
    
    pid->setLikelihood( likelihood ) ;
    
    pid->setType( userType ) ;
    
    pid->setPDG( PDG ) ;
    
    pid->setAlgorithmType( id ) ;
    
    FloatVec& ps = pid->parameters() ;
    
    ps.resize( nParam ) ;
    
    for(unsigned k=0; k< nParam  ; k++){
      
      ps[ k ] = params[ k ] ;
    }
    
    // ----------------------------------------


    if( isNewPID ) { // need to add it to the particle/cluster

      if( _type == ReconstructedParticle  ){
	
	static_cast< ReconstructedParticleImpl* >(p)->addParticleID( pid ) ; 
      }
      else if( _type == Cluster  ){
	
	static_cast< ClusterImpl* >(p)->addParticleID( pid )  ; 
      }
    }
    
  }
}
