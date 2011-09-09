#include "SIO/SIOTrackerHitPlaneHandler.h"

#include "EVENT/LCIO.h"
#include "SIO/LCSIO.h"

#include "EVENT/TrackerHitPlane.h"
#include "IOIMPL/TrackerHitPlaneIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"

#include "IMPL/LCFlagImpl.h"

using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{

    unsigned int SIOTrackerHitPlaneHandler::read(SIO_stream* stream, LCObject** objP){
        unsigned int status ;

        // create a new object :
        TrackerHitPlaneIOImpl* hit  = new TrackerHitPlaneIOImpl ;
        *objP = hit ;

        LCFlagImpl lcFlag(_flag) ;

        if( _vers > SIO_VERSION_ENCODE( 1, 51) ) {
            SIO_DATA( stream ,  &(hit->_cellID0) , 1  ) ;
            if( lcFlag.bitSet( LCIO::RTHPBIT_ID1 ) ){
                SIO_DATA( stream ,  &(hit->_cellID1) , 1  ) ;
            }
        }

        SIO_DATA( stream, &(hit->_type) , 1 ) ;
        SIO_DATA( stream, hit->_pos     , 3 ) ;
        SIO_DATA( stream, hit->_u       , 2 ) ;
        SIO_DATA( stream, hit->_v       , 2 ) ;
        SIO_DATA( stream, &(hit->_du)   , 1 ) ;
        SIO_DATA( stream, &(hit->_dv)   , 1 ) ;
        SIO_DATA( stream, &(hit->_EDep) , 1 ) ;
        SIO_DATA( stream, &(hit->_EDepError) , 1 ) ;
        SIO_DATA( stream, &(hit->_time) , 1 ) ;
        SIO_DATA( stream, &(hit->_quality) , 1 ) ;


        //float cov[TRKHITPLANENCOVMATRIX] ;
        //SIO_DATA( stream ,  cov  ,  TRKHITPLANENCOVMATRIX ) ;
        //hit->setCovMatrix( cov ) ;

        // rawHits
        int numberOfRawHits = 1 ; 
        SIO_DATA( stream ,  &numberOfRawHits , 1  ) ;

        hit->_rawHits.resize( numberOfRawHits ) ;

        for(int i=0;i<numberOfRawHits;i++){
            SIO_PNTR( stream , &(hit->_rawHits[i] ) ) ;
        }

        SIO_PTAG( stream , dynamic_cast<const TrackerHitPlane*>(hit) ) ;

        return ( SIO_BLOCK_SUCCESS ) ;

    }


    unsigned int SIOTrackerHitPlaneHandler::write(SIO_stream* stream, 
            const LCObject* obj){

        unsigned int status ; 

        const TrackerHitPlane* hit = dynamic_cast<const TrackerHitPlane*>(obj)  ;


        LCFlagImpl lcFlag(_flag) ;

        LCSIO_WRITE( stream, hit->getCellID0()  ) ;

        if( lcFlag.bitSet( LCIO::RTHPBIT_ID1 ) ){
            LCSIO_WRITE( stream, hit->getCellID1()  ) ;
        }


        LCSIO_WRITE( stream ,  hit->getType() ) ;

        // as SIO doesn't provide a write function with const arguments
        // we have to cast away the constness 
        double* pos = const_cast<double*> ( hit->getPosition() ) ; 
        SIO_DATA( stream,  pos , 3 ) ;

        float* u = const_cast<float*> ( hit->getU() ) ; 
        SIO_DATA( stream,  u , 2 ) ;

        float* v = const_cast<float*> ( hit->getV() ) ; 
        SIO_DATA( stream,  v , 2 ) ;


        LCSIO_WRITE( stream, hit->getdU()  ) ;
        LCSIO_WRITE( stream, hit->getdV()  ) ;
        LCSIO_WRITE( stream, hit->getEDep()  ) ;
        LCSIO_WRITE( stream, hit->getEDepError()  ) ;
        LCSIO_WRITE( stream, hit->getTime()  ) ;
        LCSIO_WRITE( stream, hit->getQuality()  ) ;

        
        //const FloatVec& cov = hit->getCovMatrix() ;
        //for(unsigned int i=0;i<cov.size();i++){
        //    LCSIO_WRITE( stream, cov[i]  ) ;
        //}

        const EVENT::LCObjectVec& rawHits = hit->getRawHits() ;

        LCSIO_WRITE( stream, rawHits.size()  ) ;
        for(unsigned int i=0; i < rawHits.size() ; i++){
            SIO_PNTR( stream , &(rawHits[i]) ) ;
        }

        SIO_PTAG( stream , hit ) ;

        return ( SIO_BLOCK_SUCCESS ) ;

    }

} // namespace
