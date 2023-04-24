#include "UTIL/CheckCollections.h"

#include "lcio.h"

#include "MT/LCReader.h"
#include "IMPL/LCCollectionVec.h"

#include <iomanip>

namespace UTIL{

  void CheckCollections::checkFiles( const std::vector<std::string>& fileNames ){

    for( auto n : fileNames )
      checkFile( n ) ;
  }

  void CheckCollections::checkFile( const std::string& fileName ){

    MT::LCReader lcReader(MT::LCReader::directAccess) ; 
    lcReader.open( fileName ) ;
    //----------- the event loop -----------
    while( const auto evt = lcReader.readNextEventHeader() ) {
      
      const auto* colNames = evt->getCollectionNames() ;
      
      for(const auto& name : *colNames){
	
	auto it = _map.find( name ) ;

	if( it == _map.end() ){

	  auto col = evt->getCollection( name ) ;
    /* If the type of a collection is LCRelation we use want to read the entire collections instead of just the header to get the 'toType' and 'fromType'.  
    * setReadCollectionNames({name}) allows reading of onlz certain colloctions by name instead of an entire event.
    * This flag has to be unset after reading in order for the reading of the headers to function.
    */
    std::string typeString;
    if (col->getTypeName() == "LCRelation"){     
      lcReader.setReadCollectionNames({name});
      
      auto fullEvt = lcReader.readEvent(evt->getRunNumber(), evt->getEventNumber());
      lcReader.setReadCollectionNames({});
      auto fullcol = fullEvt->getCollection( name ) ;
      const auto& params = fullcol->getParameters();
      const auto& fromType = params.getStringVal("FromType");
      const auto& toType = params.getStringVal("ToType");
      if (fromType == ""|| toType == ""){
        std::cout<< "WARNING! : Realtion " << name <<" does not have the 'fromType' and 'toType' set."<<std::endl;
      }
      typeString = "LCRelation["+fromType+","+toType+"]";
    }
    else {
      typeString = col->getTypeName();
    }
	  const auto[ itx, inserted] = _map.emplace( name,  std::make_pair( std::move(typeString) , 0 )  ) ;

	  it = itx ;
	}

	it->second.second ++ ;
      }

      _nEvents ++ ;
    }

    lcReader.close() ;
  }


  CheckCollections::Vector CheckCollections::getMissingCollections() const {
    Vector  s ;
    for(const auto& e : _map ){
      if( e.second.second != _nEvents )
	s.push_back( {e.first, e.second.first } ) ;
    }
    return s ;
  }
  

  CheckCollections::Vector CheckCollections::getConsistentCollections() const {
    Vector  s ;
    for(auto e : _map ){
      if( e.second.second == _nEvents )
	s.push_back( {e.first, e.second.first }) ;
    }
    return s ;
  }  

  void CheckCollections::patchCollections(EVENT::LCEvent* evt ) const {

    for(auto c : _patchCols ){

      try{

	evt->getCollection( c.first ) ;

      } catch( EVENT::DataNotAvailableException& e) {
      
      if (c.second.size()> 11){
      if (c.second[10] != '['){
	      evt->addCollection( new IMPL::LCCollectionVec(c.second), c.first ) ;
        }
      else{
        size_t delim = c.second.find(',');
        std::string from = c.second.substr(11, delim-11);
        std::string to = c.second.substr(delim+1, c.second.size()-delim-2);
        auto relationColl = new IMPL::LCCollectionVec("LCRelation");
        auto& params = relationColl->parameters();

        params.setValue("FromType",from);
        params.setValue("ToType",to);
        evt->addCollection( relationColl, c.first ) ;
      }
      }
      }
    }
  }

  
  void CheckCollections::print(  std::ostream& os ) const {

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
