package hep.lcio.implementation.event;

import hep.lcio.event.Cluster;

/**
 *
 * @author Tony Johnson
 */
public class ICluster extends ILCObject implements Cluster
{
   protected float energy;
   protected float phi;
   protected float theta;
   protected int type;
   protected float[] directionError;
   protected float[] hitContributions;
   protected float[] particleType;
   protected float[] position;
   protected float[] shape;
   protected float[] positionError;
   
   public java.util.Vector getCalorimeterHits()
   {
      return null;
   }
   
   public java.util.Vector getClusters()
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
   
   public void setCalorimeterHits(java.util.Vector calorimeterHits)
   {
      // FIXME:
   }
   
   public void setClusters(java.util.Vector clusters)
   {
      // FIXME:
   }
   
   public void setDirectionError(float[] directionError)
   {
      this.directionError = directionError;
   }
   
   public void setEnergy(float energy)
   {
      this.energy = energy;
   }
   
   public void setHitContributions(float[] hitContributions)
   {
      this.hitContributions = hitContributions;
   }
   
   public void setParticleType(float[] particleType)
   {
      this.particleType = particleType;
   }
   
   public void setPhi(float phi)
   {
      this.phi = phi;
   }
   
   public void setPosition(float[] position)
   {
      this.position = position;
   }
   
   public void setPositionError(float[] positionError)
   {
      this.positionError = positionError;
   }
   
   public void setShape(float[] shape)
   {
      this.shape = shape;
   }
   
   public void setTheta(float theta)
   {
      this.theta = theta;
   }
   
   public void setType(int type)
   {
      this.type = type;
   }
   
}
