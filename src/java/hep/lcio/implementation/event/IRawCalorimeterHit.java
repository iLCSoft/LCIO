package hep.lcio.implementation.event;

import hep.lcio.event.RawCalorimeterHit;
import hep.lcio.event.LCObject;

/**
 * A default implementation of CalorimeterHit
 * @author Tony Johnson
 * @version $Id: IRawCalorimeterHit.java,v 1.1 2004-09-24 10:39:29 tonyj Exp $
 */
public class IRawCalorimeterHit extends ILCObject implements RawCalorimeterHit
{
   protected int cellId0;
   protected int cellId1;
   protected int timeStamp;
   protected int amplitude;
   
   public void setCellID0(int cellID)
   {
      checkAccess();
      this.cellId0 = cellID;
   }
   
   public int getCellID0()
   {
      return cellId0;
   }
   
   public void setCellID1(int cellID)
   {
      checkAccess();
      this.cellId1 = cellID;
   }
   
   public int getCellID1()
   {
      return cellId1;
   }
   
   public void setAmplitude(int amplitude)
   {
      checkAccess();
      this.amplitude = amplitude;
   }
   
   public int getAmplitude()
   {
      return amplitude;
   }
   
   public void setTimeStamp(int timeStamp)
   {
      checkAccess();
      this.timeStamp = timeStamp;
   }   
   
   public int getTimeStamp()
   {
      return timeStamp;
   }  
}
