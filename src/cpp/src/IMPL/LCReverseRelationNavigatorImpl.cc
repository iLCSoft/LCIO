#include "IMPL/LCReverseRelationNavigatorImpl.h"

#include "EVENT/LCWgtRelation.h"

#include "EVENT/LCCollection.h"
#include "EVENT/LCIO.h"

using namespace EVENT ;

namespace IMPL {

  void LCReverseRelationNavigatorImpl::initialize( const LCCollection* col ) {


    if( col->getTypeName() != LCIO::LCWGTRELATION ) {
      return ;
    }
    
    int n = col->getNumberOfElements() ;
    
    for(int i=0; i < n; i++){
      
      LCWgtRelation* rel = dynamic_cast<LCWgtRelation*>( col->getElementAt(i) )  ;
      
      // NB: the order of from ond to is reversed !
      addRelation( rel->getTo() , rel->getFrom() , rel->getWeight()  ) ; 
      
    }
  }

};
