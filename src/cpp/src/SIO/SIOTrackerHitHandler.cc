#include "SIO/SIOTrackerHitHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/MCParticle.h"
#include "EVENT/TrackerHit.h"
#include "IOIMPL/TrackerHitIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"
#include "SIO_stream.h"

#include "EVENT/LCIO.h"
#include "IMPL/LCFlagImpl.h"


using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{

    unsigned int SIOTrackerHitHandler::read(SIO_stream* stream, 
            LCObject** objP){
        unsigned int status ; 

        // create a new object :
        TrackerHitIOImpl* hit  = new TrackerHitIOImpl ;
        *objP = hit ;

        LCFlagImpl lcFlag(_flag) ;

        if( _vers > SIO_VERSION_ENCODE( 1, 51) ) {
            SIO_DATA( stream ,  &(hit->_cellID0) , 1  ) ;
            if( lcFlag.bitSet( LCIO::RTHBIT_ID1 ) ){
                SIO_DATA( stream ,  &(hit->_cellID1) , 1  ) ;
            }
        }

        if( _vers > SIO_VERSION_ENCODE( 1, 2)   ){
            SIO_DATA( stream ,  &(hit->_type) , 1  ) ;
        } 

        SIO_DATA( stream ,    hit->_pos  , 3 ) ;

        float cov[TRKHITNCOVMATRIX] ;
        SIO_DATA( stream ,  cov  ,  TRKHITNCOVMATRIX ) ;
        hit->setCovMatrix( cov ) ;

        //SIO_DATA( stream ,  &(hit->_dEdx) , 1  ) ;
        SIO_DATA( stream ,  &(hit->_EDep) , 1  ) ;

        if( _vers > SIO_VERSION_ENCODE( 1, 12 )   ){
            SIO_DATA( stream ,  &(hit->_EDepError) , 1  ) ;
        }

        SIO_DATA( stream ,  &(hit->_time) , 1  ) ;

        if( _vers > SIO_VERSION_ENCODE( 1, 11 )   )
            SIO_DATA( stream ,  &(hit->_quality) , 1  ) ;


        // rawHits
        int numberOfRawHits = 1 ; 
        if( _vers > SIO_VERSION_ENCODE( 1, 2)   ){
            SIO_DATA( stream ,  &numberOfRawHits , 1  ) ;
        }

        hit->_rawHits.resize( numberOfRawHits ) ;

        for(int i=0;i<numberOfRawHits;i++){
            SIO_PNTR( stream , &(hit->_rawHits[i] ) ) ;
        }

        SIO_PTAG( stream , dynamic_cast<const TrackerHit*>(hit) ) ;

        return ( SIO_BLOCK_SUCCESS ) ;

    }


    unsigned int SIOTrackerHitHandler::write(SIO_stream* stream, 
            const LCObject* obj){

        unsigned int status ; 

        const TrackerHit* hit = dynamic_cast<const TrackerHit*>(obj)  ;

        LCFlagImpl lcFlag(_flag) ;

        LCSIO_WRITE( stream, hit->getCellID0()  ) ;

        if( lcFlag.bitSet( LCIO::RTHBIT_ID1 ) ){
            LCSIO_WRITE( stream, hit->getCellID1()  ) ;
        }

        LCSIO_WRITE( stream ,  hit->getType() ) ;

        // as SIO doesn't provide a write function with const arguments
        // we have to cast away the constness 
        double* pos = const_cast<double*> ( hit->getPosition() ) ; 
        SIO_DATA( stream,  pos , 3 ) ;

        const FloatVec& cov = hit->getCovMatrix() ;
        for(unsigned int i=0;i<cov.size();i++){
            LCSIO_WRITE( stream, cov[i]  ) ;
        }

        //LCSIO_WRITE( stream, hit->getdEdx()  ) ;
        LCSIO_WRITE( stream, hit->getEDep()  ) ;
        LCSIO_WRITE( stream, hit->getEDepError()  ) ;
        LCSIO_WRITE( stream, hit->getTime()  ) ;
        LCSIO_WRITE( stream, hit->getQuality()  ) ;

        //     const LCObject* raw = hit->getRawDataHit()  ;
        //     SIO_PNTR( stream , &raw ) ;

        const EVENT::LCObjectVec& rawHits = hit->getRawHits() ;


        LCSIO_WRITE( stream, rawHits.size()  ) ;
        for(unsigned int i=0; i < rawHits.size() ; i++){
            SIO_PNTR( stream , &(rawHits[i]) ) ;
        }

        // write a ptag in order to be able to point to tracker hits in the future
        SIO_PTAG( stream , hit ) ;

        return ( SIO_BLOCK_SUCCESS ) ;

    }

} // namespace
