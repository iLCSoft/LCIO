package hep.lcio.implementation.event;

import hep.lcio.data.MCParticleData;

import hep.lcio.event.MCParticle;


/**
 * A default implementation of MCParticle
 * @author Tony Johnson
 * @version $Id: IMCParticle.java,v 1.5 2003-09-04 04:27:00 tonyj Exp $
 */
public class IMCParticle extends ILCObject implements MCParticle
{
   private static final MCParticle[] noDaughters = new MCParticle[0];
   protected Object parent;
   protected Object secondParent;
   protected Object[] daughters = noDaughters;
   protected double[] endpoint = new double[3];
   protected float[] momentum = new float[3];
   protected double[] vertex = new double[3];
   protected float charge;
   protected float mass;
   protected int hepEvtStatus;
   protected int pdg;

   public void setCharge(float charge)
   {
      checkAccess();
      this.charge = charge;
   }

   public float getCharge()
   {
      return charge;
   }

   public MCParticle getDaughter(int i)
   {
      return (MCParticle) daughters[i];
   }

   public MCParticleData getDaughterData(int i)
   {
      return (MCParticleData) daughters[i];
   }

   public void setDaughters(MCParticle[] daughters)
   {
      checkAccess();
      this.daughters = daughters;
   }

   public void setEndpoint(double[] pos)
   {
      checkAccess();
      if (pos.length != 3)
         throw new IllegalArgumentException();
      this.endpoint = pos;
   }

   public double[] getEndpoint()
   {
      return endpoint;
   }

   public float getEnergy()
   {
      return (float) Math.sqrt((momentum[0] * momentum[0]) + (momentum[1] * momentum[1]) + (momentum[2] * momentum[2]) + (mass * mass));
   }

   public void setHepEvtStatus(int status)
   {
      checkAccess();
      this.hepEvtStatus = status;
   }

   public int getHepEvtStatus()
   {
      return hepEvtStatus;
   }

   public void setMass(float mass)
   {
      checkAccess();
      this.mass = mass;
   }

   public float getMass()
   {
      return mass;
   }

   public void setMomentum(float[] momentum)
   {
      checkAccess();
      if (momentum.length != 3)
         throw new IllegalArgumentException();
      this.momentum = momentum;
   }

   public float[] getMomentum()
   {
      return momentum;
   }

   public int getNumberOfDaughters()
   {
      return daughters.length;
   }

   public void setPDG(int pdg)
   {
      checkAccess();
      this.pdg = pdg;
   }

   public int getPDG()
   {
      return pdg;
   }

   public void setParent(MCParticle parent)
   {
      checkAccess();
      this.parent = parent;
   }

   public MCParticle getParent()
   {
      return (MCParticle) parent;
   }

   public MCParticleData getParentData()
   {
      return (MCParticleData) parent;
   }

   public void setSecondParent(MCParticle parent)
   {
      checkAccess();
      this.secondParent = secondParent;
   }

   public MCParticle getSecondParent()
   {
      return (MCParticle) secondParent;
   }

   public MCParticleData getSecondParentData()
   {
      return (MCParticleData) secondParent;
   }

   public void setVertex(double[] pos)
   {
      checkAccess();
      if (pos.length != 3)
         throw new IllegalArgumentException();
      this.vertex = pos;
   }

   public double[] getVertex()
   {
      return vertex;
   }
}
