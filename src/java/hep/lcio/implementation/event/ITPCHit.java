package hep.lcio.implementation.event;

import hep.lcio.event.TPCHit;


/**
 * A default implementation of TPCHit
 * @author Frank Gaede
 * @version $Id: ITPCHit.java,v 1.4 2004-09-24 13:21:22 tonyj Exp $
 */
public class ITPCHit extends ILCObject implements TPCHit
{
   protected int cellID;
   protected float time;
   protected float charge;
   protected int quality;
   protected int[] rawDataArray;
   
   public void setCellID(int cellID)
   {
      checkAccess();
      this.cellID = cellID;
   }
   
   public int getCellID()
   {
      return cellID;
   }
   public void setTime(float time)
   {
      checkAccess();
      this.time = time;
   }
   
   public float getTime()
   {
      return time;
   }
   public void setCharge(float charge)
   {
      checkAccess();
      this.charge = charge;
   }
   
   public float getCharge()
   {
      return charge;
   }
   
   public void setQuality(int quality)
   {
      checkAccess();
      this.quality= quality;
   }
   
   public int getQuality()
   {
      return quality;
   }
   
   public int getNRawDataWords()
   {
      return rawDataArray == null ? 0 : rawDataArray.length ;
   }
   
   public void setRawDataWords(int rawData[])
   {
      checkAccess();
      this.rawDataArray = rawData ;
   }
   public int getRawDataWord(int i)
   {
      return rawDataArray[ i ] ;
   }
   
}
