package hep.lcio.implementation.event;
import hep.lcio.event.TrackerRawData;

/**
 *
 * @author tonyj
 * @version $Id: ITrackerRawData.java,v 1.1 2005-05-31 07:43:30 gaede Exp $
 */
public class ITrackerRawData extends ILCObject implements TrackerRawData
{  
   protected int cellID0;
   protected int cellID1;
   protected int time;
   protected short[] aDCValues = null ;
   

   /**
    * @return Returns the cellID0.
    */
   public int getCellID0() {
      return cellID0;
   }
   /**
    * @param cellID0 The cellID0 to set.
    */
   public void setCellID0(int cellID0) {
      this.cellID0 = cellID0;
   }
   /**
    * @return Returns the cellID1.
    */
   public int getCellID1() {
      return cellID1;
   }
   /**
    * @param cellID1 The cellID1 to set.
    */
   public void setCellID1(int cellID1) {
      this.cellID1 = cellID1;
   }
   
   
   public short[] getADCValues()
   {
      return aDCValues;
   }

   public int getTime()
   {
      return time;
   }


   public void setTime(int time0)
   {
      checkAccess();
      this.time = time0;
   }

   public void setADCValues(short[] charge)
   {
      checkAccess();
      this.aDCValues = charge;
   }
   
}
