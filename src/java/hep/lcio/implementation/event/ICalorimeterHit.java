package hep.lcio.implementation.event;

import hep.lcio.event.CalorimeterHit;
import hep.lcio.event.LCObject;

/**
 * A default implementation of CalorimeterHit
 * @author Tony Johnson
 * @version $Id: ICalorimeterHit.java,v 1.10 2008-06-04 17:37:16 engels Exp $
 */
public class ICalorimeterHit extends ILCObject implements CalorimeterHit
{
   protected float[] position = new float[3];
   protected float time;
   protected float energy;
   protected float energyError;
   protected int cellId0;
   protected int cellId1;
   protected int type;
   protected LCObject rawHit;
   
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
   
   public void setEnergy(float energy)
   {
      checkAccess();
      this.energy = energy;
   }
   
   public float getEnergy()
   {
      return energy;
   }

   public float getEnergyError()
   {
      return energyError;
   }
   
   public void setPosition(float[] pos)
   {
      if (pos.length != 3) throw new IllegalArgumentException();
      checkAccess();
      position = pos;
   }
   
   public float[] getPosition()
   {
      return position;
   }

   public float getTime()
   {
      return time;
   }
   
   public void setTime(float f)
   {
      checkAccess();
      time = f;
   }
   
   public LCObject getRawHit()
   {
      return rawHit;
   }
   
   public int getType()
   {
      return type;
   }
   
   public void setRawHit(LCObject object)
   {
      checkAccess();
      rawHit = object;
   }
   
   public void setType(int i)
   {
      checkAccess();
      type = i;
   }
   
}
