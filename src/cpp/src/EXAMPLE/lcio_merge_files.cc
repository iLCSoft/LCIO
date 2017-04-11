#include "lcio.h"

#include "IO/LCWriter.h"
#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "IO/LCRunListener.h"

#include "EVENT/LCIO.h"

#include <cstdlib>
#include <iostream>

static std::vector<std::string> FILEN ;

using namespace std ;
using namespace lcio ;


/** lcio tool for merging files on an event by event and run by run basis */


// class for processing run and event records 
class RunEventProcessor : public LCRunListener, public LCEventListener{

    protected:
        LCWriter* lcWrt{NULL} ;
        int nEvent{0} ;
        unsigned int _nFiles{0} ;

    public:

       RunEventProcessor(const RunEventProcessor&) = delete ;
       RunEventProcessor operator=(const RunEventProcessor&) = delete ;

       RunEventProcessor(const char* outFileName, unsigned int nFiles) : nEvent(0), _nFiles(nFiles) {

            // open outputfile
            lcWrt = LCFactory::getInstance()->createLCWriter() ;

            try{ lcWrt->open( outFileName , LCIO::WRITE_NEW ) ; } 

            catch(IOException& e){
                cout << "[RunEventProcessor()] Can't open file for writing -  " 
                    << e.what()  << endl ;
                exit(1) ;
            }

        }

        ~RunEventProcessor(){
            // close outputfile
            lcWrt->close()  ;
            cout << "merged " << nEvent << " events from " << _nFiles << " input files." << endl ; 
        }
  
       void modifyEvent( LCEvent * /* evt */) {  /*no changes to event ! */ ; }

        void processEvent( LCEvent * evt ) {  

            // just copy events to outputfiles  
            lcWrt->writeEvent( evt ) ;
            nEvent ++ ;
            //     cout << "         event: " << evt->getEventNumber() 
            // 	 << " [run: " << evt->getRunNumber() << "] copied" << endl ;
        }

        void modifyRunHeader(LCRunHeader* /*run*/ ){  /*no changes to event ! */ ;}

        // don't manipulate run headers - use analyze 
        void processRunHeader( LCRunHeader* run){

            cout << "." ; 
            cout.flush() ;
            // just copy run headers to the outputfile
            lcWrt->writeRunHeader( run ) ;
            //     cout << "run : " << run->getRunNumber() << " [" 
            // 	 << run->getDescription() << "] - header copied " << endl ;
        }

} ;

//=============================================================================

int main(int argc, char** argv ){

    char* outFileName ;

    try{ // a large try block for debugging ....

        if( argc < 3 ){
            cout << "usage: " << argv[0] << " <output-file> <input-file1> [[input-file2],...]" << endl ;
            exit(1)  ;
        }

        // read file names from command line 
        outFileName  = argv[1] ;

        unsigned int nFiles = argc - 2 ;

        for(unsigned int i=0 ; i < nFiles ; i++){
            FILEN.push_back( argv[2+i] )  ; // 2+ because of program-name and output-file
        }

        // create reader and writer for input and output streams 
        LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;

        try{
            lcReader->open( FILEN ) ;
        } 
        catch( IOException& e){
            cout << "Can't open files : " << e.what()  << endl ;
            exit(1) ;
        }


        // create a new RunEventProcessor, register it with the reader
        // and read and proccess the whole stream 
        {
            RunEventProcessor evtProc( outFileName, nFiles ) ;

            lcReader->registerLCRunListener( &evtProc ) ; 
            lcReader->registerLCEventListener( &evtProc ) ; 

            lcReader->readStream() ;
        } 

        lcReader->close() ;
        delete lcReader ;

    }

    catch(exception& ex){
        cout << "something went wrong:  " << ex.what() << endl ; 
    }

    return 0 ;

}

//=============================================================================

