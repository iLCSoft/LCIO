#include "lcio.h"

#include "IO/LCReader.h"
#include "UTIL/LCTOOLS.h"
#include "IMPL/LCEventImpl.h"

#include <cstdlib>
#include <mutex>
#include <future>

static std::vector<std::string> FILEN ; 

using namespace std ;
using namespace lcio ;

void dumpEvent( EVENT::LCEvent *event ) {
  UTIL::LCTOOLS::dumpEvent( event );
}

/** Small utility to dump events in parallel from different files
 */
int main(int argc, char** argv ){

    // read file names from command line (only argument) 
    if( argc < 2) {
        cout << " dump events in parallel from different input files" << endl << endl;
        cout << " usage:  lcio_parallel_read <input-file1> [[input-file2],...]" << endl ;
        exit(1) ;
    }

    int nFiles = argc-1 ;
    std::vector<std::string> inputFiles ;
    std::vector<std::future<int>> futures;

    for(int i=1 ; i <= nFiles ; i++) {
        inputFiles.push_back( argv[i] )  ;
    }
    
    std::mutex printMutex;
    
    // loop through the list of input files
    for(unsigned int i=0 ; i < inputFiles.size() ; i++) {
      
      std::string fileName = inputFiles.at(i) ;
      
      futures.push_back( std::async( std::launch::async, [&printMutex,fileName]() {
        // create local reader for lcio file
        LCFactory factory ;
        LCReader* lcReader = factory.createLCReader() ;
        lcReader->open( fileName ) ;
        
        EVENT::LCEvent *lcEvent = nullptr ;
        int nevents = 0 ;
        
        while ( (lcEvent = lcReader->readNextEvent()) ) {
          // lock before printing
          std::lock_guard<std::mutex> lock( printMutex ) ;
          dumpEvent( lcEvent ) ;
          nevents ++ ;
        }
        
        lcReader->close() ;
        delete lcReader ;
        return nevents ;
      })) ;
    }
    
    int totalNEvents = 0 ;
    
    for( unsigned int i=0 ; i<futures.size() ; i++ ) {
      totalNEvents += futures.at(i).get() ;
    }
    
    std::cout << "Printed " << totalNEvents << " events from " << inputFiles.size() << " files" << std::endl ;

    return 0 ;
}


