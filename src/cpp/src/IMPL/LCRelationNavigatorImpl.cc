#include "IMPL/LCRelationNavigatorImpl.h"

#include "EVENT/LCWgtRelation.h"

#include "EVENT/LCCollection.h"
#include "EVENT/LCParameters.h"
#include "EVENT/LCIO.h"

using namespace EVENT ;

namespace IMPL {


  LCRelationNavigatorImpl::LCRelationNavigatorImpl( const EVENT::LCCollection* col ) :
  
    LCRelationNavigatorBaseImpl( col->getParameters().getStringVal("FromType") , 
				 col->getParameters().getStringVal("ToType") ) { 

    initialize(col) ; 
  }

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
