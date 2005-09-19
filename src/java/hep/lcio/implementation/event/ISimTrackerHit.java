package hep.lcio.implementation.event;

import hep.lcio.event.MCParticle;
import hep.lcio.event.SimTrackerHit;


/**
 * A default implementation of SimTrackerHit
 * @author Tony Johnson
 * @version $Id: ISimTrackerHit.java,v 1.8 2005-09-19 15:40:28 gaede Exp $
 */
public class ISimTrackerHit extends ILCObject implements SimTrackerHit
{
   protected Object particle;
   protected double[] position = new double[3];
   protected float dEdx;
   protected float time;
   protected int cellID;
   protected float[] momentum = new float[3] ;

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

public float[] getMomentum() {
	return momentum;
}

public void setMomentum(float[] fs) {
	checkAccess() ;
	if (fs.length != 3)
		throw new IllegalArgumentException();
 	momentum = fs;
}

}
