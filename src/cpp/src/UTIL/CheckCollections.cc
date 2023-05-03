#include "UTIL/CheckCollections.h"

#include "lcio.h"

#include "MT/LCReader.h"
#include "IMPL/LCCollectionVec.h"

#include <iomanip>

namespace UTIL{

  void CheckCollections::checkFiles( const std::vector<std::string>& fileNames, bool quiet){

    for( auto n : fileNames )
      checkFile( n ,quiet) ;
  }

  void CheckCollections::checkFile( const std::string& fileName, bool quiet){

    MT::LCReader lcReader(MT::LCReader::directAccess) ; 
    lcReader.open( fileName ) ;
    //----------- the event loop -----------
    while( const auto evt = lcReader.readNextEventHeader() ) {
      
      const auto* colNames = evt->getCollectionNames() ;
      
      for(const auto& name : *colNames){
	
	auto it = _map.find( name ) ;

	if( it == _map.end() ){

	  auto col = evt->getCollection( name ) ;
    // If the type of a collection is LCRelation we want to read the entire
    //  collections instead of just the header to get the 'ToType' and
    //  'FromType'. setReadCollectionNames({name}) allows reading of only
    //  certain collections by name instead of an entire event. This flag has to
    //  be unset after reading in order for the reading of the headers to
    //  function properly.
    std::string typeString;
    if (col->getTypeName() == "LCRelation"){     
      lcReader.setReadCollectionNames({name});
      auto fullEvt = lcReader.readEvent(evt->getRunNumber(), evt->getEventNumber());
      lcReader.setReadCollectionNames({});

      auto fullcol = fullEvt->getCollection( name ) ;
      const auto& params = fullcol->getParameters();
      const auto& fromType = params.getStringVal("FromType");
      const auto& toType = params.getStringVal("ToType");
      if (quiet == false){
        if (fromType == ""|| toType == ""){
          std::cout<< "WARNING! : Relation " << name <<" does not have the 'FromType' and 'ToType' set."<<std::endl;
        }
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

  // Obtain the from and to type from the encoded "LCRelation[From,To]"
  std::tuple<std::string_view, std::string_view> getToFromType(const std::string_view fullType) {
    auto delim = fullType.find(',');
    constexpr auto prefixLen = 11u; // length of "LCRelation["

    return {fullType.substr(prefixLen, delim - prefixLen),
            fullType.substr(delim + 1, fullType.size() - delim - 2)}; // need to strip final "]" as well
  }

  void CheckCollections::patchCollections(EVENT::LCEvent* evt ) const {

    for(const auto& c : _patchCols ){

      try{
        auto* coll = evt->getCollection( c.first ) ;
        // For LCRelations we still have to check whether the FromType and
        // ToType are set and correct in case they are not
        if (coll->getTypeName() == "LCRelation") {
          auto& params = coll->parameters();
          if (params.getStringVal("FromType").empty() || params.getStringVal("ToType").empty()) {
            const auto [from, to] = getToFromType(c.second);
            params.setValue("FromType", std::string(from));
            params.setValue("ToType", std::string(to));
          }
        }
      } catch( EVENT::DataNotAvailableException& e) {
      //10 is the length of the String LCRelation after which the bracket is and the "ToType" and "FromType" start.
        if (c.second.size() > 10) {
          if (c.second[10] != '[') {
            evt->addCollection( new IMPL::LCCollectionVec(c.second), c.first ) ;
          } else{
            auto relationColl = new IMPL::LCCollectionVec("LCRelation");
            auto& params = relationColl->parameters();

            const auto [from, to] = getToFromType(c.second);
            params.setValue("FromType", std::string(from));
            params.setValue("ToType", std::string(to));
            evt->addCollection( relationColl, c.first ) ;
          }
        }
      }
    }
  }

  
  void CheckCollections::print(  std::ostream& os ,bool minimal) const {

    unsigned width = 50 ;
    if (minimal == false){
    os << " ================================================================ " << std::endl ;
    os << std::endl <<  "  " <<  _nEvents << " events read " << std::endl  ;
    os << "     collections that are not in all events :  [# events where col is present]" << std::endl ;
    os << " ================================================================ " << std::endl ;
    }
    if (minimal == false){
      for(auto e : _map ){
        
        if( e.second.second != _nEvents )
    os << "     " <<  std::setw(width) << std::left << e.first << " " <<std::setw(width) << e.second.first << " [" <<  e.second.second << "]"<< std::endl ;
      }
    }

    if (minimal == false){
    os << " ================================================================ " << std::endl ;
    os << "     collections that are in all events : " << std::endl ;
    os << " ================================================================ " << std::endl ;
    }
    if (minimal == false){
      for(auto e : _map ){
        
        if( e.second.second == _nEvents )
    os << "     " <<  std::setw(width) << std::left << e.first << " " <<std::setw(width) << e.second.first << "  [" <<  e.second.second << "]"<< std::endl ;
      }
    }
    else{
      for(auto e : _map ){
      
	os << "     " <<  std::setw(width) << std::left << e.first << " " <<std::setw(width) << e.second.first << std::endl ;
    }
    }
    if (minimal == false){
    os << " ================================================================ " << std::endl ;
    }
  }

}
