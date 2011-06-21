package hep.lcio.implementation.event;

import hep.lcio.event.TrackState;

/**
 *
 * @author Tony Johnson, Jan Engels
 */
public class ITrackState extends ILCObject implements TrackState
{
   private static final float[] null3 = new float[3];
   private static final float[] null15 = new float[15];
   
   protected float[] covMatrix = null15;
   protected int location;
   protected float d0;
   protected float omega;
   protected float phi;
   protected float[] referencePoint = null3;
   protected float tanLambda;
   protected float z0;
   
   public float[] getCovMatrix()
   {
      return covMatrix;
   }
   
   public int getLocation()
   {
      return location;
   }

   public float getD0()
   {
      return d0;
   }
   
   public float getPhi()
   {
      return phi;
   }
   
   public float[] getReferencePoint()
   {
      return referencePoint;
   }
   
   public float getZ0()
   {
      return z0;
   }
   
   public float getOmega()
   {
      return omega;
   }
   
   public float getTanLambda()
   {
      return tanLambda;
   }
 
   public void setCovMatrix(float[] covMatrix)
   {
      checkAccess();
      if (covMatrix.length != 15) throw new IllegalArgumentException("covMatrix.length != 15");
      this.covMatrix = covMatrix;
   }
   
   public void setLocation(int location)
   {
      checkAccess();
      //FIXME check for valid location
      this.location = location;
   }
    public void setD0(float d0)
   {
      checkAccess();
      this.d0 = d0;
   }
   
   public void setPhi(float phi)
   {
      checkAccess();
      this.phi = phi;
   }
   
   public void setReferencePoint(float[] referencePoint)
   {
      checkAccess();
      if (referencePoint.length != 3) throw new IllegalArgumentException("referencePoint.lenhgth != 3");
      this.referencePoint = referencePoint;
   }
   
   public void setZ0(float z0)
   {
      checkAccess();
      this.z0 = z0;
   }
  
   public void setOmega(float f)
   {
      checkAccess();
      omega = f;
   }
   
   public void setTanLambda(float f)
   {
      checkAccess();
      tanLambda = f;
   }

}

