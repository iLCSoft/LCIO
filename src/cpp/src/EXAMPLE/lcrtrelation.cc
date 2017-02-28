#include "lcio.h"

#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCCollection.h" 
#include "EVENT/Track.h" 
#include "EVENT/Cluster.h" 

using namespace std ;
using namespace lcio ;



// ------  define some structs for extensions and relations:-----------

// NB: the first template argument of the parent class has to be the class itself !

// a simple int extension 
struct Index : LCIntExtension<Index> {} ;

// a simple float extension 
struct Mass : LCFloatExtension<Mass> {} ;

// a vector of strings (pointers) that are owned by the object that is extended 
struct ParticleIDs : LCOwnedExtensionVector<ParticleIDs,std::string> {};


// note extensions can be attched to any LCObject and be of any type, i.e. in particular
// of user defined types:
struct UserClass{
  int someInt ;
  float aFloat ;
};
struct MyUserExtension : LCOwnedExtension<MyUserExtension,UserClass> {} ;


// relations have to specify the type of the objects from and to which they relate: 

// example:   a one to many relationship between tracks and clusters:
struct TrkCluLink : LC1ToNRelation<TrkCluLink,Track,Cluster> {} ;


// a many to many relationship between MCParticles
struct ParentDaughter : LCNToNRelation<ParentDaughter,MCParticle,MCParticle> {} ;


//-----------------------------------------------------------------------------------------

/** Example/test program for new runtime extensions and relations
 *  requires recjob.slcio (from simjob/recjob).
 */

int main(int /*argc*/, char** /*argv*/ ){


  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;

  lcReader->open( "recjob.slcio" ) ;

  LCEvent* evt ;
  int nEvents = 0 ;
  

  //----------- the event loop -----------
  while( (evt = lcReader->readNextEvent()) != 0 && nEvents < 1 ) {
    

    LCCollection* mcpcol = evt->getCollection("MCParticle" ) ;
    
    int nmcp = mcpcol->getNumberOfElements() ;

    for(int i=0 ; i< nmcp  ; i++ ){

      MCParticle*  mcp = dynamic_cast<MCParticle*>( mcpcol->getElementAt(i) ) ;
	

      // ints can be assigned directly - w/o pointer !
      // make every particle know it's index in the collection 
      mcp->ext<Index>() = i ;   

      mcp->ext<Mass>() = mcp->getMass() ;

      // assign a user object to each particle:

      mcp->ext<MyUserExtension>() = new UserClass ;

      mcp->ext<MyUserExtension>()->someInt = i*42 ;
      mcp->ext<MyUserExtension>()->aFloat = i*3.1415 ;

      // assign some pid strings - note ownership is taken by particle 

      if( ! (i % 2) )

	mcp->ext<ParticleIDs>()->push_back( new std::string("charged")  )  ;

      else

	mcp->ext<ParticleIDs>()->push_back( new std::string("neutral")  )  ;
      
      if( ! (i % 3) )

	mcp->ext<ParticleIDs>()->push_back( new std::string("hadron")  )  ;

      else if( ! ((i+1) % 3) )

	mcp->ext<ParticleIDs>()->push_back( new std::string("photon")  )  ;

      else if( ! ((i+2) % 3) )

	mcp->ext<ParticleIDs>()->push_back( new std::string("electron")  )  ;


      // copy the parent - daughter relationship:

      const MCParticleVec& daughters = mcp->getDaughters() ;

      for(unsigned j=0 ; j< daughters.size()   ; j++ ){

	add_relation<ParentDaughter>( mcp, daughters[j] ) ;
      }
      
      
    }
    

    LCCollection* trkcol = evt->getCollection("SomeTracks" ) ;
    LCCollection* clucol = evt->getCollection("SomeClusters" ) ;

    if( trkcol && clucol ) {

      int nclu = clucol->getNumberOfElements() ;
      int ntrk = trkcol->getNumberOfElements() ;


      for(int j=0 ; j< ntrk  ; j++ ){
	
	Track*   trk = dynamic_cast<Track*>   ( trkcol->getElementAt(j) ) ;
	
	// ints can be assigned directly - w/o pointer !
	// make every track know it's index in the collection 
	trk->ext<Index>() = j ;   
	
	for(int k=0 ; k< nclu ; k++ ){
	  
	  Cluster* clu = dynamic_cast<Cluster*> ( clucol->getElementAt(k) ) ;
	  
	  // make every cluster know it's index in the collection 
 	  if( j == 0 )
	    clu->ext<Index>() = k ;
	  
	  if( j % 2  && ( k == j || k == j-1 ) )  

	    add_relation<TrkCluLink>( trk ,clu );
	}
      }
      
      
      // --- now print the relations:

      std::cout << "   ---- tracks assigned to clusters: " << std::endl ;
      for(int j=0 ; j< ntrk ; j++ ){


	Track*   trk = dynamic_cast<Track*>   ( trkcol->getElementAt(j) ) ;

	std::cout << " track " << trk->ext<Index>()  << " assigned to clusters :   " ; 

	TrkCluLink::to::rel_type clulist =  trk->rel<TrkCluLink::to>() ;

	for( TrkCluLink::to::const_iterator iclu = clulist->begin() ;
	     iclu != clulist->end() ; ++iclu ){
	  
	  Cluster* clu = *iclu ; // iterator is of type pointer to container element

	  std::cout <<  clu->ext<Index>() << ", " ; 
	}
	std::cout << std::endl ; 
      }

      std::cout << "   ----- now the inverse relation : " << std::endl ;

      for(int k=0 ; k< nclu ; k++ ){
	  
	Cluster* clu = dynamic_cast<Cluster*> ( clucol->getElementAt(k) ) ;


	Track* trk =  clu->rel<TrkCluLink::from>() ;
	
	std::cout << " cluster " 
		  << clu->ext<Index>() << " assigned from track:   " ;
	if( trk != 0 )
	  std::cout << trk->ext<Index>() << std::endl ; 
	else
	  std::cout << " none "  << std::endl ; 
      }

      std::cout << std::endl ; 


      // print MCParticle extensions and relations:

      std::cout << "   ----- MCParticles in event : : " << std::endl ;
      
      nmcp = ( nmcp < 10 ? nmcp : 10 ) ;

      MCParticle*  mcp0 = 0 ;   // pointer for first particle

      for(int i=0 ; i<nmcp  ; i++ ){
	
	MCParticle*  mcp = dynamic_cast<MCParticle*>( mcpcol->getElementAt(i) ) ;

	if( i == 0 ) mcp0 = mcp  ;
	
	ParticleIDs::ext_type pidv =  mcp->ext<ParticleIDs>() ;
	
	std::cout <<  "   --- particle " << mcp->ext<Index>()  << " found to be : " ;  
	
	for( ParticleIDs::const_iterator ipid = pidv->begin() ; ipid != pidv->end(); ++ipid){
	  
	  std::cout << **ipid << ", " ;
	}
	
	if( ( *(*pidv)[0] == "charged" && *(*pidv)[1] == "photon"  ) ||
	    ( *(*pidv)[0] == "neutral" && *(*pidv)[1] == "electron")   )

	  std::cout << " --- ooops ! " ;
	
	std::cout << std::endl ; 
	


	std::cout <<  "   --- particle " << mcp->ext<Index>()   <<  " user extension : " 
		  << " someInt: " << mcp->ext<MyUserExtension>()->someInt
		  << " aFloat:  " << mcp->ext<MyUserExtension>()->aFloat
		  << std::endl ;


	// now check that the runtime relation for daughters:

	const MCParticleVec& daughters = mcp->getDaughters() ;

	std::cout <<  "   --- particle " << mcp->ext<Index>()   <<  " daughters: " 
		  << std::endl ;  

	std::cout <<  "      --- from MCParticle:  " ; 

	for(MCParticleVec::const_iterator idau = daughters.begin() ; 
	    idau != daughters.end() ; ++idau){
	  
	  std::cout << (*idau)->ext<Index>() << ", " ; 
	}
	std::cout << std::endl ; 
	

	std::cout <<  "      --- from runtime rel: " ; 


	ParentDaughter::to::rel_type daulist =  mcp->rel<ParentDaughter::to>() ;
	
	for( ParentDaughter::to::const_iterator idau = daulist->begin(); 
	     idau != daulist->end(); ++idau){
	  
	  std::cout << (*idau)->ext<Index>() << ", " ;
	}
	std::cout << std::endl ; 
	

	std::cout <<  "   --- particle " << mcp->ext<Index>()   <<  " parents: " 
		  << std::endl ;  

	std::cout <<  "      --- from MCParticle:  " ; 


	const MCParticleVec& parents = mcp->getParents() ;

	for(MCParticleVec::const_iterator ipar = parents.begin() ; 
	    ipar != parents.end() ; ++ipar){
	  
	  std::cout << (*ipar)->ext<Index>() << ", " ; 
	}
	std::cout << std::endl ; 
	

	std::cout <<  "      --- from runtime rel: " ; 


	ParentDaughter::from::rel_type parlist =  mcp->rel<ParentDaughter::from>() ;
	
	for( ParentDaughter::from::const_iterator ipar = parlist->begin(); 
	     ipar != parlist->end(); ++ipar){
	  
	  std::cout << (*ipar)->ext<Index>() << ", " ;
	}
	std::cout << std::endl ; 


	// for demonstration we can rearange the daughter relationships:
	if(i>0) 

	  merge_relations<ParentDaughter>( mcp0 , mcp ) ;
      }
      

      for(int i=0 ; i<nmcp  ; i++ ){
	
	MCParticle*  mcp = dynamic_cast<MCParticle*>( mcpcol->getElementAt(i) ) ;

	std::cout <<  "   --- particle " << mcp->ext<Index>()  << " ( mass: " << mcp->ext<Mass>() << ") "
		  <<  " daughters after merging : " ;


	ParentDaughter::to::rel_type daulist =  mcp->rel<ParentDaughter::to>() ;
	
	for( ParentDaughter::to::const_iterator idau = daulist->begin(); 
	     idau != daulist->end(); ++idau){
	  
	  std::cout << (*idau)->ext<Index>() << ", " ;
	}
	std::cout << std::endl ; 

      }


    } else {
      std::cout << " couldn't find Track and Cluster collection in event !" << std::endl ;
    }
    
    nEvents ++ ;
  } 
  // -------- end of event loop -----------
  

  lcReader->close() ;
  delete lcReader ;
  return 0 ;
}

  
