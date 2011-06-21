#include "IMPL/TrackStateImpl.h"


using namespace EVENT ;

namespace IMPL {

    TrackStateImpl::TrackStateImpl() :
        _location(0),
        _d0(0),
        _phi(0),
        _omega(0),
        _z0(0),
        _tanLambda(0)
    {

        for(int i=0 ; i < TRKSTATENCOVMATRIX ; i++ ) {
            _covMatrix.push_back( 0.0 ) ; 
        }

        for(int i=0 ; i < TRKSTATENREFSIZE ; i++ ) {
            _reference[i] = 0.0 ;
        }

    }

    TrackStateImpl::TrackStateImpl(int location, float d0, float phi, float omega, float z0, float tanLambda, float* covMatrix, float* reference) :
        //_location(location),
        _d0(d0),
        _phi(phi),
        _omega(omega),
        _z0(z0),
        _tanLambda(tanLambda)
    {

        setLocation( location );

        for(int i=0 ; i < TRKSTATENCOVMATRIX ; i++ ) {
            _covMatrix.push_back( covMatrix[i] ) ; 
        }

        setReferencePoint(reference);
    }


    TrackStateImpl::TrackStateImpl(int location, float d0, float phi, float omega, float z0, float tanLambda, FloatVec covMatrix, float* reference) :
        //_location(location),
        _d0(d0),
        _phi(phi),
        _omega(omega),
        _z0(z0),
        _tanLambda(tanLambda),
        _covMatrix(covMatrix)
    {

        setLocation( location );

        setReferencePoint(reference);
    }


    TrackStateImpl::~TrackStateImpl() { /* no-op */ } 

    int TrackStateImpl::getLocation() const { return _location ;}
    float TrackStateImpl::getD0() const { return _d0 ;}
    float TrackStateImpl::getPhi() const { return _phi ; }
    float TrackStateImpl::getOmega() const { return _omega ; }
    float TrackStateImpl::getZ0() const { return _z0 ;}
    float TrackStateImpl::getTanLambda() const { return _tanLambda ;}

    const FloatVec& TrackStateImpl::getCovMatrix() const { return _covMatrix ; }
    const float* TrackStateImpl::getReferencePoint() const { return _reference ; }


    void  TrackStateImpl::setLocation( int location ){
        checkAccess("TrackStateImpl::setLocation") ;

        if( location < 0 || location >= TrackState::Location::size() ){
            throw( Exception( " TrackStateImpl::setLocation called with an undefined Location" )) ;
        }

        _location = location  ;
    } 
    void  TrackStateImpl::setD0( float d0 ){
        checkAccess("TrackStateImpl::setD0") ;
        _d0 = d0  ;
    } 
    void  TrackStateImpl::setPhi( float phi ){ 
        checkAccess("TrackStateImpl::setPhi") ;
        _phi = phi ; 
    } 
    void  TrackStateImpl::setOmega( float omega ) { 
        checkAccess("TrackStateImpl::setOmega") ;
        _omega = omega  ;
    } 
    void  TrackStateImpl::setZ0( float z0 ){
        checkAccess("TrackStateImpl::setZ0") ;
        _z0 = z0 ; 
    } 
    void  TrackStateImpl::setTanLambda( float tanLambda ){
        checkAccess("TrackStateImpl::setTanLambda") ;
        _tanLambda = tanLambda ; 
    } 
    void  TrackStateImpl::setCovMatrix( const float* cov ){ 
        checkAccess("TrackStateImpl::setCovMatrix") ;
        for( int i=0 ; i<TRKSTATENCOVMATRIX ; i++ ){
            _covMatrix[i] = cov[i]  ; 
        }
    } 
    void  TrackStateImpl::setCovMatrix( const FloatVec& cov ){ 
        checkAccess("TrackStateImpl::setCovMatrix") ;
        for( int i=0 ; i<TRKSTATENCOVMATRIX ; i++ ){
            _covMatrix[i] = cov[i]  ; 
        }
    } 

    void  TrackStateImpl::setReferencePoint( const float* rPnt ){ 
        checkAccess("TrackStateImpl::setReferencePoint") ;
        for( int i=0 ; i<TRKSTATENREFSIZE ; i++ ){
            _reference[i] = rPnt[i]  ; 
        }
    } 

} // namespace IMPL


