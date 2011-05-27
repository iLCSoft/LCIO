#include "lcio.h"

#include "IO/LCReader.h"
#include "UTIL/LCStdHepRdr.h"
#include "IMPL/LCEventImpl.h"

#include <cstdlib>

static std::vector<std::string> FILEN ; 

using namespace std ;
using namespace lcio ;

/** Small utility to count the number of events in lcio files
 */

int main(int argc, char** argv ){

    // read file names from command line (only argument) 
    if( argc < 2) {
        cout << " count the number of events in the given input files" << endl << endl;
        cout << " usage:  lcio_event_counter <input-file1> [[input-file2],...]" << endl ;
        exit(1) ;
    }

    int nFiles = argc-1 ;
    long total_events = 0 ;

    for(int i=1 ; i <= nFiles ; i++){
        FILEN.push_back( argv[i] )  ;
    }


    // loop through the list of input files
    for(int i=0 ; i < nFiles ; i++){

        // check if file extension is stdhep
        if( FILEN[i].substr(FILEN[i].rfind(".") + 1) == "stdhep") {
            LCStdHepRdr stdhepReader( FILEN[i].c_str() );

            // the getNumberOfEvents() method in LCStdHepRdr reads the number of events
            // entered in the file header, there is no guarantee that this number is correct !!
            // total_events += stdhepReader.getNumberOfEvents() ;

            // in most stdhep files the real number of events is one less than the number declared in the header file !!
            total_events += stdhepReader.getNumberOfEvents() - 1 ;

            // using the code below the correct number is returned by looping through the
            // whole events in the file, this is however a performance killer!
            //try{
            //    while( true ){
            //        std::auto_ptr<LCEventImpl> evt( new LCEventImpl() ) ;
            //        stdhepReader.updateNextEvent( evt.get()  , "MCParticle" ) ;
            //        total_events++ ;
            //    }
            //}
            //catch( IO::EndOfDataException& e ) { /* no-op */ }
        }
        // otherwise use lcReader
        else{
            // create reader for lcio files
            LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;

            lcReader->open( FILEN[i] ) ;
            total_events += lcReader->getNumberOfEvents() ;
            lcReader->close() ;

            delete lcReader ;
        }
    }

    cout <<  total_events << endl ;

    return 0 ;
}


