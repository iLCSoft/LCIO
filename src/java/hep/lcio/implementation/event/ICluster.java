package hep.lcio.implementation.event;

import hep.lcio.event.Cluster;

import java.util.List;
import java.util.Vector;

/**
 *
 * @author Tony Johnson
 */
public class ICluster extends ILCObject implements Cluster
{
   private static final float[] null3 = new float[3];
   private static final float[] null6 = new float[6];

   protected float energy;
   protected float phi;
   protected float theta;
   protected int type;
   protected float[] directionError = null3;
   protected float[] hitContributions = null3;
   protected float[] particleType = null3;
   protected float[] position = null3;
   protected float[] shape = null6;
   protected float[] positionError = null6;
   protected float[] subdetectorEnergies = new float[0] ;
		   
   public List getCalorimeterHits()
   {
      return null;
   }
   
   public List getClusters()
   {
      return null;
   }
   
   public float[] getDirectionError()
   {
      return directionError;
   }
   
   public float getEnergy()
   {
      return energy;
   }
   
   public float[] getHitContributions()
   {
      return hitContributions;
   }
   
   public float[] getParticleType()
   {
      return particleType;
   }
   
   public float getPhi()
   {
      return phi;
   }
   
   public float[] getPosition()
   {
      return position;
   }
   
   public float[] getPositionError()
   {
      return positionError;
   }
   
   public float[] getShape()
   {
      return shape;
   }
   
   public float getTheta()
   {
      return theta;
   }
   
   public int getType()
   {
      return type;
   }
   
   public boolean testType(int bitIndex)
   {
      return bitTest(type,bitIndex);
   }
   
   public void setCalorimeterHits(List calorimeterHits)
   {
      // FIXME:
   }
   
   public void setClusters(List clusters)
   {
      // FIXME:
   }
   
   public void setDirectionError(float[] directionError)
   {
      checkAccess();
      if (directionError.length != 3) throw new IllegalArgumentException();
      this.directionError = directionError;
   }
   
   public void setEnergy(float energy)
   {
      checkAccess();
      this.energy = energy;
   }
   
   public void setHitContributions(float[] hitContributions)
   {
      checkAccess();
      if (hitContributions.length != 3) throw new IllegalArgumentException();
      this.hitContributions = hitContributions;
   }
   
   public void setParticleType(float[] particleType)
   {
      checkAccess();
      if (particleType.length != 3) throw new IllegalArgumentException();
      this.particleType = particleType;
   }
   
   public void setPhi(float phi)
   {
      checkAccess();
      this.phi = phi;
   }
   
   public void setPosition(float[] position)
   {
      checkAccess();
      if (position.length != 3) throw new IllegalArgumentException();
      this.position = position;
   }
   
   public void setPositionError(float[] positionError)
   {
      checkAccess();
      if (positionError.length != 6) throw new IllegalArgumentException();
      this.positionError = positionError;
   }
   
   public void setShape(float[] shape)
   {
      checkAccess();
      if (shape.length != 6) throw new IllegalArgumentException();
      this.shape = shape;
   }
   
   public void setTheta(float theta)
   {
      checkAccess();
      this.theta = theta;
   }
   
   public void setType(int type)
   {
      checkAccess();
      this.type = type;
   }
   public void setTypeBit(int bit)
   {
	 setTypeBit(bit,true) ;
   }
   public void setTypeBit(int bit, boolean val)
   {
	  checkAccess();
	  bitSet(type,bit,val);
   }

   public float[] getSubdetectorEnergies()
   {
      return subdetectorEnergies ;
   }  
   public void setSubdetectorEnergies(float[] fs)
   {
      checkAccess();
      // subdetector energies can have arbitrary size 
      //if (fs.length != 3) throw new IllegalArgumentException();      
      subdetectorEnergies = fs ;
   }
}
