package hep.lcio.implementation.event;

import hep.lcio.event.Track;
import hep.lcio.event.TrackerHit;

import java.util.ArrayList;
import java.util.Collections;

import java.util.List;

/**
 *
 * @author Tony Johnson
 */
public class ITrack extends ILCObject implements Track
{
   private static final int[] null0 = new int[0];
   private static final float[] null3 = new float[3];
   private static final float[] null15 = new float[15];
   
   protected float chi2;
   protected float[] covMatrix = null15;
   protected float d0;
   protected float omega;
   protected float phi;
   protected float[] referencePoint = null3;
   protected float tanLambda;
   protected int type;
   protected float dEdx;
   protected float dEdxError;
   protected float z0;
   protected int ndf;
   protected List tracks = new ArrayList();
   protected List hits = new ArrayList();
   protected float radiusOfInnermostHit;
   
   public final static int BITISREFERENCEPOINTPCA = 31;
   protected int[] subdetectorHitNumbers = null0;
   
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
   
   public float getPhi()
   {
      return phi;
   }
   
   public float[] getReferencePoint()
   {
      return referencePoint;
   }
   
   public List getTrackerHits()
   {
      return hits == null ? Collections.EMPTY_LIST : hits;
   }
   
   public List getTracks()
   {
      return tracks == null ? Collections.EMPTY_LIST : tracks;
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
      checkAccess();
      this.chi2 = chi2;
   }
   
   public void setCovMatrix(float[] covMatrix)
   {
      checkAccess();
      if (covMatrix.length != 15) throw new IllegalArgumentException("covMatrix.length != 15");
      this.covMatrix = covMatrix;
   }
   
   public void setD0(float d0)
   {
      checkAccess();
      this.d0 = d0;
   }
   
   public void setDEdx(float dEdx)
   {
      checkAccess();
      this.dEdx = dEdx;
   }
   
   public void setDEdxError(float dEdxError)
   {
      checkAccess();
      this.dEdxError = dEdxError;
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
   
   protected void setType(int type)
   {
      checkAccess();
      this.type = type;
   }
   
   public void setZ0(float z0)
   {
      checkAccess();
      this.z0 = z0;
   }
   public float getOmega()
   {
      return omega;
   }
   
   public float getTanLambda()
   {
      return tanLambda;
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
   
   public int getNdf()
   {
      return ndf;
   }
   
   public void setNdf(int i)
   {
      checkAccess();
      ndf = i;
   }
   
   public boolean isReferencePointPCA()
   {
      return bitTest(type,BITISREFERENCEPOINTPCA);
   }
   
   public void setReferencePointPCA(boolean b)
   {
      type = bitSet(type,BITISREFERENCEPOINTPCA, b);
   }
   public void setTypeBit(int bit)
   {
      setTypeBit(bit,true) ;
   }
   public void setTypeBit(int bit, boolean val)
   {
      checkAccess();
      type = bitSet(type,bit,val);
   }  
   public void setTracks(List tracks)
   {
      checkAccess();
      this.tracks = tracks;
   }
   
   public void setTrackerHits(List hits)
   {
      checkAccess();
      this.hits = hits;
   }
   
   public float getRadiusOfInnermostHit()
   {
      return radiusOfInnermostHit;
   }
   
   public void setRadiusOfInnermostHit(float f)
   {
      checkAccess();
      radiusOfInnermostHit = f;
   }
   
   public int[] getSubdetectorHitNumbers()
   {
      return subdetectorHitNumbers ;
   }
   
   public void setSubdetectorHitNumbers(int[] is)
   {
      checkAccess();
      subdetectorHitNumbers = is;
   }
}
