#include "MT/LCReader.h"

#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCRunHeaderImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "UTIL/LCTOOLS.h"
#include "Exceptions.h"

#include <ctime>
#include <iostream>

/** Performance profiling of LCIO reader
 */
int main(int argc, char** argv ){
  
  if( argc < 2 ) { 
    std::cout << "Usage:" << std::endl ; 
    std::cout << "   lcio_read_perf <input-file>" << std::endl ;
    std::cout << std::endl ;
    std::cout << " Read input file and print read performances" << std::endl ;
    return 1 ;
  }
  
  const std::string filen = argv[1] ;
  
  try {
    MT::LCReader reader( MT::LCReader::directAccess | MT::LCReader::lazyUnpack ) ;
    reader.open( filen ) ;
    std::unique_ptr<EVENT::LCEvent> event = nullptr ;
    
    float readTime = 0.f ;
    float decodeTime = 0.f ;
    size_t event_counter = 0 ;
    
    while( 1 ) {
      auto read_begin = clock() ;
      event = reader.readNextEvent() ;
      auto read_end = clock() ;
      if ( nullptr == event ) {
        break ;
      }
      event_counter++ ;
      readTime += ( (read_end - read_begin) / static_cast<float>(CLOCKS_PER_SEC) ) ;
      auto colnames = event->getCollectionNames() ;
      
      if ( ! colnames->empty() ) {
        auto colname = colnames->at(0) ;
        auto decode_begin = clock() ;
        event->getCollection( colname ) ;
        auto decode_end = clock() ;
        decodeTime += ( (decode_end - decode_begin) / static_cast<float>(CLOCKS_PER_SEC) ) ;
      }
    }
    std::cout << "Read " << event_counter << " events from file " << filen << std::endl ;
    std::cout << "   Total read time:  " << (readTime + decodeTime)*1000. << " ms ( " << ((readTime + decodeTime)*1000. ) / event_counter << " ms / event )" << std::endl ;
    std::cout << "   IO time:          " << readTime*1000. << " ms ( " << (readTime*1000. ) / event_counter << " ms / event )" << std::endl ;
    std::cout << "   Decode time:      " << decodeTime*1000. << " ms ( " << (decodeTime*1000. ) / event_counter << " ms / event )" << std::endl ;
  } 
  catch( EVENT::Exception& ex) {
    std::cout << " an exception occured: " << std::endl ;
    std::cout << "  " << ex.what() << std::endl ;
    return 1 ;
  }

  return 0 ;
}

