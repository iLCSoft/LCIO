package hep.lcio.implementation.event;

import hep.lcio.event.Track;
import java.util.Collections;
import java.util.List;

/**
 *
 * @author  Tony Johnson
 */
public class ITrack extends ILCObject implements Track
{
   protected float chi2;
   protected float[] covMatrix;
   protected float d0;
   protected float momentum;
   protected float phi;
   protected float[] referencePoint;
   protected float theta;
   protected int type;
   protected float dEdx;
   protected float dEdxError;
   protected float z0;
   
   public float getChi2()
   {
      return chi2;
   }
   
   public float[] getCovMatrix()
   {
      return covMatrix;
   }
   
   public float getD0()
   {
      return d0;
   }
   
   public float getMomentum()
   {
      return momentum;
   }
   
   public float getPhi()
   {
      return phi;
   }
   
   public float[] getReferencePoint()
   {
      return referencePoint;
   }
   
   public float getTheta()
   {
      return theta;
   }
   
   public List getTrackerHits()
   {
      // FixMe:
      return Collections.EMPTY_LIST;
   }
   
   public List getTracks()
   {
      //FixMe:
      return Collections.EMPTY_LIST;
   }
   
   public int getType()
   {
      return type;
   }
   
   public float getZ0()
   {
      return z0;
   }
   
   public float getdEdx()
   {
      return dEdx;
   }
   
   public float getdEdxError()
   {
      return dEdxError;
   }
   
   public void setChi2(float chi2)
   {
      this.chi2 = chi2;
   }

   public void setCovMatrix(float[] covMatrix)
   {
      if (referencePoint.length != 15) throw new IllegalArgumentException("referencePoint.lenhgth != 3");
      this.covMatrix = covMatrix;
   }
   
   public void setD0(float d0)
   {
      this.d0 = d0;
   }
   
   public void setDEdx(float dEdx)
   {
      this.dEdx = dEdx;
   }

   public void setDEdxError(float dEdxError)
   {
      this.dEdxError = dEdxError;
   }
   
   public void setMomentum(float momentum)
   {
      this.momentum = momentum;
   }

   public void setPhi(float phi)
   {
      this.phi = phi;
   }
   
   public void setReferencePoint(float[] referencePoint)
   {
      if (referencePoint.length != 3) throw new IllegalArgumentException("referencePoint.lenhgth != 3");
      this.referencePoint = referencePoint;
   }
   
   public void setTheta(float theta)
   {
      this.theta = theta;
   }
   
   public void setType(int type)
   {
      this.type = type;
   }
   
   public void setZ0(float z0)
   {
      this.z0 = z0;
   } 
}
