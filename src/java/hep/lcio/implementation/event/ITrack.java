package hep.lcio.implementation.event;

import hep.lcio.event.Track;

import java.util.BitSet;
import java.util.Collections;
import java.util.List;

/**
 *
 * @author  Tony Johnson
 * fg20040602 changed to new parameters d0,phi,omega,z0,tanLambda
 * added Ndf and changed type to bitset.
 */
public class ITrack extends ILCObject implements Track
{
   protected float chi2;
   protected float[] covMatrix;
   protected float d0;
   protected float omega ;
   protected float phi;
   protected float[] referencePoint;
   protected float tanLambda;
   //protected int type;
   protected float dEdx;
   protected float dEdxError;
   protected float z0;
   protected int ndf ;
   //boolean isReferencePointPCA ;
   protected BitSet type ;  
   
   public final static int BITISREFERENCEPOINTPCA = 31 ;
   
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
   	  int intType = 0 ;
   	  for (int i = 0; i < 32; i++) {
		if( type.get(i) ){
		   intType |= 1<<i ;
		}	
	  }
      return intType;
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
   

   public void setPhi(float phi)
   {
      this.phi = phi;
   }
   
   public void setReferencePoint(float[] referencePoint)
   {
      if (referencePoint.length != 3) throw new IllegalArgumentException("referencePoint.lenhgth != 3");
      this.referencePoint = referencePoint;
   }
   
   
   protected void setType(int typeWord)
   {
     
      for (int i = 0; i < 32 ; i++) {
		if( (typeWord & (1<<i)) > 1 )
		 type.set(i) ;
		else
         type.clear(i) ;			
	}
      
     // this.type = type;
   }
   
   public void setZ0(float z0)
   {
      this.z0 = z0;
   } 
public float getOmega() {
	return omega;
}

public float getTanLambda() {
	return tanLambda;
}

public void setOmega(float f) {
	omega = f;
}

public void setTanLambda(float f) {
	tanLambda = f;
}

public int getNdf() {
	return ndf;
}

public void setNdf(int i) {
	ndf = i;
}

public boolean isReferencePointPCA() {
    return type.get( BITISREFERENCEPOINTPCA ) ;
}

public void setReferencePointPCA(boolean b) {
	type.set( BITISREFERENCEPOINTPCA, b ) ;
}

	/* (non-Javadoc)
	 * @see hep.lcio.event.Track#testType(int)
	 */
	public boolean testType(int bitIndex) {
	
	return type.get( bitIndex );
	}

}
