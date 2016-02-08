package hep.lcio.implementation.event;

import hep.lcio.event.MCParticle;

import java.util.ArrayList;
import java.util.List;


/**
 * A default implementation of MCParticle.
 * @author Tony Johnson
 * @version $Id: IMCParticle.java,v 1.14 2005-06-01 15:10:44 gaede Exp $
 */
public class IMCParticle extends ILCObject implements MCParticle
{
   protected List daughters = new ArrayList();
   protected List parents = new ArrayList();
   protected double[] endpoint = new double[3];
   protected double[] momentum = new double[3];
   protected double[] vertex = new double[3];
   protected double[] momentumAtEndpoint = new double[3];
   protected float charge;
   protected float time ;
   protected double mass;
   protected int generatorStatus;
   protected int pdg;
   protected int simulatorStatus;
   protected float[] spin = new float[3];
   protected int[] colorFlow = new int[2];
   
   public void setSpin(float[] spin)
   {
      checkAccess();
      if (spin.length != 3)
         throw new IllegalArgumentException();
      this.spin = spin;
   }
 
   public float[] getSpin()
   {
      return spin;
   }

   public void setColorFlow(int[] cflow)
   {
      checkAccess();
      if (cflow.length != 2)
         throw new IllegalArgumentException();
      this.colorFlow = cflow;
   }

   public int[] getColorFlow()
   {
      return colorFlow;
   }

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
   
   public double getEnergy()
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
   
   public double getMass()
   {
      return mass;
   }
   
   public void setMomentum(float[] momentum)
   {
      checkAccess();
      if (momentum.length != 3)
         throw new IllegalArgumentException();
      setMomentum( momentum[0], momentum[1], momentum[2]) ;
   }
   
   public void setMomentum(double[] momentum)
   {
      checkAccess();
      if (momentum.length != 3)
         throw new IllegalArgumentException();
      this.momentum = momentum;
   }
   
   public void setMomentum(double px , double py, double pz){
      this.momentum[0] = px ;
      this.momentum[1] = py ;
      this.momentum[2] = pz ;
      
   }
   
   public double[] getMomentum()
   {
      return momentum;
   }
   
   public double[] getMomentumAtEndpoint() {
      return momentumAtEndpoint; 
   }
   
   public void setMomentumAtEndpoint(float[] momemtum)
   {
      checkAccess();
      if (momentum.length != 3)
         throw new IllegalArgumentException(); 
      momentumAtEndpoint[0] = momentum[0];
      momentumAtEndpoint[1] = momentum[1];
      momentumAtEndpoint[2] = momentum[2];
   }
   
   public void setMomentumAtEndpoint(double[] momemtum)
   {
      checkAccess();
      if (momentum.length != 3)
         throw new IllegalArgumentException(); 
      momentumAtEndpoint[0] = momentum[0];
      momentumAtEndpoint[1] = momentum[1];
      momentumAtEndpoint[2] = momentum[2];
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
      // fg: this is inconsistent with c++ - enpoint bit is not different from other bits
      //status |= (0x7fffffff & status); // bit 31 reserved for endpoint
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
      return bitTest(simulatorStatus,BITLeftDetector);
   }
   
   public boolean isBackscatter()
   {
      return bitTest(simulatorStatus,BITBackscatter);
   }
   
   public boolean isCreatedInSimulation()
   {
      return bitTest(simulatorStatus,BITCreatedInSimulation);
   }
   
   public boolean isDecayedInCalorimeter()
   {
      return bitTest(simulatorStatus,BITDecayedInCalorimeter);
   }
   
   public boolean isDecayedInTracker()
   {
      return bitTest(simulatorStatus,BITDecayedInTracker);
   }
   
   public boolean isStopped()
   {
      return bitTest(simulatorStatus,BITStopped);
   }

   public boolean isOverlay()
   {
      return bitTest(simulatorStatus,BITOverlay);
   }

   private void setBit(int bit, boolean value)
   {
      checkAccess();
      simulatorStatus = bitSet(simulatorStatus,bit,value);
   }
   public void setBackscatter(boolean val)
   {
      setBit(BITBackscatter,val);
   }
   
   public void setCreatedInSimulation(boolean val)
   {
      setBit(BITCreatedInSimulation,val);
   }
   
   public void setDecayedInCalorimeter(boolean val)
   {
      setBit(BITDecayedInCalorimeter,val);
   }
   
   public void setDecayedInTracker(boolean val)
   {
      setBit(BITDecayedInTracker,val);
   }
   
   public void setHasLeftDetector(boolean val)
   {
      setBit(BITLeftDetector,val);
   }
   
   public void setStopped(boolean val)
   {
      setBit(BITStopped,val);
   }
   
   public void setOverlay(boolean val)
   {
      setBit(BITOverlay,val);
   }
   
   public void setVertexIsNotEndpointOfParent(boolean val)
   {
      setBit(BITVertexIsNotEndpointOfParent,val);
   }
   
   public boolean vertexIsNotEndpointOfParent()
   {
      return bitTest(simulatorStatus,BITVertexIsNotEndpointOfParent);
   }
   
   public List getDaughters()
   {
      return daughters;
   }
   
   public List getParents()
   {
      return parents;
   }
   
   public float getTime()
   {
      return time;
   }
   
   public void setTime(float f)
   {
      checkAccess();
      time = f;
   }
   
}
