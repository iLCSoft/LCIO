package hep.lcio.implementation.event;

import hep.lcio.event.MCParticle;


/**
 * A default implementation of MCParticle
 * @author Tony Johnson
 * @version $Id: IMCParticle.java,v 1.2 2003-05-06 06:22:11 tonyj Exp $
 */
public class IMCParticle extends ILCObject implements MCParticle
{
   private static final MCParticle[] noDaughters = new MCParticle[0];
   protected Object parent;
   protected Object secondParent;
   protected Object[] daughters = noDaughters;
   protected float[] momentum = new float[3];
   protected double[] vertex = new double[3];
   protected float charge;
   protected float energy;
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

   public void setDaughters(MCParticle[] daughters)
   {
      checkAccess();
      this.daughters = daughters;
   }

   public MCParticle[] getDaughters()
   {
      return (MCParticle[]) daughters;
   }

   public void setEnergy(float energy)
   {
      checkAccess();
      this.energy = energy;
   }

   public float getEnergy()
   {
      return energy;
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

   public void setSecondParent(MCParticle parent)
   {
      checkAccess();
      this.secondParent = secondParent;
   }

   public MCParticle getSecondParent()
   {
      return (MCParticle) secondParent;
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
