#include "SIO/SIOTrackerHitZCylinderHandler.h"

#include "SIO/LCSIO.h"

#include "EVENT/LCIO.h"
#include "EVENT/TrackerHitZCylinder.h"
#include "IOIMPL/TrackerHitZCylinderIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"

#include "IMPL/LCFlagImpl.h"

using namespace EVENT ;
using namespace IMPL ;
using namespace IOIMPL ;


namespace SIO{

    unsigned int SIOTrackerHitZCylinderHandler::read(SIO_stream* stream, LCObject** objP){
        unsigned int status ;

        // create a new object :
        TrackerHitZCylinderIOImpl* hit  = new TrackerHitZCylinderIOImpl ;
        *objP = hit ;

        LCFlagImpl lcFlag(_flag) ;

        if( _vers > SIO_VERSION_ENCODE( 1, 51) ) {
            SIO_DATA( stream ,  &(hit->_cellID0) , 1  ) ;
            if( lcFlag.bitSet( LCIO::RTHZBIT_ID1 ) ){
                SIO_DATA( stream ,  &(hit->_cellID1) , 1  ) ;
            }
        }

        SIO_DATA( stream, &(hit->_type) , 1 ) ;
        SIO_DATA( stream, hit->_pos     , 3 ) ;
        SIO_DATA( stream, hit->_center  , 2 ) ;
        //SIO_DATA( stream, &(hit->_r)    , 1 ) ;
        SIO_DATA( stream, &(hit->_drphi), 1 ) ;
        SIO_DATA( stream, &(hit->_dz)   , 1 ) ;
        SIO_DATA( stream, &(hit->_EDep) , 1 ) ;
        SIO_DATA( stream, &(hit->_EDepError) , 1 ) ;
        SIO_DATA( stream, &(hit->_time) , 1 ) ;
        SIO_DATA( stream, &(hit->_quality) , 1 ) ;


        //float cov[TRKHITZCYLNCOVMATRIX] ;
        //SIO_DATA( stream ,  cov  ,  TRKHITZCYLNCOVMATRIX ) ;
        //hit->setCovMatrix( cov ) ;

        // rawHits
        int numberOfRawHits = 1 ; 
        SIO_DATA( stream ,  &numberOfRawHits , 1  ) ;

        hit->_rawHits.resize( numberOfRawHits ) ;

        for(int i=0;i<numberOfRawHits;i++){
            SIO_PNTR( stream , &(hit->_rawHits[i] ) ) ;
        }

        SIO_PTAG( stream , dynamic_cast<const TrackerHitZCylinder*>(hit) ) ;

        return ( SIO_BLOCK_SUCCESS ) ;

    }


    unsigned int SIOTrackerHitZCylinderHandler::write(SIO_stream* stream, 
            const LCObject* obj){

        unsigned int status ; 

        const TrackerHitZCylinder* hit = dynamic_cast<const TrackerHitZCylinder*>(obj)  ;

        LCFlagImpl lcFlag(_flag) ;

        LCSIO_WRITE( stream, hit->getCellID0()  ) ;

        if( lcFlag.bitSet( LCIO::RTHZBIT_ID1 ) ){
            LCSIO_WRITE( stream, hit->getCellID1()  ) ;
        }

        LCSIO_WRITE( stream ,  hit->getType() ) ;

        // as SIO doesn't provide a write function with const arguments
        // we have to cast away the constness 
        double* pos = const_cast<double*> ( hit->getPosition() ) ; 
        SIO_DATA( stream,  pos , 3 ) ;

        float* center = const_cast<float*> ( hit->getCenter() ) ; 
        SIO_DATA( stream,  center , 2 ) ;

        //LCSIO_WRITE( stream, hit->getR()  ) ;
        LCSIO_WRITE( stream, hit->getdRPhi()  ) ;
        LCSIO_WRITE( stream, hit->getdZ()  ) ;
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
