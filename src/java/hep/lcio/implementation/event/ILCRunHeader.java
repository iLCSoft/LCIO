package hep.lcio.implementation.event;

import hep.lcio.event.LCRunHeader;

import java.util.Arrays;
import java.util.List;


/**
 * A default implementation of LCRunHeader
 * @author Tony Johnson
 * @version $Id: ILCRunHeader.java,v 1.2 2003-05-06 06:22:11 tonyj Exp $
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
      checkAccess();

      int l = activeSubdetectors.length;
      String[] newActive = new String[l + 1];
      System.arraycopy(activeSubdetectors, 0, newActive, 0, l);
      newActive[l] = det;
      activeSubdetectors = newActive;
   }
}
