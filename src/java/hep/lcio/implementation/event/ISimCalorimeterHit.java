package hep.lcio.implementation.event;

import java.util.ArrayList;
import java.util.List;

import hep.lcio.event.MCParticle;
import hep.lcio.event.SimCalorimeterHit;


/**
 * A default implementation of SimCalorimeterHit
 * @author Tony Johnson
 * @version $Id: ISimCalorimeterHit.java,v 1.9 2004-09-24 10:39:29 tonyj Exp $
 */
public class ISimCalorimeterHit extends ILCObject implements SimCalorimeterHit
{
   protected float[] energyContrib;
   protected Object[] particle;
   protected int[] pdg;
   protected float[] position = new float[3];
   protected float[] time;
   protected float[] length;
   protected float energy;
   protected int cellId0;
   protected int cellId1;
   protected int nContributions;
   //protected float[] step = new float[3];
   protected List steps ;
   
   public void setCellID0(int cellID)
   {
      checkAccess();
      this.cellId0 = cellID;
   }
   
   public int getCellID0()
   {
      return cellId0;
   }
   
   public void setCellID1(int cellID)
   {
      checkAccess();
      this.cellId1 = cellID;
   }
   
   public int getCellID1()
   {
      return cellId1;
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
   
   public float getEnergyCont(int i)
   {
      return energyContrib[i];
   }
   
   public int getNMCParticles()
   {
      return getNMCContributions() ;
   }
   
   public int getPDGCont(int i)
   {
      return pdg[i];
   }

   public float[] getStepPosition(int i)
   {
	  return (float[]) steps.toArray()[i] ;
      ///throw new RuntimeException("getStepPosition not yet implemented");
   }
   
   public MCParticle getParticleCont(int i)
   {
      return (MCParticle) particle[i];
   }
   
   public void setPosition(float[] pos)
   {
      checkAccess();
      if (pos.length != 3)
         throw new IllegalArgumentException();
      position = pos;
   }
   
   public float[] getPosition()
   {
      return position;
   }
   
   public float getTimeCont(int i)
   {
      return time[i];
   }
   

  public float getLengthCont(int i){
    return length[i];
  }

  public void addMCParticleContribution(MCParticle p, float energy, float time, int pdg){
    addMCParticleContribution( p , energy, time , 0 , pdg,  null ) ;
  }
  
  public void addMCParticleContribution(MCParticle p, float energy, float time, int pdg, float stepPos[]){
    addMCParticleContribution( p , energy, time , 0 , pdg,  stepPos ) ;
  }

  public void addMCParticleContribution(MCParticle p, float energy, float time, float length, int pdg, float stepPos[])
    {
      checkAccess();
      
      int i = getIndexForNextContrib();
      this.particle[i] = p;
      this.energyContrib[i] = energy;
      this.time[i] = time;
      this.length[i] = length;
      this.pdg[i] = pdg;
      
      if( steps==null ) steps = new ArrayList() ;
      if( stepPos == null ) stepPos = new float[3] ; 
      steps.add( stepPos ) ;
   }
   
   private int getIndexForNextContrib()
   {
      int i = nContributions++;
      if ((particle == null) || (i >= particle.length))
      {
         int oldSize = (particle == null) ? 0 : particle.length;
         int size = oldSize + 10;
         Object old = particle;
         particle = new Object[size];
         if (oldSize > 0) System.arraycopy(old, 0, particle, 0, oldSize);
         old = energyContrib;
         energyContrib = new float[size];
         if (oldSize > 0) System.arraycopy(old, 0, energyContrib, 0, oldSize);
         old = time;
         time = new float[size];
         if (oldSize > 0) System.arraycopy(old, 0, time, 0, oldSize);
         length = new float[size];
         if (oldSize > 0) System.arraycopy(old, 0, length, 0, oldSize);
         old = pdg;
         pdg = new int[size];
         if (oldSize > 0) System.arraycopy(old, 0, pdg, 0, oldSize);
      }
      return i;
   }
   
   public int getNMCContributions()
   {
      return nContributions;
   }
   
}
