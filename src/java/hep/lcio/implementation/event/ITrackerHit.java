package hep.lcio.implementation.event;

import hep.lcio.event.LCObject;
import hep.lcio.event.TrackerHit;

/**
 * @author Tony Johnson
 * @version $Id: ITrackerHit.java,v 1.6 2004-09-13 18:02:57 tonyj Exp $
 */
public class ITrackerHit extends ILCObject implements TrackerHit
{
   protected double[] position = new double[3];
   protected float[] covMatrix = new float[6];
   protected float dEdx;
   protected float time;
   protected int type;
   protected Object rawDataHit;
   
   public double[] getPosition()
   {
      return position;
   }
   
   public void setPosition(double[] pos)
   {
      if (pos.length != 3) throw new IllegalArgumentException();
      checkAccess();
      this.position = pos;
   }
   
   public float[] getCovMatrix()
   {
      return covMatrix;
   }
   
   public void setCovMatrix(float[] matrix)
   {
      if (matrix.length != 6) throw new IllegalArgumentException();
      checkAccess();
      this.covMatrix = matrix;
   }
   
   public float getdEdx()
   {
      return dEdx;
   }
   public void setdEdx()
   {
      checkAccess();
      this.dEdx = dEdx;
   }
   
   public float getTime()
   {
      return time;
   }
   
   public void setTime(float time)
   {
      checkAccess();
      this.time = time;
   }
      
   public int getType()
   {
      return type;
   }
   
   public void setType(int type)
   {
      checkAccess();
      this.type = type;
   }
   
   public LCObject getRawDataHit()
   {
      return (LCObject) rawDataHit;
   }
   
   public void setRawDataHit(LCObject hit)
   {
      checkAccess();
      this.rawDataHit = hit;
   }
}
