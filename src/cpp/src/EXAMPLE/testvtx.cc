#include "lcio.h"

#include "EVENT/LCCollection.h"
#include "EVENT/ReconstructedParticle.h"
#include "EVENT/Vertex.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/ReconstructedParticleImpl.h"
#include "IMPL/VertexImpl.h"

#include "LCIOSTLTypes.h"
#include "UTIL/LCTOOLS.h"
#include "UTIL/Operators.h"

using namespace std ;
using namespace lcio ;


////////////////////////////////////////////////////////////////////////////////
// This file is temporary and ONLY for the purpose of testing the Vertex Class!!
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv ){

  LCCollectionVec* vertexVec = new LCCollectionVec( LCIO::VERTEX ) ;
  
  //gives a warnig...
  //cout<<header(&VertexImpl());
  
  for(int i=0; i < 10; i++){
    
      VertexImpl* vtx = new VertexImpl ;
      
      if( i==0 ){ vtx->setPrimary(true); }
      else{ vtx->setPrimary(false); }
      
      vtx->setChi2(1+i*.01);
      vtx->setProbability(0.0032+i*.01);
      vtx->setPosition(0.3453+i*.01,.45345354+i*.01,2.345354+i*.01);

      FloatVec cov(6) ;
      cov[0] = 1. ;
      cov[1] = 2. ;
      cov[2] = 3. ;
      cov[3] = 4. ;
      cov[4] = 5. ;
      cov[5] = 6. ;

      vtx->setCovMatrix( cov ) ;
      
      //Vertex* v=vtx;
      //cout<<lcshort(v);
      
      vertexVec->addElement ( vtx ) ;

    }
    
  //gives a warnig...
  //cout<<tail(&VertexImpl());
 
  cout<<&IMPL::VertexImpl()<<endl;
  
  LCTOOLS::printVertices( vertexVec  ) ;

  delete vertexVec ;

  return 0 ;
}

