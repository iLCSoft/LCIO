package hep.lcio.implementation.event;

import java.util.ArrayList;
import java.util.List;

import hep.lcio.event.TrackerHitPlane;

/**
 * @author Tony Johnson
 * @version $Id: ITrackerHitPlane.java,v 1.12 2010-06-02 10:59:34 engels Exp $
 */
public class ITrackerHitPlane extends ILCObject implements TrackerHitPlane
{
   protected int cellID0;
   protected int cellID1;
   protected double[] position = new double[3];
   protected float[] covMatrix = new float[6];
   //protected float dEdx; // DEPRECATED. renamed to EDep
   protected float[] u = new float[2];
   protected float[] v = new float[2];
   protected float dU;
   protected float dV;
   protected float EDep;
   protected float EDepError;
   protected float time;
   protected int type;
   protected int quality;
   protected List rawHits = new ArrayList() ;
   

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
   
   public float[] getU()
   {
      return u;
   }
   
   public void setU(float[] u)
   {
      if (u.length != 2) throw new IllegalArgumentException();
      checkAccess();
      this.u = u;
   }
   public float[] getV()
   {
      return v;
   }
   
   public void setV(float[] v)
   {
      if (v.length != 2) throw new IllegalArgumentException();
      checkAccess();
      this.v = v;
   }
 
   public float[] getCovMatrix()
   {
      return covMatrix;
   }
   
   //public void setCovMatrix(float[] matrix)
   //{
   //   if (matrix.length != 6) throw new IllegalArgumentException();
   //   checkAccess();
   //   this.covMatrix = matrix;
   //}
   
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
   
   public float getdU()
   {
      return dU;
   }

   public void setdU(float dU)
   {
      checkAccess();
      this.dU = dU;
   }

   public float getdV()
   {
      return dV;
   }

   public void setdV(float dV)
   {
      checkAccess();
      this.dV = dV;
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

   public float getEDepError()
   {
      return EDepError;
   }

   public void setEDepError(float e)
   {
      checkAccess();
      this.EDepError = e;
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
