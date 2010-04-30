package hep.lcio.implementation.event;

import java.util.ArrayList;
import java.util.List;

import hep.lcio.event.TrackerHit;

/**
 * @author Tony Johnson
 * @version $Id: ITrackerHit.java,v 1.10 2010-04-30 15:28:25 engels Exp $
 */
public class ITrackerHit extends ILCObject implements TrackerHit
{
   protected double[] position = new double[3];
   protected float[] covMatrix = new float[6];
   protected float dEdx;
   protected float time;
   protected float charge;
   protected float chargeError;
   protected int type;
   protected int quality;
   protected List rawHits = new ArrayList() ;
   
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
   public void setdEdx(float dEdx)
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
      
   public float getCharge()
   {
      return charge;
   }

   public void setCharge(float charge)
   {
      checkAccess();
      this.charge = charge;
   }

   public float getChargeError()
   {
      return chargeError;
   }

   public void setChargeError(float error)
   {
      checkAccess();
      this.chargeError = error;
   }

   public int getQuality()
   {
      return quality;
   }

   public void setQuality(int quality)
   {
      checkAccess();
      this.quality = quality;
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
   

   public List getRawHits() 
   {
     return rawHits ;
   }
   
   public void setRawHits(List rawHits)
   {
      checkAccess();
      this.rawHits = rawHits; 
   }

}
