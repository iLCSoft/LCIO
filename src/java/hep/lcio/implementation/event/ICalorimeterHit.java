package hep.lcio.implementation.event;

import hep.lcio.event.CalorimeterHit;

/**
 * A default implementation of CalorimeterHit
 * @author Tony Johnson
 * @version $Id: ICalorimeterHit.java,v 1.6 2004-04-08 09:57:59 gaede Exp $
 */
public class ICalorimeterHit extends ILCObject implements CalorimeterHit
{
   protected float[] position = new float[3];
   protected float energy;
   protected int cellId0;
   protected int cellId1;

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

   public void setPosition(float[] pos)
   {
      checkAccess();
      if (pos.length != 3)
         throw new IllegalArgumentException();
      position = pos;
   }

   public float[] getPosition()
   {
      return position;
   }
}
