package hep.lcio.implementation.event;

import hep.lcio.data.LCCollectionData;

import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;

import java.util.LinkedHashMap;
import java.util.Map;


/**
 * A default implementation of LCEvent
 * @author Tony Johnson
 * @version $Id: ILCEvent.java,v 1.6 2003-09-04 04:27:00 tonyj Exp $
 */
public class ILCEvent extends ILCObject implements LCEvent
{
   protected String detectorName;
   protected int eventNumber;
   protected int runNumber;
   protected long timeStamp;
   private Map collections = new LinkedHashMap();

   public LCCollection getCollection(String name)
   {
      return (LCCollection) collections.get(name);
   }

   public LCCollectionData getCollectionData(String name)
   {
      return (LCCollectionData) collections.get(name);
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

   public void addCollection(LCCollection col, String name)
   {
      collections.put(name, col);
   }

   public void removeCollection(String name)
   {
      collections.remove(name);
   }
}
