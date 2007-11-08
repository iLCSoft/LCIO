package hep.lcio.implementation.event;

import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCParameters;
import hep.lcio.exceptions.DataNotAvailableException;
import hep.lcio.exceptions.EventException;

import java.util.LinkedHashMap;
import java.util.Map;


/**
 * A default implementation of LCEvent
 * 
 * @author Tony Johnson
 * @version $Id: ILCEvent.java,v 1.14 2007-11-08 19:35:37 gaede Exp $
 */
public class ILCEvent extends ILCObject implements LCEvent
{
   protected String detectorName;
   protected int eventNumber;
   protected int runNumber;
   protected long timeStamp;
   private Map collections = new LinkedHashMap();
   protected ILCParameters parameters = new ILCParameters() ;
   
   public LCCollection getCollection(String name) throws DataNotAvailableException
   {
      LCCollection col = (LCCollection) collections.get(name);
      if( col == null )
         throw new DataNotAvailableException() ;
      return col ;
   }
   
   public LCCollection getCollectionData(String name)
   {
      return (LCCollection) collections.get(name);
   }
   
   public String[] getCollectionNames()
   {
      String[] keys = new String[collections.size()];
      collections.keySet().toArray(keys);
      return keys;
   }
   
   public void setDetectorName(String detectorName)
   {
      checkAccess();
      this.detectorName = detectorName;
   }
   
   public String getDetectorName()
   {
      return detectorName;
   }
   
   public void setEventNumber(int event)
   {
      checkAccess();
      this.eventNumber = event;
   }
   
   public int getEventNumber()
   {
      return eventNumber;
   }
   
   public void setRunNumber(int run)
   {
      checkAccess();
      this.runNumber = run;
   }
   
   public int getRunNumber()
   {
      return runNumber;
   }
   
   public void setTimeStamp(long timeStamp)
   {
      checkAccess();
      this.timeStamp = timeStamp;
   }
   
   public long getTimeStamp()
   {
      return timeStamp;
   }
   
   public double getWeight() {
     double w = (double) parameters.getFloatVal("_weight") ;
     return w == 0 ? 1. : w ; 
   }

   public void  setWeight( double w) {
     checkAccess();
     parameters.setValue("_weight", (float) w ) ;
   }

   public void addCollection(LCCollection col, String name)
   {
       if( !isValidCollectionName(name))
    	   throw new EventException("Invalid collection name: "+name) ;
	   collections.put(name, col);
   }
   
   public void removeCollection(String name)
   {
      checkAccess();
      collections.remove(name);
   }
   
   public LCParameters getParameters()
   {
      return parameters;
   }
   

   protected boolean isAlpha(char c) {
	   return ( (c>='a'&&c<='z') || (c>='A'&&c<='Z') ) ;
   }	
   protected boolean isNum(char c) {
	   return (c>='0' && c<='9') ;
   }	
   protected boolean isValidCollectionName(String name){

      int len = name.length() ;
	  if(len==0)
	     return false ;
	  char c0 = name.charAt(0) ;

      if( !isAlpha( c0 ) && c0 != '_' ) 
         return false ;
	      
	  for( int i=1; i< len; i++ ){
	     char c = name.charAt(i) ;
	     if( isAlpha(c) || isNum(c) || c == '_'   ) 
	        continue ;
	     else
	        return false ;
	  } 
	  return true ;
   }
}
