package hep.lcio.implementation.event;
import hep.lcio.event.TrackerData;

/**
 *
 * @author tonyj
 * @version $Id: ITrackerData.java,v 1.1 2005-05-31 07:43:30 gaede Exp $
 */
public class ITrackerData extends ILCObject implements TrackerData
{  
   protected int cellID0;
   protected int cellID1;
   protected float time;
   protected float[] chargeValues = null ;
   

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


   public float[] getChargeValues()
   {
      return chargeValues;
   }

   public float getTime()
   {
      return time;
   }


   public void setTime(float time)
   {
      checkAccess();
      this.time = time;
   }

   /**
    * @param chargeValues The chargeValues to set.
    */
   public void setChargeValues(float[] chargeValues) {
      checkAccess();
      this.chargeValues = chargeValues;
   }
   
}
