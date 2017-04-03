package hep.lcio.implementation.event;

import hep.lcio.event.MCParticle;
import hep.lcio.event.SimTrackerHit;


/**
 * A default implementation of SimTrackerHit
 * @author Tony Johnson
 * @version $Id: ISimTrackerHit.java,v 1.11 2010-06-02 10:59:34 engels Exp $
 */
public class ISimTrackerHit extends ILCObject implements SimTrackerHit
{
   protected Object particle;
   protected double[] position = new double[3];
   //protected float dEdx; // DEPRECATED. renamed to EDep
   protected float EDep;
   protected float time;
   //protected int cellID; // DEPRECATED. renamed to cellID0
   protected int cellID0;
   protected int cellID1;
   protected float[] momentum = new float[3] ;
   protected float pathLength;
   protected int quality;

   // DEPRECATED. renamed to setcellID0
   public void setCellID(int cellID)
   {
      //checkAccess();
      //this.cellID = cellID;
      setCellID0( cellID );
   }
   
   // DEPRECATED. renamed to getcellID0
   public int getCellID()
   {
      //return cellID;
      return getCellID0();
   }
   
   public void setCellID0(int cellID)
   {
      checkAccess();
      this.cellID0 = cellID;
   }
   
   public int getCellID0()
   {
      return cellID0;
   }
    public void setCellID1(int cellID)
   {
      checkAccess();
      this.cellID1 = cellID;
   }
   
   public int getCellID1()
   {
      return cellID1;
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
   
   // DEPRECATED. renamed to getEDep()
   public float getdEdx()
   {
      //return dEdx;
      return getEDep();
   }
   
   // DEPRECATED. renamed to setEDep()
   public void setdEdx(float dEdx)
   {
      //checkAccess();
      //this.dEdx = dEdx;
      setEDep( dEdx );
   }
   
   public float getEDep()
   {
      return EDep;
   }

   public void setEDep(float e)
   {
      checkAccess();
      this.EDep = e;
   }

    public float[] getMomentum()
   {
      return momentum;
   }
   
   public void setMomentum(float[] fs)
   {
      checkAccess() ;
      if (fs.length != 3) throw new IllegalArgumentException();
      momentum = fs;
   }
   
   public float getPathLength()
   {
      return pathLength;
   }
   
   public void setPathLength(float length)
   {
      this.pathLength = length;
   }


   public int getQuality()
   {
      return quality;
   }

   public void setQuality(int quality)
   {
      this.quality = quality;
   }

    public boolean isProducedBySecondary()
    {
	return bitTest(this.quality, BITProducedBySecondary);
    }

    public boolean isOverlay()
    {
	return bitTest(this.quality, BITOverlay);
    }

    public void setProducedBySecondary(boolean val)
    {
	quality = bitSet(quality, BITProducedBySecondary, val);
    }

    public void setOverlay(boolean val)
    {
	quality = bitSet(quality, BITOverlay, val);
    }

}
