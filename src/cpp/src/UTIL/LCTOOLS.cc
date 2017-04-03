#include "UTIL/LCTOOLS.h"
#include "UTIL/Operators.h"
#include "UTIL/LCObjectHandle.h"
#include "UTIL/LCTime.h"
#include "UTIL/CellIDDecoder.h"
#include "UTIL/PIDHandler.h"

#include "EVENT/LCCollection.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/CalorimeterHit.h"
#include "EVENT/RawCalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"
#include "EVENT/TrackerHit.h"
#include "EVENT/TrackerHitPlane.h"
#include "EVENT/TrackerHitZCylinder.h"
#include "EVENT/TPCHit.h"
#include "EVENT/TrackerRawData.h"
#include "EVENT/TrackerData.h"
#include "EVENT/TrackerPulse.h"
#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCFloatVec.h"
#include "EVENT/LCIntVec.h"
#include "EVENT/Track.h"
#include "EVENT/Cluster.h"
#include "EVENT/ReconstructedParticle.h"
#include "EVENT/Vertex.h"
#include "EVENT/LCGenericObject.h"
#include "EVENT/LCRelation.h"

//#include "IMPL/LCFlagImpl.h"
#include "UTIL/BitSet32.h"
#include "LCIOSTLTypes.h"

#include <map>
#include <set>
#include <cstdio>


typedef std::vector<std::string> LCStrVec ;

using namespace std ;
using namespace EVENT ;
using namespace IMPL ;


namespace UTIL {

    static int MAX_HITS = 1000 ;

    void LCTOOLS::dumpEvent(const LCEvent* evt){

        // the event:
        cout << "///////////////////////////////////" << endl;
        cout << "EVENT: " << evt->getEventNumber() << endl
            << "RUN: " << evt->getRunNumber() << endl
            << "DETECTOR: " << evt->getDetectorName() << endl
            << "COLLECTIONS: (see below)" << endl;
        cout << "///////////////////////////////////" << endl << endl;

        cout << "---------------------------------------------------------------------------" << endl;
        cout.width(30); cout << left << "COLLECTION NAME";
        cout.width(25); cout << left << "COLLECTION TYPE";
        cout.width(20); cout << left << "NUMBER OF ELEMENTS" << endl;
        cout << "===========================================================================" << endl;

        const std::vector< std::string >* strVec = evt->getCollectionNames() ;

        // loop over collections:
        for( std::vector< std::string >::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){

            cout.width(30); cout << left << *name;
            cout.width(25); cout << left << evt->getCollection( *name )->getTypeName();
            cout.width(9); cout << right << evt->getCollection( *name )->getNumberOfElements();
            cout << endl;
            //cout << "---------------------------------------------------------------------------" << endl;
        }
        cout << "---------------------------------------------------------------------------" << endl;
        cout << endl << endl << endl;
    }


    void LCTOOLS::dumpEventDetailed(const LCEvent* evt){

        // the event:
        cout << endl 
            << "============================================================================" << endl ;
        cout << "        Event  : " << evt->getEventNumber() 
            << " - run:  "         << evt->getRunNumber()
            << " - timestamp "     << evt->getTimeStamp()   
            << " - weight "        << evt->getWeight()   
            << endl ;
        cout << "============================================================================" << endl ;    

        LCTime evtTime( evt->getTimeStamp() ) ;
        cout << " date:      "      << evtTime.getDateString() << endl ;     
        cout << " detector : "      << evt->getDetectorName() << endl ;

        cout << " event parameters: " << endl ; 

        printParameters( evt->getParameters() ) ;


        const std::vector< std::string >* strVec = evt->getCollectionNames() ;

        // loop over all collections:
        std::vector< std::string >::const_iterator name ;

        for( name = strVec->begin() ; name != strVec->end() ; name++){

            LCCollection* col = evt->getCollection( *name ) ;

            cout << endl 
                << " collection name : " << *name 
                << endl 
                << " parameters: " << endl ;

            //      printParameters( col->getParameters() ) ;



            // call the detailed print functions depending on type name
            if( evt->getCollection( *name )->getTypeName() == LCIO::MCPARTICLE ){

                printMCParticles( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::SIMTRACKERHIT ){

                printSimTrackerHits( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::TPCHIT ){

                printTPCHits( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::TRACKERHIT ){

                printTrackerHits( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::TRACKERHITPLANE ){

                printTrackerHitPlane( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::TRACKERHITZCYLINDER ){

                printTrackerHitZCylinder( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::SIMCALORIMETERHIT ){

                printSimCalorimeterHits( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::CALORIMETERHIT ){

                printCalorimeterHits( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::RAWCALORIMETERHIT ){

                printRawCalorimeterHits( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::LCFLOATVEC ){

                printLCFloatVecs( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::LCINTVEC ){

                printLCIntVecs( col ) ;                               

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::LCSTRVEC ){

                printLCStrVecs( col ) ;                                 

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::TRACK ){

                printTracks( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::CLUSTER ){

                printClusters( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::RECONSTRUCTEDPARTICLE ){

                printReconstructedParticles( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::VERTEX ){

                printVertices( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::LCGENERICOBJECT ){

                printLCGenericObjects( col ) ;

            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::LCRELATION ){

                printRelation( col ) ;
            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::TRACKERRAWDATA ){

                printTrackerRawData( col ) ;
            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::TRACKERDATA ){

                printTrackerData( col ) ;
            }
            else if( evt->getCollection( *name )->getTypeName() == LCIO::TRACKERPULSE ){

                printTrackerPulse( col ) ;
            }

        }

    }

    void LCTOOLS::printTracks(const EVENT::LCCollection* col ){
        if( col->getTypeName() != LCIO::TRACK ){

            cout << " collection not of type " << LCIO::TRACK << endl ;
            return ;
        }
        cout << endl 
            << "--------------- " << "print out of "  << LCIO::TRACK << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        BitSet32 flag( col->getFlag() ) ;
        cout << "     LCIO::TRBIT_HITS : " << flag.test( LCIO::TRBIT_HITS ) << endl ;

        int nTracks =  col->getNumberOfElements() ;
        int nPrint = nTracks > MAX_HITS ? MAX_HITS : nTracks ;

        cout << endl;
        Track* trk=NULL;
        cout << header(trk);
        cout << tail(trk);

        for( int i=0 ; i< nPrint ; i++ ){
            trk = dynamic_cast<Track*>( col->getElementAt( i ));
            cout << lcio_short<EVENT::Track>(trk, col);
            cout << tail(trk);
        }
    }

    void LCTOOLS::printSimTrackerHits(const EVENT::LCCollection* col ){
        if( col->getTypeName() != LCIO::SIMTRACKERHIT ){

            cout << " collection not of type " << LCIO::SIMTRACKERHIT << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::SIMTRACKERHIT << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        BitSet32 flag( col->getFlag() ) ;
        cout << "     LCIO::THBIT_BARREL : " << flag.test( LCIO::THBIT_BARREL ) << endl ;
        cout << "     LCIO::THBIT_MOMENTUM : " << flag.test( LCIO::THBIT_MOMENTUM ) << endl ;
        cout << LCTOOLS::getQualityBits() << endl;

        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

        cout << endl;
        SimTrackerHit* hit=NULL;
        cout << header(hit);
        cout << tail(hit);
        for( int i=0 ; i< nPrint ; i++ ){
            hit = dynamic_cast<SimTrackerHit*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::SimTrackerHit>(hit,col);
        }
        cout << tail(hit) << endl;
    }

    void LCTOOLS::printTrackerHits(const EVENT::LCCollection* col ){

        if( col->getTypeName() != LCIO::TRACKERHIT ){

            cout << " collection not of type " << LCIO::TRACKERHIT << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::TRACKERHIT << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        BitSet32 flag( col->getFlag() ) ;
        cout << "     LCIO::THBIT_BARREL : " << flag.test( LCIO::THBIT_BARREL ) << endl ;


        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

        cout << endl;
        TrackerHit* hit=NULL;
        cout << header(hit);
        cout << tail(hit);
        for( int i=0 ; i< nPrint ; i++ ){
            hit = dynamic_cast<TrackerHit*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::TrackerHit>(hit,col);
            // const LCObjectVec& rawHits = hit->getRawHits() ;
            // cout << "    rawHits: " ;
            // try{
            // 	    for( unsigned j=0 ; j < rawHits.size() ; j++ ) {
            // 	      cout << dec << "[" << rawHits[j]->id() << "], " <<  dec ;
            // 	    }
            //   cout << endl;
            // }catch(std::exception& e){}
        }
        cout << tail(hit) << endl;
    }

    void LCTOOLS::printTrackerHitPlane(const EVENT::LCCollection* col ){

        if( col->getTypeName() != LCIO::TRACKERHITPLANE ){

            cout << " collection not of type " << LCIO::TRACKERHITPLANE << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::TRACKERHITPLANE << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        BitSet32 flag( col->getFlag() ) ;
        cout << "     LCIO::THBIT_BARREL : " << flag.test( LCIO::THBIT_BARREL ) << endl ;


        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

        cout << endl;
        TrackerHitPlane* hit=NULL;
        cout << header(hit);
        cout << tail(hit);
        for( int i=0 ; i< nPrint ; i++ ){
            hit = dynamic_cast<TrackerHitPlane*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::TrackerHitPlane>(hit,col);
            // const LCObjectVec& rawHits = hit->getRawHits() ;
            // cout << "    rawHits: " ;
            // try{
            // 	    for( unsigned j=0 ; j < rawHits.size() ; j++ ) {
            // 	      cout << dec << "[" << rawHits[j]->id() << "], " <<  dec ;
            // 	    }
            //   cout << endl;
            // }catch(std::exception& e){}
        }
        cout << tail(hit) << endl;
    }

    void LCTOOLS::printTrackerHitZCylinder(const EVENT::LCCollection* col ){

        if( col->getTypeName() != LCIO::TRACKERHITZCYLINDER ){

            cout << " collection not of type " << LCIO::TRACKERHITZCYLINDER << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::TRACKERHITZCYLINDER << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        BitSet32 flag( col->getFlag() ) ;
        cout << "     LCIO::THBIT_BARREL : " << flag.test( LCIO::THBIT_BARREL ) << endl ;


        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

        cout << endl;
        TrackerHitZCylinder* hit=NULL;
        cout << header(hit);
        cout << tail(hit);
        for( int i=0 ; i< nPrint ; i++ ){
            hit = dynamic_cast<TrackerHitZCylinder*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::TrackerHitZCylinder>(hit,col);
            // const LCObjectVec& rawHits = hit->getRawHits() ;
            // cout << "    rawHits: " ;
            // try{
            // 	    for( unsigned j=0 ; j < rawHits.size() ; j++ ) {
            // 	      cout << dec << "[" << rawHits[j]->id() << "], " <<  dec ;
            // 	    }
            //   cout << endl;
            // }catch(std::exception& e){}
        }
        cout << tail(hit) << endl;
    }

    void LCTOOLS::printTrackerRawData(const EVENT::LCCollection* col ) {

        if( col->getTypeName() != LCIO::TRACKERRAWDATA ){

            cout << " collection not of type " << LCIO::TRACKERRAWDATA << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::TRACKERRAWDATA << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        BitSet32 flag( col->getFlag() ) ;
        cout << "     LCIO::TRAWBIT_ID1    : " << flag.test( LCIO::TRAWBIT_ID1 ) << endl ;

        printParameters( col->getParameters() ) ;

        CellIDDecoder<TrackerRawData> id( col ) ;

        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

        cout << endl;
        TrackerRawData* hit=NULL;
        cout << header(hit);
        cout << tail(hit);
        for( int i=0 ; i< nPrint ; i++ ){
            hit = dynamic_cast<TrackerRawData*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::TrackerRawData>(hit,col);
        }
        cout << tail(hit);
    }

    void LCTOOLS::printTrackerData(const EVENT::LCCollection* col ) {

        if( col->getTypeName() != LCIO::TRACKERDATA ){

            cout << " collection not of type " << LCIO::TRACKERDATA << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::TRACKERDATA << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        BitSet32 flag( col->getFlag() ) ;
        cout << "     LCIO::TRAWBIT_ID1    : " << flag.test( LCIO::TRAWBIT_ID1 ) << endl ;

        printParameters( col->getParameters() ) ;

        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

        CellIDDecoder<TrackerData> id( col ) ;

        cout << endl;
        TrackerData* hit=NULL;
        cout << header(hit);
        cout << tail(hit);
        for( int i=0 ; i< nPrint ; i++ ){
            hit = dynamic_cast<TrackerData*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::TrackerData>(hit,col);
        }
        cout << tail(hit);
    }

    void LCTOOLS::printTrackerPulse(const EVENT::LCCollection* col ) {

        if( col->getTypeName() != LCIO::TRACKERPULSE ){

            cout << " collection not of type " << LCIO::TRACKERPULSE << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::TRACKERPULSE << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        BitSet32 flag( col->getFlag() ) ;
        cout << "     LCIO::TRAWBIT_ID1    : " << flag.test( LCIO::TRAWBIT_ID1 )
            << "     LCIO::TRAWBIT_CM     : " << flag.test( LCIO::TRAWBIT_CM ) << endl ;

        printParameters( col->getParameters() ) ;

        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

        CellIDDecoder<TrackerPulse> id( col ) ;

        cout << endl;
        TrackerPulse* pul=NULL;
        cout << header(pul);
        cout << tail(pul);
        for( int i=0 ; i< nPrint ; i++ ){
            pul = dynamic_cast<TrackerPulse*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::TrackerPulse>(pul,col);
        }
        cout << tail(pul);
    }

    void LCTOOLS::printTPCHits(const EVENT::LCCollection* col ) { //tpc hit is deprecated

        if( col->getTypeName() != LCIO::TPCHIT ){

            cout << " collection not of type " << LCIO::TPCHIT << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::TPCHIT << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        BitSet32 flag( col->getFlag() ) ;
        cout << "  -> LCIO::TPCBIT_RAW   : " << flag.test( LCIO::TPCBIT_RAW ) << endl ;
        cout << "  -> LCIO::TPCBIT_NO_PTR   : " << flag.test( LCIO::TPCBIT_NO_PTR ) << endl ;

        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

        std::cout << endl
            << " [   id   ]  | cellId0 | time | charge | quality " 
            << endl << "  -> raw data (bytes) : "
                << endl 
                   << endl ;

        for( int i=0 ; i< nPrint ; i++ ){

            TPCHit* hit = 
                dynamic_cast<TPCHit*>( col->getElementAt( i ) ) ;

            int id0 = hit->getCellID()  ;
            //      std::cout << hit->getCellID() << " | " 
            printf( " [%8.8x] |", hit->id() ) ;
            std::cout	<< ((id0& 0xff000000)>>24) << "/" 
                << ((id0& 0x00ff0000)>>16) << "/" 
                << ((id0& 0x0000ff00)>> 8) << "/" 
                << ((id0& 0x000000ff)>> 0) << " | "
                << hit->getTime() << " | "
                << hit->getCharge() << " | ["
                << hit->getQuality() << "] "
                << std::endl ;
            if( flag.test( LCIO::TPCBIT_RAW ) ){

                int nWords = hit->getNRawDataWords() ;
                std::cout << "  ->  " ;

                for(int  j=0;j<nWords;j++){
                    int rawData = hit->getRawDataWord( j ) ;
                    std::cout << ((rawData& 0xff000000)>>24) << ", " 
                        << ((rawData& 0x00ff0000)>>16) << ", " 
                        << ((rawData& 0x0000ff00)>> 8) << ", " 
                        << ((rawData& 0x000000ff)>> 0)  ;
                    if( j < nWords-1) std::cout << ", " ;
                }

                std::cout << std::endl ;

            }
        }
        cout << endl 
            << "-------------------------------------------------------------------------------- " 
            << endl ;
    }

    void LCTOOLS::printLCStrVecs( const EVENT::LCCollection* col ) {

        if( col->getTypeName() != LCIO::LCSTRVEC ){

            cout << " collection not of type " << LCIO::LCSTRVEC << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::LCSTRVEC << " collection (user extension) "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;



        std::cout << endl
            << " element index: val0, val1, ..." 
            << endl 
            << endl ;

        for( int i=0 ; i< nPrint ; i++ ){

            LCStrVec* vec =
                dynamic_cast<LCStrVec*>( col->getElementAt( i ) ) ;

            std::cout << i << ": " ;
            for(unsigned int j=0;j< vec->size();j++){

                std::cout << (*vec)[j] ;
                if( j<vec->size()-1) std::cout << ", " ;

                if( ! ( (j+1) % 10)  ) std::cout << endl << "     " ;
            }
            std::cout << std::endl ;
        }
        cout << endl 
            << "-------------------------------------------------------------------------------- " 
            << endl ;

    }

    void LCTOOLS::printLCFloatVecs( const EVENT::LCCollection* col ) {

        if( col->getTypeName() != LCIO::LCFLOATVEC ){

            cout << " collection not of type " << LCIO::LCFLOATVEC << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::LCFLOATVEC << " collection (user extension) "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

        cout << endl;
        LCFloatVec* vec=NULL;
        cout << header(vec);
        cout << tail(vec);
        for( int i=0 ; i< nPrint ; i++ ){
            vec = dynamic_cast<LCFloatVec*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::LCFloatVec>(vec,col);
        }
        cout << tail(vec);
    }

    void LCTOOLS::printLCIntVecs( const EVENT::LCCollection* col ) {

        if( col->getTypeName() != LCIO::LCINTVEC ){

            cout << " collection not of type " << LCIO::LCINTVEC << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::LCINTVEC << " collection (user extension) "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;
        cout << endl;
        LCIntVec* vec=NULL;
        cout << header(vec);
        cout << tail(vec);
        for( int i=0 ; i< nPrint ; i++ ){
            vec = dynamic_cast<LCIntVec*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::LCIntVec>(vec,col);
        }
        cout << tail(vec);
    }

    void LCTOOLS::printSimCalorimeterHits(const EVENT::LCCollection* col ){

        if( col->getTypeName() != LCIO::SIMCALORIMETERHIT ){

            cout << " collection not of type " << LCIO::SIMCALORIMETERHIT << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::SIMCALORIMETERHIT << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        BitSet32 flag( col->getFlag() ) ;
        cout << "  -> LCIO::CHBIT_LONG   : " << flag.test( LCIO::CHBIT_LONG ) << endl ;
        cout << "     LCIO::CHBIT_BARREL : " << flag.test( LCIO::CHBIT_BARREL ) << endl ;
        cout << "     LCIO::CHBIT_ID1    : " << flag.test( LCIO::CHBIT_ID1 ) << endl ;
        cout << "     LCIO::CHBIT_STEP   : " << flag.test( LCIO::CHBIT_STEP ) << endl ;

        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;


        CellIDDecoder<SimCalorimeterHit> idDecoder( col ) ; 

        cout << endl;
        SimCalorimeterHit* hit=NULL;
        cout << header(hit);
        cout << tail(hit);
        for( int i=0 ; i< nPrint ; i++ ){
            hit = dynamic_cast<SimCalorimeterHit*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::SimCalorimeterHit>(hit,col);
        }
        cout << tail(hit);
    }

    void LCTOOLS::printCalorimeterHits(const EVENT::LCCollection* col ){

        if( col->getTypeName() != LCIO::CALORIMETERHIT ){

            cout << " collection not of type " << LCIO::CALORIMETERHIT << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::CALORIMETERHIT << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        BitSet32 flag( col->getFlag() ) ;
        cout << "  -> LCIO::RCHBIT_LONG   : " << flag.test( LCIO::RCHBIT_LONG ) << endl ;
        cout << "     LCIO::RCHBIT_BARREL : " << flag.test( LCIO::RCHBIT_BARREL ) << endl ;
        cout << "     LCIO::RCHBIT_ID1    : " << flag.test( LCIO::RCHBIT_ID1 ) << endl ;
        cout << "     LCIO::RCHBIT_TIME   : " << flag.test( LCIO::RCHBIT_TIME ) << endl ;
        cout << "     LCIO::RCHBIT_NO_PTR : " << flag.test( LCIO::RCHBIT_NO_PTR ) << endl ;
        cout << "     LCIO::RCHBIT_ENERGY_ERROR  : " << flag.test( LCIO::RCHBIT_ENERGY_ERROR ) << endl ;

        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;

        CellIDDecoder<CalorimeterHit> idDecoder( col ) ; 

        cout << endl;
        CalorimeterHit* hit=NULL;
        cout << header(hit);
        cout << tail(hit);
        for( int i=0 ; i< nPrint ; i++ ){
            hit = dynamic_cast<CalorimeterHit*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::CalorimeterHit>(hit,col);
        }
        cout << tail(hit);
    }

    void LCTOOLS::printRawCalorimeterHits(const EVENT::LCCollection* col ){

        if( col->getTypeName() != LCIO::RAWCALORIMETERHIT ){

            cout << " collection not of type " << LCIO::RAWCALORIMETERHIT << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::RAWCALORIMETERHIT << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        BitSet32 flag( col->getFlag() ) ;

        cout << "     LCIO::RCHBIT_ID1    : " << flag.test( LCIO::RCHBIT_ID1 ) << endl ;
        cout << "     LCIO::RCHBIT_TIME   : " << flag.test( LCIO::RCHBIT_TIME ) << endl ;
        cout << "     LCIO::RCHBIT_NO_PTR : " << flag.test( LCIO::RCHBIT_NO_PTR ) << endl ;

        int nHits =  col->getNumberOfElements() ;
        int nPrint = nHits > MAX_HITS ? MAX_HITS : nHits ;


        CellIDDecoder<RawCalorimeterHit> idDecoder( col ) ; 

        cout << endl;
        RawCalorimeterHit* hit=NULL;
        cout << header(hit);
        cout << tail(hit);
        for( int i=0 ; i< nPrint ; i++ ){
            hit = dynamic_cast<RawCalorimeterHit*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::RawCalorimeterHit>(hit,col);
        }
        cout << tail(hit);
    }

    void LCTOOLS::printClusters(const EVENT::LCCollection* col ){
        if( col->getTypeName() != LCIO::CLUSTER ){

            cout << " collection not of type " << LCIO::CLUSTER << endl ;
            return ;
        }
        cout << endl 
            << "--------------- " << "print out of "  << LCIO::CLUSTER << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        BitSet32 flag( col->getFlag() ) ;
        cout << "     LCIO::CLBIT_HITS : " << flag.test( LCIO::CLBIT_HITS ) << endl ;

        int nClusters =  col->getNumberOfElements() ;
        int nPrint = nClusters > MAX_HITS ? MAX_HITS : nClusters ;

        cout << endl;
        Cluster* clu = NULL;
        cout << header(clu);
        cout << tail(clu);
        for( int i=0 ; i< nPrint ; i++ ){
            clu = dynamic_cast<Cluster*>(col->getElementAt(i)) ;
            cout << lcio_short<EVENT::Cluster>(clu,col);
        }
        cout << tail(clu);

        // --- detailed PID info:

        cout <<  endl 
            << "  ------------ detailed PID info: --- " <<   endl  <<   endl 
            << "   algorithms : " 
            <<   endl ;


        PIDHandler pidH( col )  ;

        try{ 
            const IntVec& ids =  pidH.getAlgorithmIDs() ;

            for(unsigned i=0; i<ids.size() ; ++i){

                cout << "   [id: " << ids[i] << "]   " 
                    <<  pidH.getAlgorithmName( ids[i] ) 
                    << " - params: " ;

                const StringVec& pNames = pidH.getParameterNames( ids[i] ) ;

                for( StringVec::const_iterator it = pNames.begin() ; it != pNames.end() ; ++it ){

                    cout << " " << *it  ;
                }
                cout << endl ;
            }
            cout << endl ;
        }
        catch( UnknownAlgorithm &e ){
            cout << "- NA - ";
        }

        std::cout << endl
            << "   [cluster ] |  PDG   | likelihood |  type  |  algoId  | parameters : " << endl
            << "              |        |            |        |          |              "
            << endl ;


        for( int i=0 ; i< nPrint ; i++ ){

            Cluster* clu1 = 
                dynamic_cast<Cluster*>( col->getElementAt( i ) ) ;

            printf("   [%8.8x] " , clu1->id() ) ;


            for(unsigned int l=0;l<clu1->getParticleIDs().size();l++){

                if( l!=0)
                    printf("              " ) ;

                ParticleID* pid = clu1->getParticleIDs()[l] ;
                try{	
                    printf("| %6d | %6.4e | %6.6d | %8d | [",  
                            pid->getPDG() , 
                            pid->getLikelihood()  ,
                            pid->getType() ,
                            pid->getAlgorithmType() 
                          ) ;


                    const StringVec& pNames = pidH.getParameterNames(  pid->getAlgorithmType() ) ;

                    for(unsigned j=0;j< pNames.size() ;++j){

                        cout << " " <<  pNames[j]
                            << " : " <<  pid->getParameters()[j] << "," ; 

                    }
                }
                catch( UnknownAlgorithm &e ){
                    cout << "- NA - ";
                }

                cout << "]"<< endl ;
            }
            cout << endl ;

        }

        cout << endl 
            << "-------------------------------------------------------------------------------- " 
            << endl ;
    }

    void LCTOOLS::printVertices( const EVENT::LCCollection* col ){
        if( col->getTypeName() != LCIO::VERTEX ){

            cout << " collection not of type " << LCIO::VERTEX << endl ;
            return ;
        }
        cout << endl 
            << "--------------- " << "print out of "  << LCIO::VERTEX << " collection "
            << "--------------- " << endl << endl;

        printParameters( col->getParameters() ) ;

        int nVertices = col->getNumberOfElements() ;
        int nPrint = nVertices > MAX_HITS ? MAX_HITS : nVertices ;

        for( int i=0 ; i< nPrint ; i++ ){
            Vertex* v = dynamic_cast<Vertex*>( col->getElementAt( i ) ) ;

            if( i==0) { cout<<header(v); }
            //cout<<lcshort(v,col);
            cout << lcshort(v);
            if(i==nPrint-1){ cout<<tail(v); }
        }
    }

    void LCTOOLS::printReconstructedParticles( const EVENT::LCCollection* col ){

        if( col->getTypeName() != LCIO::RECONSTRUCTEDPARTICLE ){

            cout << " collection not of type " << LCIO::RECONSTRUCTEDPARTICLE << endl ;
            return ;
        }
        cout << endl 
            << "--------------- " << "print out of "  << LCIO::RECONSTRUCTEDPARTICLE << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        int nReconstructedParticles =  col->getNumberOfElements() ;
        int nPrint = nReconstructedParticles > MAX_HITS ? MAX_HITS : nReconstructedParticles ;

        cout << endl;
        ReconstructedParticle* recP=NULL;
        cout << header(recP);
        cout << tail(recP);

        for( int i=0 ; i< nPrint ; i++ ){
            recP = dynamic_cast<ReconstructedParticle*>( col->getElementAt( i ));
            cout << lcio_short<EVENT::ReconstructedParticle>(recP, col);
        }
        cout << tail(recP);

        // --- detailed PID info:

        cout <<  endl 
            << "  ------------ detailed PID info: --- " <<   endl  <<   endl 
            << "   algorithms : " 
            <<   endl ;


        PIDHandler pidH( col )  ;

        try{  
            const IntVec& ids =  pidH.getAlgorithmIDs() ;

            for(unsigned i=0; i<ids.size() ; ++i){

                cout << "   [id: " << ids[i] << "]   " 
                    <<  pidH.getAlgorithmName( ids[i] ) 
                    << " - params: " ;

                const StringVec& pNames = pidH.getParameterNames( ids[i] ) ;

                for( StringVec::const_iterator it = pNames.begin() ; it != pNames.end() ; ++it ){

                    cout << " " << *it  ;
                }
                cout << endl ;
            }
            cout << endl ;
        }
        catch( UnknownAlgorithm &e ){
            cout << "- NA - ";
        }

        std::cout << endl
            << "   [particle] |  PDG   | likelihood |  type  |  algoId  | parameters : " << endl
            << "              |        |            |        |          |              "
            << endl ;


        for( int i=0 ; i< nPrint ; i++ ){

            ReconstructedParticle* recP1 = 
                dynamic_cast<ReconstructedParticle*>( col->getElementAt( i ) ) ;

            printf("   [%8.8x] " , recP1->id() ) ;


            for(unsigned int l=0;l<recP1->getParticleIDs().size();l++){

                if( l!=0)
                    printf("              " ) ;

                ParticleID* pid = recP1->getParticleIDs()[l] ;
                try{	
                    printf("| %6d | %6.4e | %6.6d | %8d | [",  
                            pid->getPDG() , 
                            pid->getLikelihood()  ,
                            pid->getType() ,
                            pid->getAlgorithmType() 
                          ) ;

                    const StringVec& pNames = pidH.getParameterNames(  pid->getAlgorithmType() ) ;

                    for(unsigned j=0;j< pNames.size() ;++j){

                        cout << " " <<  pNames[j]
                            << " : " <<  pid->getParameters()[j] << "," ; 

                    }
                    cout << "]"<< endl ;

                }
                catch( UnknownAlgorithm &e ){
                    cout << "- NA - ";
                }

            }
            cout << endl ;

        } 

        cout << endl 
            << "-------------------------------------------------------------------------------- " 
            << endl ;

    }

    void LCTOOLS::printLCGenericObjects( const EVENT::LCCollection* col ){

        if( col->getTypeName() != LCIO::LCGENERICOBJECT ){

            cout << " collection not of type " << LCIO::LCGENERICOBJECT 
                << " [ " <<  col->getParameters().getStringVal("TypeName") << " ] " 
                << endl ;

            return ;
        }
        cout << endl 
            << "--------------- " << "print out of "  << LCIO::LCGENERICOBJECT << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        int nLCGenericObjects =  col->getNumberOfElements() ;
        int nPrint = nLCGenericObjects > MAX_HITS ? MAX_HITS : nLCGenericObjects ;

        //bool isFixedSize  =  BitSet32( col->getFlag() ).test( LCIO::GOBIT_FIXED );

        cout << endl;
        LCGenericObject* obj=NULL;
        cout << header(obj, col);
        cout << tail(obj);

        for( int i=0 ; i< nPrint ; i++ ){
            obj = dynamic_cast<LCGenericObject*>( col->getElementAt( i ));
            cout << lcio_short<EVENT::LCGenericObject>(obj, col);
            cout << tail(obj);
        }
        cout << endl;
    }


    void LCTOOLS::printMCParticles(const EVENT::LCCollection* col ) {

        if( col->getTypeName() != LCIO::MCPARTICLE ){

            cout << " collection not of type " << LCIO::MCPARTICLE << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::MCPARTICLE << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        int nParticles =  col->getNumberOfElements() ;


        cout << "  " << getSimulatorStatusString() << std::endl ;

        // fill map with particle pointers and collection indices
        typedef std::map< MCParticle*, int > PointerToIndexMap ;
        PointerToIndexMap p2i_map ;
        std::vector<MCParticle*> moms ;

        /*
           cout << endl;
           MCParticle* part=NULL;
           cout << header(part);
           cout << tail(part);

           for( int i=0 ; i< nParticles ; i++ ){
           part = dynamic_cast<MCParticle*>( col->getElementAt( i ));
           cout << lcio_short<EVENT::MCParticle>(part, col); //WARNING! 
        //not exact the same output as the code below:
        //<< lcio_short prints the id's of the parents/daughters particles, 
        //the "old" version prints the position in the collection
        }
         */

        for( int k=0; k<nParticles; k++){
            MCParticle* part =  dynamic_cast<MCParticle*>( col->getElementAt( k ) ) ;
            p2i_map[ part ] = k ; 

            moms.push_back( part ) ;
        }

        std::cout << endl
            <<  "[   id   ]index|      PDG |    px,     py,        pz    | px_ep,   py_ep , pz_ep      | energy  |gen|[simstat ]| vertex x,     y   ,   z     | endpoint x,    y  ,   z     |    mass |  charge |            spin             | colorflow | [parents] - [daughters]"    
            << endl 
            << endl ;

        // loop over collection - preserve order
        for(  int index = 0 ; index < nParticles ; index++){

            MCParticle* part =  dynamic_cast<MCParticle*>( col->getElementAt( index ) ) ;

            printf("[%8.8d]", part->id() );
            printf("%5d|"   , index );
            printf("%10d|" , part->getPDG() );
            printf("% 1.2e,% 1.2e,% 1.2e|" , 
                    part->getMomentum()[0] ,
                    part->getMomentum()[1] , 
                    part->getMomentum()[2] );
            printf("% 1.2e,% 1.2e,% 1.2e|" , 
                    part->getMomentumAtEndpoint()[0] ,
                    part->getMomentumAtEndpoint()[1] , 
                    part->getMomentumAtEndpoint()[2] );
            printf("% 1.2e|" , part->getEnergy() ) ; 

            printf(" %1d |" , part->getGeneratorStatus()  );
            printf("%s|" , getSimulatorStatusString( part ).c_str()  ); 
            printf("% 1.2e,% 1.2e,% 1.2e|" , 
                    part->getVertex()[0] , 
                    part->getVertex()[1] , 
                    part->getVertex()[2] );
            printf("% 1.2e,% 1.2e,% 1.2e|" , 
                    part->getEndpoint()[0] , 
                    part->getEndpoint()[1] ,  
                    part->getEndpoint()[2] );
            printf("% 1.2e|" , part->getMass() ) ; 
            printf("% 1.2e|" , part->getCharge() ) ; 

            printf("% 1.2e,% 1.2e,% 1.2e|" , 
                    part->getSpin()[0] ,
                    part->getSpin()[1] , 
                    part->getSpin()[2] );

            printf("  (%d, %d)   |" , 
                    part->getColorFlow()[0] ,
                    part->getColorFlow()[1] );

            cout << " [" ;

            for(unsigned int k=0;k<part->getParents().size();k++){
                if(k>0) cout << "," ;
                cout << p2i_map[ part->getParents()[k] ]  ;
            }
            cout << "] - [" ;
            for(unsigned int k=0;k<part->getDaughters().size();k++){
                if(k>0) cout << "," ;
                cout << p2i_map[ part->getDaughters()[k] ]  ;
            }
            cout << "] " << endl ;
        }

        cout << endl 
            << "-------------------------------------------------------------------------------- " 
            << endl ;
    }

    void LCTOOLS::printRelation( const EVENT::LCCollection* col ) {

        if( col->getTypeName() != LCIO::LCRELATION ){
            cout << " collection not of type " << LCIO::LCRELATION << endl ;
            return ;
        }

        cout << endl 
            << "--------------- " << "print out of "  << LCIO::LCRELATION << " collection "
            << "--------------- " << endl ;

        cout << endl 
            << "  flag:  0x" << hex  << col->getFlag() << dec << endl ;

        printParameters( col->getParameters() ) ;

        int nRel =  col->getNumberOfElements() ;

        cout << " fromType : " << col->getParameters().getStringVal("FromType")  << endl ;
        cout << " toType : "   << col->getParameters().getStringVal("ToType")    << endl ;

        cout << endl;
        LCRelation* rel=NULL;
        cout << header(rel);
        cout << tail(rel);

        for( int i=0; i< nRel; i++ ){
            rel = dynamic_cast<LCRelation*>( col->getElementAt( i ));
            cout << lcio_short<EVENT::LCRelation>(rel, col);
        }


    }


    int LCTOOLS::printDaughterParticles(const MCParticle* part, int index){

        int motherIndex = index - 1 ;
        // print this particles daughters
        for(unsigned int i=0; i<part->getDaughters().size();i++){

            MCParticle* d =  part->getDaughters()[i] ;

            cout << index++ << " [" << motherIndex << "] " 
                <<  d->getPDG() << " | ("
                <<  d->getMomentum()[0]  << ", "
                <<  d->getMomentum()[1]  << ", "
                <<  d->getMomentum()[2]  << ") | "
                <<  part->getGeneratorStatus() << " | "
                <<  part->getSimulatorStatus() << " | ("
                <<  d->getVertex()[0]    << ", "
                <<  d->getVertex()[1]    << ", "
                <<  d->getVertex()[2]    << ") | (" ;
            if( part->getEndpoint() != 0 ){
                cout <<  part->getEndpoint()[0]  << ", "
                    <<  part->getEndpoint()[1]  << ", "
                    <<  part->getEndpoint()[2]  << ") | " ;
            }else{
                cout << " not set ) | " ; 
            }
            cout <<  d->getMass()         << " | " 
                <<  d->getCharge()       << " | " 
                <<  d->getEnergy()      
                << endl ;	

            index = printDaughterParticles( part->getDaughters()[i] , index ) ;
        }
        return index ;

    }

    void LCTOOLS::printParameters( const EVENT::LCParameters& params ){

        StringVec intKeys ;
        int nIntParameters = params.getIntKeys( intKeys ).size() ;
        for(int i=0; i< nIntParameters ; i++ ){
            IntVec intVec ;
            params.getIntVals(  intKeys[i], intVec ) ;
            int nInt  = intVec.size()  ;   
            cout << " parameter " << intKeys[i] << " [int]: " ; 

            if( nInt == 0 ){ 
                cout << " [empty] " << std::endl ;
            }
            for(int j=0; j< nInt ; j++ ){
                cout << intVec[j] << ", " ;
            }
            cout << endl ;
        }
        StringVec floatKeys ;
        int nFloatParameters = params.getFloatKeys( floatKeys ).size() ;
        for(int i=0; i< nFloatParameters ; i++ ){
            FloatVec floatVec ;
            params.getFloatVals(  floatKeys[i], floatVec ) ;
            int nFloat  = floatVec.size()  ;   
            cout << " parameter " << floatKeys[i] << " [float]: " ; 
            if( nFloat == 0 ){ 
                cout << " [empty] " << std::endl ;
            }
            for(int j=0; j< nFloat ; j++ ){
                cout << floatVec[j] << ", " ;
            }
            cout << endl ;
        }
        StringVec stringKeys ;
        int nStringParameters = params.getStringKeys( stringKeys ).size() ;
        for(int i=0; i< nStringParameters ; i++ ){
            StringVec stringVec ;
            params.getStringVals(  stringKeys[i], stringVec ) ;
            int nString  = stringVec.size()  ;   
            cout << " parameter " << stringKeys[i] << " [string]: " ; 
            if( nString == 0 ){ 
                cout << " [empty] " << std::endl ;
            }
            for(int j=0; j< nString ; j++ ){
                cout << stringVec[j] << ", " ;
            }
            cout << endl ;
        }

    }

    void LCTOOLS::dumpRunHeader(const EVENT::LCRunHeader* run) {
        cout  << "  Run : " << run->getRunNumber() 
            << " - "      << run->getDetectorName() 
            << ":  "      << run->getDescription()  << endl ;

        printParameters( const_cast<EVENT::LCRunHeader*>(run)->parameters() ) ;

        cout  << " --------------------------------------------------------------- " << endl ;
    }



    std::string LCTOOLS::getSimulatorStatusString(const MCParticle* mcp){

        if( mcp == 0  ) {

            std::stringstream str ;

            str << "simulator status bits: [sbvtcls] "
                << " s: created in simulation"
                << " b: backscatter"
                << " v: vertex is not endpoint of parent" 
                << " t: decayed in tracker"
                << " c: decayed in calorimeter"
                << " l: has left detector"
                << " s: stopped"
                << " o: overlay"
                << std::endl ;

            return str.str() ;
        }

        std::string s("[    0   ]") ;

        if( mcp->getSimulatorStatus() == 0 ) 

            return s ;

        if( mcp->isCreatedInSimulation() )
            s[1]='s' ;
        else
            s[1]=' ' ;
        if( mcp->isBackscatter() )
            s[2]='b' ;
        else
            s[2]=' ' ;
        if( mcp->vertexIsNotEndpointOfParent() )
            s[3]='v' ;
        else
            s[3]=' ' ;
        if( mcp->isDecayedInTracker() )
            s[4]='t' ;
        else
            s[4]=' ' ;
        if( mcp->isDecayedInCalorimeter() )
            s[5]='c' ;
        else
            s[5]=' ' ;
        if( mcp->hasLeftDetector() )
            s[6]='l' ;
        else
            s[6]=' ' ;
        if( mcp->isStopped() )
            s[7]='s' ;
        else
            s[7]=' ' ;
        if( mcp->isOverlay() )
            s[8]='o' ;
        else
            s[8]=' ' ;

        return s ;
    }


    std::string LCTOOLS::getQualityBits(const SimTrackerHit* sth){

        if( sth == 0  ) {

            std::stringstream str ;

            str << "    quality bits: [os......] "
                << " o: hit from overlay"
                << " s: hit from secondary not from the MCParticle associated to it"
                << std::endl ;

            return str.str() ;
        }


        if( sth->getQuality() == 0 )
            return "[   0   ]";

        std::string s("[       ]");
        if( sth->isOverlay() )
            s[1]='o' ;
        else
            s[1]=' ' ;
        if( sth->isProducedBySecondary() )
            s[2]='s' ;
        else
            s[2]=' ' ;

        return s ;
    }

} // namespace

