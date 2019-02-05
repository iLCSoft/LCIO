#include "lcio.h"

#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "IO/LCRunListener.h"
#include "IMPL/LCEventImpl.h"
#include "Exceptions.h"

#include <cstdlib>
#include <memory>

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>

using namespace std ;
using namespace lcio ;

class BenchmarkListener : public IO::LCEventListener, public IO::LCRunListener {
public:
  BenchmarkListener(const BenchmarkListener&) = delete ;
  BenchmarkListener& operator=(const BenchmarkListener&) = delete ;
  
  BenchmarkListener(const std::string &fname) {
    m_pTFile = std::make_shared<TFile>( fname.c_str() , "RECREATE" ) ;
    m_pTTree = new TTree( "LCIOBenchmark", "LCIOBenchmark" ) ;
    m_pTTree->SetDirectory( m_pTFile.get() ) ;
    // cpu benchmarks
    m_pTTree->Branch( "CpuInfo_fIdle",         &m_cpuInfo.fIdle ) ;
    m_pTTree->Branch( "CpuInfo_fLoad15m",      &m_cpuInfo.fLoad15m ) ;
    m_pTTree->Branch( "CpuInfo_fLoad1m",       &m_cpuInfo.fLoad1m ) ;
    m_pTTree->Branch( "CpuInfo_fLoad5m",       &m_cpuInfo.fLoad5m ) ;
    m_pTTree->Branch( "CpuInfo_fSys",          &m_cpuInfo.fSys ) ;
    m_pTTree->Branch( "CpuInfo_fTotal",        &m_cpuInfo.fTotal ) ;
    m_pTTree->Branch( "CpuInfo_fUser",         &m_cpuInfo.fUser ) ;
    // memory benchmarks
    m_pTTree->Branch( "MemInfo_fMemFree",      &m_memInfo.fMemFree ) ;
    m_pTTree->Branch( "MemInfo_fMemTotal",     &m_memInfo.fMemTotal ) ;
    m_pTTree->Branch( "MemInfo_fMemUsed",      &m_memInfo.fMemUsed ) ;
    m_pTTree->Branch( "MemInfo_fSwapFree",     &m_memInfo.fSwapFree ) ;
    m_pTTree->Branch( "MemInfo_fSwapTotal",    &m_memInfo.fSwapTotal ) ;
    m_pTTree->Branch( "MemInfo_fSwapUsed",     &m_memInfo.fSwapUsed ) ;
    // process benchmarks
    m_pTTree->Branch( "ProcInfo_fCpuSys",      &m_procInfo.fCpuSys ) ;
    m_pTTree->Branch( "ProcInfo_fCpuUser",     &m_procInfo.fCpuUser ) ;
    m_pTTree->Branch( "ProcInfo_fMemResident", &m_procInfo.fMemResident, "ProcInfo_fMemResident/Long_t" ) ;
    m_pTTree->Branch( "ProcInfo_fMemVirtual",  &m_procInfo.fMemVirtual, "ProcInfo_fMemVirtual/Long_t" ) ;
    
    // differences on read record
    m_pTTree->Branch( "ReadTime",      &m_readTime ) ;
    m_pTTree->Branch( "ReadRSS",      &m_readResidentMemory, "ReadRSS/Long_t" ) ;
    
    // init benchamark vars
    gSystem->GetCpuInfo(&m_cpuInfo);
    gSystem->GetMemInfo(&m_memInfo);
    gSystem->GetProcInfo(&m_procInfo);
    m_residentMemoryReference = m_procInfo.fMemResident;
    m_lastClock = clock();
  }
  
  void processEvent(EVENT::LCEvent * /*evt*/) {
    getVariables();
    m_pTTree->Fill();
  }

  void modifyEvent(EVENT::LCEvent * /*evt*/) {
    /* nop */
  }
  
  void processRunHeader(EVENT::LCRunHeader * /*rh*/) {
    getVariables();
    m_pTTree->Fill();
  }

  void modifyRunHeader(EVENT::LCRunHeader * /*rh*/) {
    /* nop */
  }
  
  void getVariables() {
    // Get benchmarks and fill with the tree
    CpuInfo_t cpuInfo ;
    MemInfo_t memInfo ;
    ProcInfo_t procInfo ;
    clock_t currentClock = clock();
    gSystem->GetCpuInfo(&cpuInfo);
    gSystem->GetMemInfo(&memInfo);
    gSystem->GetProcInfo(&procInfo);
    
    m_readTime = (currentClock - m_lastClock) / static_cast<float>( CLOCKS_PER_SEC );
    m_readResidentMemory = ( procInfo.fMemResident - m_residentMemoryReference ) ;
    
    m_cpuInfo = cpuInfo;
    m_memInfo = memInfo;
    m_procInfo = procInfo;
    m_lastClock = currentClock;
  }
  
  void write() {
    m_pTFile->cd();
    m_pTTree->Write();
    m_pTFile->Close();
    m_pTFile = nullptr;
  }
  
private:
  // output root file with benchmarking info
  std::shared_ptr<TFile>           m_pTFile {nullptr} ;
  // output ROOT tree
  TTree                           *m_pTTree {nullptr} ;
  // Cpu benchmarks
  CpuInfo_t                        m_cpuInfo {} ;
  // Memory benchmarks
  MemInfo_t                        m_memInfo {} ;
  // Process benchmarks
  ProcInfo_t                       m_procInfo {} ;
  
  Float_t                          m_readTime {0} ;
  clock_t                          m_lastClock {} ;
  
  Long_t                           m_residentMemoryReference {0} ;
  Long_t                           m_readResidentMemory {0} ;
};

/** Small utility to benchmark lcio reading performances
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


