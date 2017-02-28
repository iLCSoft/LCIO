////////////////////////////////////////
// RTextension for LCIO tests
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"

#include "EVENT/LCIO.h"
#include "IMPL/MCParticleImpl.h" 
#include "IMPL/ReconstructedParticleImpl.h" 


#include <iostream>

using namespace std ;
using namespace lcio ;

// replace mytest with the name of your test
const static string testname="testRTextension";

//=============================================================================

struct SomeClass{
  // define a type for a simple int extension
  //  ( inside a class for testing ... )
  struct Index : LCIntExtension<Index> {} ;

  double someDouble ;
  int someInt ;
  bool someBool ;
};


struct UserStruct : LCOwnedExtension<UserStruct,SomeClass>  {} ;

//a simple float extension 
struct Mass : LCFloatExtension<Mass> {} ;


// a vector of strings (pointers) that are owned by the object that is extended 
struct ParticleIDs : LCOwnedExtensionVector< ParticleIDs,std::string >{} ;


// a many to many relationship between MCParticles and Reconstructed Particles
struct MCTruth2Rec : LCNToNRelation< MCTruth2Rec, MCParticle, ReconstructedParticle> {} ;


//=============================================================================

int main(int /*argc*/, char** /*argv*/ ){
  
  // this should be the first line in your test
  TEST MYTEST=TEST( testname, std::cout );
  
  
  MCParticle* mcp = new MCParticleImpl ;
  ReconstructedParticleImpl* rcp = new ReconstructedParticleImpl ;
  
  MYTEST.LOG( " test to add int, float and stringVec extension to MCParticle ..." );
  
  mcp->ext<SomeClass::Index>() = 42 ;
  
  MYTEST(  mcp->ext<SomeClass::Index>() ,  42, " ext<SomeClass::Index> == 42 " )  ;
  
  rcp->ext<SomeClass::Index>() = 42 ;
  
  MYTEST(  rcp->ext<SomeClass::Index>() ,  42, " ext<SomeClass::Index> == 42 " )  ;
  
  mcp->ext<Mass>() = 42.12345f ;
  
  MYTEST(  mcp->ext<Mass>() ,  42.12345f, " ext<Mass> == 42.12345f " )  ;
  
  
  mcp->ext<ParticleIDs>()->push_back(  new std::string("Pion") )  ;
  mcp->ext<ParticleIDs>()->push_back(  new std::string("Muon") )  ;
  mcp->ext<ParticleIDs>()->push_back(  new std::string("Tau") )  ;
  
  std::string pid1 = *(*mcp->ext<ParticleIDs>())[1] ;
  
  MYTEST(  pid1   , "Muon" , " *(*mcp->ext<ParticleIDs>())[1] == \"Muon\" " )  ;
 
  mcp->ext<SomeClass::Index>() = 0 ;
  MYTEST(  mcp->ext<SomeClass::Index>() ,  0, " ext<SomeClass::Index> == 0 " )  ;
  

  MYTEST.LOG( " test owned extension with user class ..." ) ;

  MYTEST(  mcp->ext<UserStruct>() ,  (void*) 0  , " ext<SomeClass> == 0 " )  ; // pointer is not yet initiallized
  
  mcp->ext<UserStruct>() = new SomeClass ; // add a new user object

  mcp->ext<UserStruct>()->someDouble = 3.141592 ;

  MYTEST(   mcp->ext<UserStruct>()->someDouble, 3.141592  , " mcp->ext<UserStruct>()->someDouble == 3.141592 " );


  MYTEST.LOG( " test relations ... " ) ;

  add_relation<MCTruth2Rec>( mcp , rcp ) ;
  add_relation<MCTruth2Rec>( mcp , rcp ) ;
  add_relation<MCTruth2Rec>( mcp , rcp ) ;

  MYTEST(   mcp->rel<MCTruth2Rec::to>()->size() , unsigned( 3 )   , " mcp->rel<MCTruth2Rec::to>()->size() == 3  " ) ;

  MYTEST(   rcp->rel<MCTruth2Rec::from>()->size() , unsigned( 3 )   , " rcp->rel<MCTruth2Rec::from>()->size() == 3  " ) ;


  for(  MCTruth2Rec::to::const_iterator it = mcp->rel<MCTruth2Rec::to>()->begin() ;
	it != mcp->rel<MCTruth2Rec::to>()->end() ; ++it ) {

    MYTEST(   *it ,  rcp    , " mcp->rel<MCTruth2Rec::to>() == rcp  " ) ;

  }

  for(  MCTruth2Rec::from::const_iterator it = rcp->rel<MCTruth2Rec::from>()->begin() ;
	it != rcp->rel<MCTruth2Rec::from>()->end() ; ++it ) {

    MYTEST(   *it ,  mcp    , " rcp->rel<MCTruth2Rec::from>() == mcp  " ) ;

  }


  //----------------------------------------------------------
  //if( true ){
  if( false ){
    // force test program to fail in this way:
    MYTEST.FAILED( "oops, something went wrong..." );
  }
  
  
  return 0;
}

//=============================================================================

