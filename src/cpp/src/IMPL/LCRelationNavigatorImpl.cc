#include "IMPL/LCRelationNavigatorImpl.h"

#include "EVENT/LCWgtRelation.h"

#include "EVENT/LCCollection.h"
#include "EVENT/LCIO.h"

using namespace EVENT ;

namespace IMPL {

  void LCRelationNavigatorImpl::initialize( const LCCollection* col ) {


    if( col->getTypeName() != LCIO::LCWGTRELATION ) {
      return ;
    }
    
    int n = col->getNumberOfElements() ;
    
    for(int i=0; i < n; i++){
      
      LCWgtRelation* rel = dynamic_cast<LCWgtRelation*>( col->getElementAt(i) )  ;
      
      addRelation( rel->getFrom() , rel->getTo() , rel->getWeight()  ) ; 
      
    }
  }

};
