#include "UTIL/CheckCollections.h"

#include "lcio.h"

#include "MT/LCReader.h"
#include "IMPL/LCCollectionVec.h"


namespace UTIL{

  void CheckCollections::checkFiles( const std::vector<std::string> fileNames ){

    for( auto n : fileNames )
      checkFile( n ) ;
  }

  void CheckCollections::checkFile( const std::string fileName ) {

    MT::LCReader lcReader(0) ; 

    lcReader.open( fileName ) ;

    std::unique_ptr<EVENT::LCEvent> evt ;

    //----------- the event loop -----------
    while( (evt = lcReader.readNextEventHeader()) != 0 ) {
      
      auto colNames = evt->getCollectionNames() ;
      
      for(unsigned i=0,n= colNames->size() ; i<n ; ++i){
	
	auto name = (*colNames)[i] ;
	
	auto it = _map.find( name ) ;

	if( it == _map.end() ){

	  auto col = evt->getCollection( name ) ;

	  const auto[ itx, inserted] = _map.emplace( name,  std::make_pair( col->getTypeName() , 0 )  )   ;

	  it = itx ;
	}

	it->second.second ++ ;
      }

      _nEvents ++ ;
    }

    lcReader.close() ;
  }


  CheckCollections::Set CheckCollections::getMissingCollections(){
    Set  s ;
    for(auto e : _map ){
      if( e.second.second != _nEvents )
	s.emplace( std::make_pair(e.first, e.second.first ) ) ; 
    }
    return s ;
  }
  

  CheckCollections::Set CheckCollections::getConsistentCollections(){
    Set  s ;
    for(auto e : _map ){
      if( e.second.second == _nEvents )
	s.emplace( std::make_pair(e.first, e.second.first ) ) ; 
    }
    return s ;
  }  

  void CheckCollections::patchCollections(EVENT::LCEvent* evt ) {

    for(auto c : _patchCols ){

      try{

	evt->getCollection( c.first ) ;

      } catch( EVENT::DataNotAvailableException& e) {

	evt->addCollection( new IMPL::LCCollectionVec(c.second), c.first ) ;
      }
    }
  }

  
  void CheckCollections::print(  std::ostream& os ){

    unsigned width = 50 ;

    os << " ================================================================ " << std::endl ;
    os << std::endl <<  "  " <<  _nEvents << " events read " << std::endl  ;
    os << "     collections that are not in all events :  [# events where col is present]" << std::endl ;
    os << " ================================================================ " << std::endl ;
    
    for(auto e : _map ){
      
      if( e.second.second != _nEvents )
	os << "     " <<  std::setw(width) << std::left << e.first << " " <<std::setw(width) << e.second.first << " [" <<  e.second.second << "]"<< std::endl ;
    }
    
    os << " ================================================================ " << std::endl ;
    os << "     collections that are in all events : " << std::endl ;
    os << " ================================================================ " << std::endl ;
    
    
    for(auto e : _map ){
      
      if( e.second.second == _nEvents )
	os << "     " <<  std::setw(width) << std::left << e.first << " " <<std::setw(width) << e.second.first << "  [" <<  e.second.second << "]"<< std::endl ;
    }
    os << " ================================================================ " << std::endl ;

  }

}
