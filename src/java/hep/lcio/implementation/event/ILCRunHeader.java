package hep.lcio.implementation.event;

import hep.lcio.event.LCRunHeader;


/**
 * A default implementation of LCRunHeader
 * @author Tony Johnson
 * @version $Id: ILCRunHeader.java,v 1.4 2004-04-08 09:57:59 gaede Exp $
 */
public class ILCRunHeader extends ILCObject implements LCRunHeader
{
   protected String description;
   protected String detectorName;
   protected String[] activeSubdetectors = new String[0];
   protected int runNumber;

   public String[] getActiveSubdetectors()
   {
      return activeSubdetectors;
   }

   public void setDescription(String desc)
   {
      checkAccess();
      this.description = desc;
   }

   public String getDescription()
   {
      return description;
   }

   public void setDetectorName(String name)
   {
      checkAccess();
      this.detectorName = name;
   }

   public String getDetectorName()
   {
      return detectorName;
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

   public void addActiveSubdetector(String det)
   {
       //      checkAccess();
       // allow to add active subdetectors

      int l = activeSubdetectors.length;
      String[] newActive = new String[l + 1];
      System.arraycopy(activeSubdetectors, 0, newActive, 0, l);
      newActive[l] = det;
      activeSubdetectors = newActive;
   }
}
