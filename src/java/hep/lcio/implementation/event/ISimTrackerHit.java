package hep.lcio.implementation.event;

import hep.lcio.data.MCParticleData;

import hep.lcio.event.MCParticle;
import hep.lcio.event.SimTrackerHit;


/**
 * A default implementation of SimTrackerHit
 * @author Tony Johnson
 * @version $Id: ISimTrackerHit.java,v 1.6 2003-09-10 18:19:46 gaede Exp $
 */
public class ISimTrackerHit extends ILCObject implements SimTrackerHit
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

   public MCParticleData getMCParticleData()
   {
      return getMCParticle();
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
