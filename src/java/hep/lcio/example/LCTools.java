/*
 * LCTools.java
 *
 * Created on May 4, 2003, 9:48 PM
 */

package hep.lcio.example;

import hep.lcio.event.CalorimeterHit;
import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCFloatVec;
import hep.lcio.event.LCIO;
import hep.lcio.event.MCParticle;
import hep.lcio.event.TrackerHit;

/**
 *
 * @author  Tony Johnson
 */
public class LCTools {
    
    public static void dumpEvent(LCEvent evt)
    {    
     // the event:
     System.out.println("event  : " 
	 + evt.getEventNumber() 
	 + " - run " + evt.getRunNumber()
	 + " detector : "  +  evt.getDetectorName()
	 + " - collections  : ");
    
     String[] strVec = evt.getCollectionNames() ;

     // loop over collections:
     for( int j=0; j<strVec.length; j++){
         String name = strVec[j];
    
      System.out.print( "     " +  name +   " " 
	   +   evt.getCollection( name ).getTypeName() + " : "  ); 
    
      LCCollection col = evt.getCollection( name ) ;
    
      // print CalorimeterHit collections:
      if(evt.getCollection( name ).getTypeName().equals( LCIO.CALORIMETERHIT) ){
      
	int nHits =  col.getNumberOfElements() ;
	System.out.print(nHits + " hits - first hit: " );
	int nPrint = nHits>0 ? 1 : 0;

	if(nPrint == 0 ) System.out.println() ;
	for( int i=0 ; i< nPrint ; i++ ){
	
	  CalorimeterHit hit = 
	    (CalorimeterHit) col.getElementAt( i );
	
	  float[] x =  hit.getPosition() ;
	  System.out.println( "    hit -  e: " 
	       + hit.getEnergy() + "  pos: " 
	       + x[0] + ", " + x[1] + ", " + x[2] 
	       );   
	}
      
	// print the MCParticle collection
      } else if(evt.getCollection( name ).getTypeName().equals( LCIO.TRACKERHIT) ){
      
	int nHits =  col.getNumberOfElements() ;
	System.out.print(nHits + " hits : ") ;
	int nPrint = nHits>0 ? 1 : 0 ;

	if(nPrint==0) System.out.println();
	for( int i=0 ; i< nPrint ; i++ ){
	  TrackerHit hit = (TrackerHit) col.getElementAt( i )  ;
	
	  double[] x =  hit.getPosition() ;
	  System.out.println( "    hit -  dEdx: " 
	       + hit.getdEdx() 
	       + "  mc: " + hit.getMCParticle().getPDG() 
	       + "  pos: " 
	       + x[0] + ", " + x[1] + ", " + x[2] 
	       );   

	}
      } else if(evt.getCollection( name ).getTypeName().equals(LCIO.LCFLOATVEC) ){
	
	int nHits =  col.getNumberOfElements() ;
	System.out.print( nHits + " vectors: " );
	int nPrint = nHits>0 ? 1 : 0 ;
	
	if(nPrint == 0) System.out.println();
	for( int i=0 ; i< nPrint ; i++ ){
	  LCFloatVec vec = (LCFloatVec) col.getElementAt( i )  ;
	  
	  System.out.print(" values(" + i + "): ") ;
// FIXME:
//	  for(int k=0 ; k< vec.size() ; k++ )
//	    System.out.print( (*vec)[k]  + ", ") ;
	  System.out.println() ;   

	}
      } else if(evt.getCollection( name ).getTypeName().equals( LCIO.MCPARTICLE) ){
      
      
	int nHits =  col.getNumberOfElements() ;
	System.out.print( nHits + " particles : ") ;
	int nPrint = nHits>0 ? 1 : 0 ;

	if(nPrint == 0) System.out.println();
	for( int i=0 ; i< nPrint ; i++ ){
	  MCParticle part = (MCParticle) col.getElementAt( i )  ;
	
	  System.out.println( "           " + part.getPDG() + " p: " 
	       +  part.getMomentum()[0]  + ", "
	       +  part.getMomentum()[1]  + ", "
	       +  part.getMomentum()[2]  
	       );	
	
	}

      } else {
	System.out.println() ;
      } 
    }
    System.out.println() ;
  }
    
}