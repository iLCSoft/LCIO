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
   protected int cellID;
   protected float[] momentum = new float[3] ;
   protected float pathLength;
   
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

   
}
