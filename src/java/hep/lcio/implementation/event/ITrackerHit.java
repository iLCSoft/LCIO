package hep.lcio.implementation.event;

import hep.lcio.event.MCParticle;
import hep.lcio.event.TrackerHit;


/**
 * A default implementation of TrackerHit
 * @author Tony Johnson
 * @version $Id: ITrackerHit.java,v 1.2 2003-05-06 06:22:11 tonyj Exp $
 */
public class ITrackerHit extends ILCObject implements TrackerHit
{
   protected Object particle;
   protected double[] position = new double[3];
   protected float dEdx;
   protected float time;
   protected int cellID;

   public void setCellID(int cellID)
   {
      checkAccess();
      this.cellID = cellID;
   }

   public int getCellID()
   {
      return cellID;
   }

   public void setMCParticle(MCParticle mc)
   {
      checkAccess();
      particle = mc;
   }

   public MCParticle getMCParticle()
   {
      return (MCParticle) particle;
   }

   public void setPosition(double[] pos)
   {
      checkAccess();
      if (pos.length != 3)
         throw new IllegalArgumentException();
      position = pos;
   }

   public double[] getPosition()
   {
      return position;
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

   public float getdEdx()
   {
      return dEdx;
   }

   public void setdEdx(float dEdx)
   {
      checkAccess();
      this.dEdx = dEdx;
   }
}
