package hep.lcio.implementation.event;

import hep.lcio.event.Cluster;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Tony Johnson
 */
public class ICluster extends ILCObject implements Cluster
{
   private static final float[] null0 = new float[0];
   private static final float[] null3 = new float[3];
   private static final float[] null6 = new float[6];
   
   protected float energy;
   protected float energyError;
   protected float phi;
   protected float theta;
   protected int type;
   protected float[] directionError = null3;
   protected float[] hitContributions = null0;
   protected float[] position = null3;
   protected float[] shape = null0;
   protected List particleIDs = new ArrayList();
   protected List calorimeterHits = new ArrayList();
   protected List clusters = new ArrayList();
   protected float[] positionError = null6;
   protected float[] subdetectorEnergies = null0;
   
   public List getCalorimeterHits()
   {
      return calorimeterHits;
   }
   
   public List getClusters()
   {
      return clusters;
   }
   
   public float[] getDirectionError()
   {
      return directionError;
   }
   
   public float getEnergy()
   {
      return energy;
   }

   public float getEnergyError()
   {
      return energyError;
   }
   
   public float[] getHitContributions()
   {
      return hitContributions;
   }
    
   public float getIPhi()
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
   
   public float getITheta()
   {
      return theta;
   }
   
   public int getType()
   {
      return type;
   }
   
   public void setCalorimeterHits(List calorimeterHits,float[] hitContributions)
   {
      if (calorimeterHits == null)
      {
         if (hitContributions == null) hitContributions = null0;
         if (hitContributions.length != 0) throw new IllegalArgumentException();
      }
      else if (calorimeterHits.size() != hitContributions.length) throw new IllegalArgumentException();
      checkAccess();
      this.calorimeterHits = calorimeterHits;
      this.hitContributions = hitContributions;
   }
   
   public void setClusters(List clusters)
   {
      checkAccess();
      this.clusters = clusters;
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

   public void setEnergyError(float energyError)
   {
      checkAccess();
      this.energyError = energyError;
   }
   
   public void setIPhi(float phi)
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
   
   public void setITheta(float theta)
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
      type = bitSet(type,bit,val);
   }
   
   public float[] getSubdetectorEnergies()
   {
      return subdetectorEnergies;
   }
   public void setSubdetectorEnergies(float[] fs)
   {
      checkAccess();
      subdetectorEnergies = fs;
   }

   public List getParticleIDs()
   {
      return particleIDs;
   }
      
   public void setParticleIDs(List list)
   {
      checkAccess();
      particleIDs = list;
   }  
}
