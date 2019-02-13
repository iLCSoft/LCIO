#include "lcio.h"

#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "IO/LCRunListener.h"
#include "IMPL/LCEventImpl.h"
#include "Exceptions.h"

#include <cstdlib>
#include <memory>
#include <sstream>

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>

using namespace std ;
using namespace lcio ;

/**
 *  @brief  BenchmarkListener class
 *  Simple event/run listener getting memory/proc statistics on callback
 */
class BenchmarkListener : public IO::LCEventListener, public IO::LCRunListener {
public:
  BenchmarkListener(const BenchmarkListener&) = delete ;
  BenchmarkListener& operator=(const BenchmarkListener&) = delete ;
  
  /**
   *  @brief  Constructor
   *  
   *  @param  fname the ROOT output file with statistics
   */
  BenchmarkListener(const std::string &fname) {
    m_pTFile = std::make_shared<TFile>( fname.c_str() , "RECREATE" ) ;
    m_pTTree = new TTree( "LCIOBenchmark", "LCIOBenchmark" ) ;
    m_pTTree->SetDirectory( m_pTFile.get() ) ;
    // global benchmarking
    m_pTTree->Branch( "ProcRSS",               &m_procRSS, "ProcRSS/Long_t" ) ;
    // read benchmarks
    m_pTTree->Branch( "ReadTime",              &m_readTime ) ;
    m_pTTree->Branch( "ReadRSS",               &m_readResidentMemory, "ReadRSS/Long_t" ) ;
    // write benchmarks
    m_pTTree->Branch( "WriteTime",             &m_writeTime ) ;
    m_pTTree->Branch( "WriteRSS",              &m_writeRSS, "WriteRSS/Long_t" ) ;
    // the entry id for easy plotting
    m_pTTree->Branch( "Entry",                 &m_entry ) ;
    // init benchamark vars
    ProcInfo_t procInfo ;
    gSystem->GetProcInfo(&procInfo);
    m_residentMemoryReference = procInfo.fMemResident;
    m_firstClock = m_lastClock = clock();
    // initialize the LCWriter instance
    m_lcWriter = LCFactory::getInstance()->createLCWriter();
    m_lcioOutputFileName << "LCIO_benchmarking_" << gSystem->GetPid() << ".slcio";
    std::cout << "Opening output LCIO file: " << m_lcioOutputFileName.str() << std::endl;
    m_lcWriter->open( m_lcioOutputFileName.str(), LCIO::WRITE_NEW );
  }
  
  /**
   *  @brief  Destructor
   */
  ~BenchmarkListener() {
    std::cout << "Closing and deleting LCIO file " << m_lcioOutputFileName.str() << std::endl;
    m_lcWriter->close();
    delete m_lcWriter;
    std::remove( m_lcioOutputFileName.str().c_str() );
    std::cout << "Total processing time was " << float(m_lastClock - m_firstClock) / CLOCKS_PER_SEC << " secs" << std::endl;
  }

  /**
   *  @brief  Extract and fill benchmark variables
   *  
   *  @param  evt the event to process
   */
  void processEvent(EVENT::LCEvent *evt) {
    getVariables(evt);
    m_pTTree->Fill();
  }
  
  /**
   *  @brief  Extract benchmark variables
   *  
   *  @param  event the event to process
   */
  void getVariables(EVENT::LCEvent *event) {
    // Get benchmarks and fill with the tree
    // "Read event" variables 
    ProcInfo_t procInfo, procInfo2 ;
    clock_t currentClock = clock();
    m_readTime = float( currentClock - m_lastClock ) / CLOCKS_PER_SEC ;
    gSystem->GetProcInfo(&procInfo); 
    m_procRSS = procInfo.fMemResident;
    m_readResidentMemory = ( procInfo.fMemResident - m_residentMemoryReference ) ;    
    // "Write event" variables
    clock_t writeStart = clock() ;
    m_lcWriter->writeEvent(event) ;
    clock_t writeEnd = clock() ;
    gSystem->GetProcInfo(&procInfo2) ;
    m_writeTime = float( writeEnd - writeStart ) / CLOCKS_PER_SEC ;
    m_writeRSS = ( procInfo2.fMemResident - procInfo.fMemResident ) ;
    m_residentMemoryReference = procInfo2.fMemResident ;    
    m_lastClock = clock();
    m_entry++;
  }
  
  /**
   *  @brief  Write benchmarks to ROOT file
   */
  void write() {
    m_pTFile->cd();
    m_pTTree->Write();
    m_pTFile->Close();
    m_pTFile = nullptr;
  }
  
private:
  void modifyEvent(EVENT::LCEvent * /*evt*/) { /* nop */ }
  void processRunHeader(EVENT::LCRunHeader * /*rh*/) { /* nop */ }
  void modifyRunHeader(EVENT::LCRunHeader * /*rh*/) { /* nop */ }
  
private:
  /// Output root file with benchmarking info
  std::shared_ptr<TFile>           m_pTFile {nullptr} ;
  /// Output ROOT tree
  TTree                           *m_pTTree {nullptr} ;
  /// LCIO output file writer
  IO::LCWriter                    *m_lcWriter {nullptr} ;
  /// The LCIO output file name
  std::stringstream                m_lcioOutputFileName {} ;
  /// Event reading time
  Float_t                          m_readTime {0} ;
  /// The first clock measurement on instance creation
  clock_t                          m_firstClock {0} ;
  /// The last clock measurement on processEvent()
  clock_t                          m_lastClock {0} ;
  /// The resident set size on instance creation (reference)
  Long_t                           m_residentMemoryReference {0} ;
  /// The resident set size after reading an event (minus reference)
  Long_t                           m_readResidentMemory {0} ;
  /// The current entry id while filling the tree (for easy plotting)
  Int_t                            m_entry {0} ;
  /// The event write time
  Float_t                          m_writeTime {0} ;
  /// The resisdent set size after writing
  Long_t                           m_writeRSS {0} ;
  /// The process resident set size taken in processEvent callback
  Long_t                           m_procRSS {0} ;
};

/** Small utility to benchmark lcio reading/writing performances
 */
int main(int argc, char** argv ){

    // read file names from command line (only argument) 
    if( argc < 3) {
        cout << " read/write LCIO file and output io benchmarks in a ROOT file" << endl << endl;
        cout << " usage:  lcio_io_benchmark <input-file> <output-file> [<nevents>]" << endl ;
        exit(1) ;
    }
    std::string infilen, outfilen ;
    int nevents = -1;
    infilen = argv[1] ;
    outfilen = argv[2] ;
    if( argc > 3 ) {
      nevents = atoi( argv[3] );
    }
    
    auto lcReader = LCFactory::getInstance()->createLCReader() ;    
    lcReader->open( infilen ) ;
    
    BenchmarkListener *benchmark = new BenchmarkListener( outfilen );
    lcReader->registerLCEventListener(benchmark);
    lcReader->registerLCRunListener(benchmark);
    
    try {
      // read the stream, filling benchmark variables
      if( nevents > 0 ) {
        lcReader->readStream( nevents );
      }
      else {
        lcReader->readStream();      
      }
    }
    catch(IO::EndOfDataException) {
        
    }

    lcReader->close() ;
    benchmark->write();
    delete benchmark;
    delete lcReader ;

    return 0 ;
}


