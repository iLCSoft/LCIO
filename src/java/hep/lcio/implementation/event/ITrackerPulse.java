package hep.lcio.implementation.event;

import hep.lcio.event.TrackerData;
import hep.lcio.event.TrackerPulse;

/**
 *
 * @author tonyj
 * @version $Id: ITrackerPulse.java,v 1.4 2010-05-12 14:50:01 engels Exp $
 */
public class ITrackerPulse extends ILCObject implements TrackerPulse
{
   protected int cellID0;
   protected int cellID1;
   protected float charge;
   //protected float chargeError;
   protected int quality;
   protected TrackerData correctedData;
   protected float time;
   //protected float timeError;
   protected static int covMatrixSize=3;
   protected float[] covMatrix = new float[covMatrixSize];
   
 
   
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
   public float getCharge()
   {
      return charge;
   }
   //public float getChargeError()
   //{
   //   return chargeError;
   //}
   
   public int getQuality()
   {
      return quality;
   }
   
   public TrackerData getTrackerData()
   {
      return correctedData;
   }
   
   public float getTime()
   {
      return time;
   }
   //public float getTimeError()
   //{
   //   return timeError;
   //}
      
   public float[] getCovMatrix()
   {
      return covMatrix;
   }

   public void setCovMatrix(float[] matrix)
   {
      if (matrix.length != covMatrixSize) throw new IllegalArgumentException();
      checkAccess();
      this.covMatrix = matrix;
   }

   public void setCharge(float charge)
   {
      checkAccess();
      this.charge = charge;
   }
   //public void setChargeError(float chargeError)
   //{
   //   checkAccess();
   //   this.chargeError = chargeError;
   //}
   
   public void setQuality(int quality)
   {
      checkAccess();
      this.quality = quality;
   }
   
   
   public void setTrackerData(TrackerData correctedData)
   {
      checkAccess();
      this.correctedData = correctedData;
   }
   
   public void setTime(float time)
   {
      checkAccess();
      this.time = time;
   }
   //public void setTimeError(float timeError)
   //{
   //   checkAccess();
   //   this.timeError = timeError;
   //}
}
