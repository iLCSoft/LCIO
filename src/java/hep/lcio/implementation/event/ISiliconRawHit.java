package hep.lcio.implementation.event;

import hep.lcio.event.SiliconRawHit;

/**
 *
 * @author tonyj
 * @version $Id: ISiliconRawHit.java,v 1.1 2005-05-10 01:12:55 tonyj Exp $
 */
public class ISiliconRawHit extends ILCObject implements SiliconRawHit
{
   protected int adcCount;
   protected int cellID0;
   protected int cellID1;
   protected int timeStamp;

   public int getADCCounts()
   {
      return adcCount;
   }

   public int getCellID0()
   {
      return cellID0;
   }

   public int getCellID1()
   {
      return cellID1;
   }

   public int getTimeStamp()
   {
      return timeStamp;
   }

   public void setADCCounts(int adcCount)
   {
      checkAccess();
      this.adcCount = adcCount;
   }

   public void setCellID0(int cellID0)
   {
      checkAccess();
      this.cellID0 = cellID0;
   }

   public void setCellID1(int cellID1)
   {
      checkAccess();
      this.cellID1 = cellID1;
   }

   public void setTimeStamp(int timeStamp)
   {
      checkAccess();
      this.timeStamp = timeStamp;
   }
}
