package hep.lcio.implementation.event;

import hep.lcio.event.CalorimeterHit;
import hep.lcio.event.LCIO;
import hep.lcio.event.MCParticle;

/**
 * A default implementation of CalorimeterHit
 * @author Tony Johnson
 * @version $Id: ICalorimeterHit.java,v 1.1 2003-05-06 04:58:55 tonyj Exp $
 */
public class ICalorimeterHit extends ILCObject implements CalorimeterHit {
    
    protected int cellId0;
    protected int cellId1;
    protected float energy;
    protected int nContributions;
    protected float[] position = new float[3];
    protected Object[] particle;
    protected float[] energyContrib;
    protected float[] time;
    protected int[] pdg;

    public int getCellID0() {
        return cellId0;
    }
    public void setCellID0(int cellID) {
        checkAccess();
        this.cellId0 = cellID;
    }    
    public int getCellID1() {
        return cellId1;
    }
    public void setCellID1(int cellID) {
        checkAccess();
        this.cellId1 = cellID;
    }      
    public float getEnergy() {
        return energy;
    }
    public void setEnergy(float energy) {
        checkAccess();
        this.energy = energy;
    }      
    public float getEnergyCont(int i) {
        return energyContrib[i];
    }
    public int getNMCParticles() {
        return particle.length;
    }
    public int getPDGCont(int i) {
        //FIXME: What to return if pdg was not specified?
        return pdg[i];
    }
    public MCParticle getParticleCont(int i) {
        return (MCParticle) particle[i];
    }
    public float[] getPosition() {
        //FIXME: lcio.xml says this is double??
        return position;
    }    
    public void setPosition(float[] pos) {
        checkAccess();
        if (pos.length != 3) throw new IllegalArgumentException();
        position = pos;
    }
    public float getTimeCont(int i) {
        return time[i];
    }
    // FIXME: this is not flexible enough, we need to be able to omit the PDG, and also we
    // need to be able to (optionally) set the vertex. Flags should be set appropriately
    public void addMCParticleContribution(MCParticle p, float energy, float time, int pdg)
    {
        checkAccess();
        int i = getIndexForNextContrib();
        this.particle[i] = p;
        this.energyContrib[i] = energy;
        this.time[i] = time;
        this.pdg[i] = pdg;
    }
    private int getIndexForNextContrib()
    {
        int i = ++nContributions;
        if (particle == null || i>particle.length)
        {
            int oldSize = particle == null ? 0 : particle.length;
            int size = oldSize + 10;
            Object old = particle;
            particle = new Object[size];
            if (oldSize > 0) System.arraycopy(old,0, particle, 0,  oldSize);
            old = energyContrib;
            energyContrib = new float[size];
            if (oldSize > 0) System.arraycopy(old,0, energyContrib, 0,  oldSize);
            old = time;
            time = new float[size];
            if (oldSize > 0) System.arraycopy(old,0, time, 0,  oldSize);
            old = pdg;
            pdg = new int[size];
            if (oldSize > 0) System.arraycopy(old,0, pdg, 0,  oldSize);            
        }
        return i;
    }
}