#include "lcio.h"

#include "MT/Types.h"
#include "MT/LCReader.h"
#include "MT/LCReaderListener.h"
#include "UTIL/LCTOOLS.h"
#include "IMPL/LCEventImpl.h"

#include <cstdlib>
#include <mutex>
#include <future>
#include <functional>
#include <thread>
#include <unistd.h>

static std::vector<std::string> FILEN ; 

using namespace std ;
using namespace lcio ;
using LCEventPtr = MT::LCEventPtr;
using LCRunHeaderPtr = MT::LCRunHeaderPtr;

std::mutex printMutex;
#define SAFE_PRINT( message ) { std::lock_guard<std::mutex> lock(printMutex); std::cout << message << std::endl; }
#define SAFE_CODE( code ) { std::lock_guard<std::mutex> lock(printMutex); code; }

class Scheduler final : public MT::LCReaderListener {
private:
  typedef void                               task_return_type ;
  typedef std::future<task_return_type>      future_type ;
  typedef std::vector<future_type>           future_list ;
  
public:
  Scheduler( unsigned int maxNTasks = std::thread::hardware_concurrency() ) :
    _maxNTasks(maxNTasks) {
    SAFE_PRINT( "Scheduler created with maxNTasks = " << _maxNTasks ) ;
  }
  
  ~Scheduler() {
    waitForAll();
  }
  
  void startTask( LCEventPtr event ) {
    while ( not canStartNewTask() ) {
      processFinishedTasks() ;
      usleep(1000) ;
    }
    SAFE_PRINT( "Starting new task ..." ) ;
    _futures.push_back( std::async( std::launch::async, [](LCEventPtr evt) {
      
      SAFE_PRINT( "Task processing run no " 
        << evt->getRunNumber() 
        << ", event no "
        << evt->getEventNumber() ) ;
        
      // n usec to sleep within the task
      int t = (rand() / (float) RAND_MAX) * 1000000 ;
      usleep( t ) ;
    }, event)) ;
  }
  
  void waitForAll() {
    SAFE_PRINT( "waitForAll()" ) ;
    for ( unsigned int i=0 ; i<_futures.size() ; ++i ) {
      _futures.at(i).get();
    }
    _futures.clear();
  }
  
private:
  
  void processFinishedTasks() {
    auto iter = _futures.begin() ;
    while ( iter != _futures.end() ) {
      auto status = iter->wait_for( std::chrono::seconds(0) ) ;
      if ( status == std::future_status::ready ) {
        // process finished task and remove it for the pending task list
        iter->get();
        iter = _futures.erase( iter );
      }
      else {
        iter++ ;        
      }
    }
  }
  
  bool canStartNewTask() const {
    return ( _futures.size() < _maxNTasks ) ;
  }
  
  void processEvent( LCEventPtr event ) override {
    startTask( event ) ;
  }
  
  void processRunHeader( LCRunHeaderPtr hdr ) override {
    // Wait for all event task to finish
    // and then process the new run header
    waitForAll();
    UTIL::LCTOOLS::dumpRunHeader( hdr.get() );
  }
  
private:
  unsigned int                   _maxNTasks {} ;
  future_list                    _futures {} ;     
};

/** Small utility to dump events in parallel from different files
 */
int main(int argc, char** argv ){

    // read file names from command line (only argument) 
    if( argc < 2) {
        cout << " start tasks to process events in parallel" << endl << endl;
        cout << " usage:  lcio_parallel_processing <input-file1> [[input-file2],...]" << endl ;
        exit(1) ;
    }

    int nFiles = argc-1 ;
    std::vector<std::string> inputFiles ;

    for(int i=1 ; i <= nFiles ; i++) {
        inputFiles.push_back( argv[i] )  ;
    }
    
    // The LCReader to read events and runs
    MT::LCReader reader( MT::LCReader::directAccess ) ;
    reader.open( inputFiles );
    
    unsigned int nThreads = std::thread::hardware_concurrency() ;
    char *nthreadsenv = getenv( "LCIO_MAX_THREADS" ) ;
    if ( nthreadsenv ) {
      nThreads = atoi( nthreadsenv );
      if ( nThreads <= 0 ) {
        nThreads = 1;
      }
    }
    
    // The task scheduler
    Scheduler scheduler (nThreads);
    MT::LCReaderListenerList listeners ;
    listeners.insert( &scheduler ) ;
    
    // Read stream and process events and run headers
    while (1) {
      try {
        reader.readNextRecord( listeners );        
      }
      catch ( const IO::EndOfDataException& ) {
        break;
      }
    }

    return 0 ;
}


