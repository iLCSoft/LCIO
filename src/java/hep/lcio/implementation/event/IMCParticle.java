package hep.lcio.implementation.event;

import hep.lcio.event.MCParticle;

import java.util.ArrayList;
import java.util.List;


/**
 * A default implementation of MCParticle.
 * @author Tony Johnson
 * @version $Id: IMCParticle.java,v 1.9 2004-05-24 03:33:02 tonyj Exp $
 */
public class IMCParticle extends ILCObject implements MCParticle
{
   protected List daughters = new ArrayList();
   protected List parents = new ArrayList();
   protected double[] endpoint = new double[3];
   protected float[] momentum = new float[3];
   protected double[] vertex = new double[3];
   protected float charge;
   protected float mass;
   protected int generatorStatus;
   protected int pdg;
   protected int simulatorStatus;
   
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
      return (MCParticle) daughters.get(i);
   }
   
   
   public void setEndpoint(double[] pos)
   {
      checkAccess();
      if (pos.length != 3)
         throw new IllegalArgumentException();
      simulatorStatus |= (1 << BITEndpoint); // set bit 31
      this.endpoint = pos;
   }
   
   public double[] getEndpoint()
   {
      if ((simulatorStatus & 1<<BITEndpoint) == 0)
      {
         for(int i=0;i<getNumberOfDaughters();i++)
         {
            if(!getDaughter(i).vertexIsNotEndpointOfParent())
               return getDaughter(i).getVertex();
         }
      }
      return endpoint ;
   }
   
   public float getEnergy()
   {
      return (float) Math.sqrt((momentum[0] * momentum[0]) + (momentum[1] * momentum[1]) + (momentum[2] * momentum[2]) + (mass * mass));
   }
   
   public void setGeneratorStatus(int status)
   {
      checkAccess();
      this.generatorStatus = status;
   }
   
   public int getGeneratorStatus()
   {
      return generatorStatus;
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
      return daughters.size();
   }
   
   public int getNumberOfParents()
   {
      return parents.size();
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
   
   public MCParticle getParent(int i)
   {
      return (MCParticle) parents.get(i);
   }
   
   public void setSimulatorStatus(int status)
   {
      checkAccess();
      status |= (0x7fffffff & status); // bit 31 reserved for endpoint
      this.simulatorStatus = status;
   }
   
   public int getSimulatorStatus()
   {
      return simulatorStatus;
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
   
   public void addDaughter(MCParticle daughter)
   {
      daughters.add(daughter);
      ((IMCParticle) daughter).addParent(this);
   }
   
   public void removeAllDaughters()
   {
      for (int i = 0; i < daughters.size(); i++)
         ((IMCParticle) daughters.get(i)).removeParent(this);
      daughters.clear();
   }
   
   public void removeDaughter(MCParticle daughter)
   {
      daughters.remove(daughter);
      ((IMCParticle) daughter).removeParent(this);
   }
   
   private void addParent(MCParticle parent)
   {
      parents.add(parent);
   }
   
   private void removeParent(MCParticle parent)
   {
      parents.remove(parent);
   }
   
   public boolean hasLeftDetector()
   {
      return (simulatorStatus & 1 << BITLeftDetector) != 0;
   }
   
   public boolean isBackscatter()
   {
      return (simulatorStatus & 1 << BITBackscatter) != 0;
   }
   
   public boolean isCreatedInSimulation()
   {
      return (simulatorStatus & 1 << BITCreatedInSimulation) != 0;
   }
   
   public boolean isDecayedInCalorimeter()
   {
      return (simulatorStatus & 1 << BITDecayedInCalorimeter) != 0;
   }
   
   public boolean isDecayedInTracker()
   {
      return (simulatorStatus & 1 << BITDecayedInTracker) != 0;
   }
   
   public boolean isStopped()
   {
      return (simulatorStatus & 1 << BITStopped) != 0;
      
   }
   private void setBit(int pattern, boolean value)
   {
      checkAccess();
      if (value) simulatorStatus |= pattern;
      else simulatorStatus &= ~pattern;
   }
   //FixMe: Should these really be in the interface
   public void setBackscatter(boolean val)
   {
      setBit(1<<BITBackscatter,val);
   }
   
   public void setCreatedInSimulation(boolean val)
   {
      setBit(1<<BITCreatedInSimulation,val);
   }
   
   public void setDecayedInCalorimeter(boolean val)
   {
      setBit(1<<BITDecayedInCalorimeter,val);
   }
   
   public void setDecayedInTracker(boolean val)
   {
      setBit(1<<BITDecayedInTracker,val);
   }
   
   public void setHasLeftDetector(boolean val)
   {
      setBit(1<<BITLeftDetector,val);
   }
   
   public void setStopped(boolean val)
   {
      setBit(1<<BITStopped,val);
   }
   
   public void setVertexIsNotEndpointOfParent(boolean val)
   {
      setBit(1<<BITVertexIsNotEndpointOfParent,val);
   }
   
   public boolean vertexIsNotEndpointOfParent()
   {
      return (simulatorStatus & 1 << BITVertexIsNotEndpointOfParent) != 0;
   }
   
}
