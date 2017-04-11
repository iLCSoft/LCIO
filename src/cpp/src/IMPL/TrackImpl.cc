#include "IMPL/TrackImpl.h"
#include "IMPL/TrackStateImpl.h"

#include <cmath>
#include <sstream>

using namespace EVENT ;

namespace IMPL {
  
    TrackImpl::TrackImpl() :
        _type(0),
        _chi2(0),
        _ndf(0),
        _dEdx(0),
        _dEdxError(0),
        _radiusOfInnermostHit(0) { 
        }

  // copy constructor
  TrackImpl::TrackImpl(const TrackImpl& o)
  { 

      *this = o ; // call operator =
    
  }

    const TrackImpl & TrackImpl::operator = ( const TrackImpl &o )
    {
    _type = o._type ;
    _chi2 = o._chi2 ;
    _ndf = o._ndf ;
    _dEdx = o._dEdx ;
    _dEdxError = o._dEdxError ;
    _radiusOfInnermostHit = o._radiusOfInnermostHit ;

    std::copy( o._subdetectorHitNumbers.begin() ,  o._subdetectorHitNumbers.end() , std::back_inserter( _subdetectorHitNumbers ) ) ;

    std::copy( o._hits.begin() ,  o._hits.end() , std::back_inserter( _hits ) ) ;

    std::copy( o._tracks.begin() ,  o._tracks.end() , std::back_inserter( _tracks ) ) ;

    _trackStates.reserve(  o._trackStates.size() ) ;
    for( unsigned int i=0; i< o._trackStates.size() ; i++ ){
      _trackStates.push_back( new TrackStateImpl(  *o._trackStates[i] ) ) ;
    }

    return *this ;
    }


    TrackImpl::~TrackImpl() { 

        // delete track states in here ?
        for( unsigned int i=0; i<_trackStates.size() ; i++ ){

	  // std::cout << " *************** TrackImpl::~TrackImpl() @ " << this 
	  // 	    << " -  _trackStates.size() : " << _trackStates.size() 
	  // 	    << " - delete object at : " << _trackStates[i] 
	  // 	    << std::endl ;
	  
	  delete _trackStates[i] ;
        }
	
        //     for( IndexMap::const_iterator iter = _indexMap.begin() ; iter != _indexMap.end() ; iter++ ){
        //       delete iter->second ;
        //     }

    } 

    //  const std::string & TrackImpl::getType() const { return _type ; }

    int TrackImpl::getType() const { return _type.to_ulong() ; }

    //   bool TrackImpl::testType(int bitIndex) const {
    //     return _type.test( bitIndex ) ;
    //   }

    //float TrackImpl::getD0() const { return _d0 ;}
    //float TrackImpl::getPhi() const { return _phi ; }
    //float TrackImpl::getOmega() const { return _omega ; }
    //float TrackImpl::getZ0() const { return _z0 ;}
    //float TrackImpl::getTanLambda() const { return _tanLambda ;}
    //const FloatVec& TrackImpl::getCovMatrix() const { return _covMatrix ; }
    //const float* TrackImpl::getReferencePoint() const { return _reference ; }

    float TrackImpl::getD0() const {                    return ( _trackStates.size()>0 ? _trackStates[0]->getD0()             : 0 ) ;  }
    float TrackImpl::getPhi() const {                   return ( _trackStates.size()>0 ? _trackStates[0]->getPhi()            : 0 ) ;  }
    float TrackImpl::getOmega() const {                 return ( _trackStates.size()>0 ? _trackStates[0]->getOmega()          : 0 ) ;  }
    float TrackImpl::getZ0() const {                    return ( _trackStates.size()>0 ? _trackStates[0]->getZ0()             : 0 ) ;  }
    float TrackImpl::getTanLambda() const {             return ( _trackStates.size()>0 ? _trackStates[0]->getTanLambda()      : 0 ) ;  }
    const FloatVec& TrackImpl::getCovMatrix() const {   
      static const TrackStateImpl dummy ;
      return ( _trackStates.size()>0 ? _trackStates[0]->getCovMatrix()      : dummy.getCovMatrix() ) ;  
    }
    const float* TrackImpl::getReferencePoint() const { 
      static const TrackStateImpl dummy ;
      return ( _trackStates.size()>0 ? _trackStates[0]->getReferencePoint() : dummy.getReferencePoint() ) ;  
    }


    //bool  TrackImpl::isReferencePointPCA() const { 
    //    //    return _isReferencePointPCA ;
    //    return _type.test( BIT_ISREFERENCEPOINTDCA ) ;
    //}

    float TrackImpl::getChi2() const { return _chi2 ;}
    int   TrackImpl::getNdf() const { return _ndf ;}
    float TrackImpl::getdEdx() const { return _dEdx ; }
    float TrackImpl::getdEdxError() const { return _dEdxError ; }


    float TrackImpl::getRadiusOfInnermostHit() const {
        return _radiusOfInnermostHit ;
    }

    const IntVec & TrackImpl::getSubdetectorHitNumbers() const {
        return _subdetectorHitNumbers ;
    }
    const TrackerHitVec & TrackImpl::getTrackerHits() const {
        return _hits ;
    }

    const TrackVec & TrackImpl::getTracks() const {
        return _tracks ;
    } 

    const TrackStateVec & TrackImpl::getTrackStates() const {
        return _trackStates ;
    } 

    const TrackState* TrackImpl::getClosestTrackState( float x, float y, float z ) const {
        TrackState* closest = _trackStates[0] ;
        const float * refP = _trackStates[0]->getReferencePoint() ;
        float shortest_distance_square = pow( ( x - refP[0] ) , 2 ) + pow( ( y - refP[1] ) , 2 ) + pow( ( z - refP[2] ) , 2 ) ;
        float current_distance_square = 0 ;

        for( unsigned int i=1 ; i < _trackStates.size() ; i++ ){
            refP = _trackStates[i]->getReferencePoint() ;
            current_distance_square = pow( ( x - refP[0] ) , 2 ) + pow( ( y - refP[1] ) , 2 ) + pow( ( z - refP[2] ) , 2 ) ;
            if( current_distance_square < shortest_distance_square ){
                closest = _trackStates[i] ;
                shortest_distance_square = current_distance_square ;
            }
        }
        return closest ;
    } 

    const TrackState* TrackImpl::getTrackState( int location ) const {

        // if( location < 0 || location > TrackState::LastLocation ){
        //     throw( Exception( " trying to use getTrackState with an undefined Location" )) ;
        // }

        for( unsigned int i=0 ; i < _trackStates.size() ; i++ ){
            if( _trackStates[i]->getLocation() == location ){
                return _trackStates[i] ;  
            }
        }
        return NULL ;
    } 

    void  TrackImpl::setTypeBit( int  index, bool val){  
        checkAccess("TrackImpl::setTypeBit") ;
        _type.set( index, val  )  ;
    }

    //  void  TrackImpl::setType( const std::string&  type ){  
    void  TrackImpl::setType( int  type ){  
        checkAccess("TrackImpl::setType") ;

        //     int highWord = ( _type.to_ulong() & 0xFFFF0000 ) ;
        //     int lowWord = ( 0x0000FFFF & type ) ;
        //     _type = highWord | lowWord ; 

        _type = type ;
    } 

    void  TrackImpl::setD0( float d0 ){
        //checkAccess("TrackImpl::setD0") ;
        //_d0 = d0  ;

        if( _trackStates.size() == 0 ){
            // create a first TrackState for backwards compatibility
            TrackState* ts = new TrackStateImpl() ;
            _trackStates.push_back( ts ) ;
        }

        if( _trackStates.size() != 1 ){
            throw( Exception( " trying to use setD0 within Track object containing more than one TrackState." )) ;
        }

        ((TrackStateImpl*)_trackStates[0])->setD0( d0 ) ;
    } 
    void  TrackImpl::setPhi( float phi ){ 
        //checkAccess("TrackImpl::setPhi") ;
        //_phi = phi ; 

        if( _trackStates.size() == 0 ){
            // create a first TrackState for backwards compatibility
            TrackState* ts = new TrackStateImpl() ;
            _trackStates.push_back( ts ) ;
        }

        if( _trackStates.size() != 1 ){
            throw( Exception( " trying to use setPhi within Track object containing more than one TrackState." )) ;
        }

        ((TrackStateImpl*)_trackStates[0])->setPhi( phi ) ;
    } 
    void  TrackImpl::setOmega( float omega ) { 
        //checkAccess("TrackImpl::setOmega") ;
        //_omega = omega  ;

        if( _trackStates.size() == 0 ){
            // create a first TrackState for backwards compatibility
            TrackState* ts = new TrackStateImpl() ;
            _trackStates.push_back( ts ) ;
        }

        if( _trackStates.size() != 1 ){
            throw( Exception( " trying to use setOmega within Track object containing more than one TrackState." )) ;
        }

        ((TrackStateImpl*)_trackStates[0])->setOmega( omega ) ;
    } 
    void  TrackImpl::setZ0( float z0 ){
        //checkAccess("TrackImpl::setZ0") ;
        //_z0 = z0 ; 

        if( _trackStates.size() == 0 ){
            // create a first TrackState for backwards compatibility
            TrackState* ts = new TrackStateImpl() ;
            _trackStates.push_back( ts ) ;
        }

        if( _trackStates.size() != 1 ){
            throw( Exception( " trying to use setZ0 within Track object containing more than one TrackState." )) ;
        }

        ((TrackStateImpl*)_trackStates[0])->setZ0( z0 ) ;
    } 
    void  TrackImpl::setTanLambda( float tanLambda ){
        //checkAccess("TrackImpl::setTanLambda") ;
        //_tanLambda = tanLambda ; 

        if( _trackStates.size() == 0 ){
            // create a first TrackState for backwards compatibility
            TrackState* ts = new TrackStateImpl() ;
            _trackStates.push_back( ts ) ;
        }

        if( _trackStates.size() != 1 ){
            throw( Exception( " trying to use setTanLambda within Track object containing more than one TrackState." )) ;
        }

        ((TrackStateImpl*)_trackStates[0])->setTanLambda( tanLambda ) ;
    } 

    void  TrackImpl::setCovMatrix( const float* cov ){ 
        //checkAccess("TrackImpl::setCovMatrix") ;
        //for(int i=0;i<TRKNCOVMATRIX;i++) {
        //  _covMatrix[i] = cov[i]  ; 
        //}

        if( _trackStates.size() == 0 ){
            // create a first TrackState for backwards compatibility
            TrackState* ts = new TrackStateImpl() ;
            _trackStates.push_back( ts ) ;
        }

        if( _trackStates.size() != 1 ){
            throw( Exception( " trying to use setCovMatrix within Track object containing more than one TrackState." )) ;
        }

        ((TrackStateImpl*)_trackStates[0])->setCovMatrix( cov ) ;
    } 
    void  TrackImpl::setCovMatrix( const FloatVec& cov ){ 
        //checkAccess("TrackImpl::setCovMatrix") ;
        //for(int i=0;i<TRKNCOVMATRIX;i++) {
        //  _covMatrix[i] = cov[i]  ; 
        //}

        if( _trackStates.size() == 0 ){
            // create a first TrackState for backwards compatibility
            TrackState* ts = new TrackStateImpl() ;
            _trackStates.push_back( ts ) ;
        }

        if( _trackStates.size() != 1 ){
            throw( Exception( " trying to use setCovMatrix within Track object containing more than one TrackState." )) ;
        }

        ((TrackStateImpl*)_trackStates[0])->setCovMatrix( cov ) ;
    } 

    void  TrackImpl::setReferencePoint( const float* rPnt){ 
        //checkAccess("TrackImpl::setReferencePoint") ;
        //for(int i=0;i<3;i++) {
        //  _reference[i] = rPnt[i]  ; 
        //}

        if( _trackStates.size() == 0 ){
            // create a first TrackState for backwards compatibility
            TrackState* ts = new TrackStateImpl() ;
            _trackStates.push_back( ts ) ;
        }

        if( _trackStates.size() != 1 ){
            throw( Exception( " trying to use setReferencePoint within Track object containing more than one TrackState." )) ;
        }

        ((TrackStateImpl*)_trackStates[0])->setReferencePoint( rPnt ) ;
    } 

    //void  TrackImpl::setIsReferencePointPCA( bool val){ 
    //    checkAccess("TrackImpl::setIsReferencePointPCA") ;
    //    //    _isReferencePointPCA = val ;
    //    _type.set( BIT_ISREFERENCEPOINTDCA , val ) ;
    //} 

    void  TrackImpl::setChi2( float chi2 ){ 
        checkAccess("TrackImpl::setChi2") ;
        _chi2 = chi2 ; 
    } 
    void  TrackImpl::setNdf( int ndf ){ 
        checkAccess("TrackImpl::setNdf") ;
        _ndf = ndf  ; 
    } 
    void  TrackImpl::setdEdx( float dEdx ){ 
        checkAccess("TrackImpl::setdEdx") ;
        _dEdx = dEdx ; 
    } 
    void  TrackImpl::setdEdxError( float dEdxError ){
        checkAccess("TrackImpl::setdEdxError") ;
        _dEdxError = dEdxError  ;
    }   

    void TrackImpl::addHit( TrackerHit* hit) {
        _hits.push_back( hit ) ;
    }

    void  TrackImpl::addTrack( EVENT::Track* trk ) {
        checkAccess("TrackImpl::addTrack") ;
        _tracks.push_back( trk ) ;
    }

    void  TrackImpl::addTrackState( EVENT::TrackState* trkstate ) {
        checkAccess("TrackImpl::addTrackState") ;
        if( trkstate->getLocation() != TrackState::AtOther &&
            getTrackState( trkstate->getLocation() ) != NULL )
        {
            std::stringstream ss;
            ss << "another TrackState already exists with Location set to: " << trkstate->getLocation() ;
            throw( Exception( ss.str() )) ;
        }
        _trackStates.push_back( trkstate ) ;
    }

  TrackStateVec & TrackImpl::trackStates()  {
    checkAccess("TrackImpl::trackStates") ;
    return _trackStates ;
  } 

    IntVec & TrackImpl::subdetectorHitNumbers(){
        checkAccess("TrackImpl::subdetectorHitNumbers") ;
        return _subdetectorHitNumbers ;
    }

    void  TrackImpl::setRadiusOfInnermostHit( float r ){
        checkAccess("TrackImpl::setRadiusOfInnermostHit") ;
        _radiusOfInnermostHit = r ;
    }

} // namespace IMPL


