package hep.lcio.implementation.event;

import hep.lcio.event.MCParticle;

/**
 * A default implementation of MCParticle
 * @author Tony Johnson
 * @version $Id: IMCParticle.java,v 1.1 2003-05-06 04:58:55 tonyj Exp $
 */
public class IMCParticle extends ILCObject implements MCParticle {
    
    private static final MCParticle[] noDaughters = new MCParticle[0];
    protected float charge;
    protected float energy;
    protected int hepEvtStatus;
    protected float[] momentum = new float[3];
    protected double[] vertex = new double[3];
    protected int pdg;
    protected Object parent;
    protected Object secondParent;
    protected Object[] daughters = noDaughters;
    
    public float getCharge() {
        return charge;
    } 
    public void setCharge(float charge) {
        checkAccess();
        this.charge = charge;
    }
    
    public MCParticle[] getDaughters() {
        return (MCParticle[]) daughters;
    }
    public void setDaughters(MCParticle[] daughters)
    {
        checkAccess();        
        this.daughters = daughters;
    }
    public float getEnergy() {
        return energy;
    }
    public void setEnergy(float energy) {
        checkAccess();
        this.energy = energy;
    }
    
    public int getHepEvtStatus() {
        return hepEvtStatus;
    }
    public void setHepEvtStatus(int status) {
        checkAccess();
        this.hepEvtStatus = status;
    }
    
    public float[] getMomentum() {
        return momentum;
    }
    public void setMomentum(float[] momentum)
    {
        checkAccess();
        if (momentum.length != 3) throw new IllegalArgumentException();
        this.momentum = momentum;
    }
    public int getPDG() {
        return pdg;
    }
    public void setPDG(int pdg) {
        checkAccess();
        this.pdg = pdg;
    }    
    public MCParticle getParent() {
        return (MCParticle) parent;
    }
    public void setParent(MCParticle parent) {
        checkAccess();
        this.parent = parent;
    }    
    public MCParticle getSecondParent() {
        return (MCParticle) secondParent;
    }
    public void setSecondParent(MCParticle parent) {
        checkAccess();
        this.secondParent = secondParent;
    }      
    public double[] getVertex() {
        return vertex;
    }
    public void setVertex(double[] pos)
    {
        checkAccess();
        if (pos.length != 3) throw new IllegalArgumentException();
        this.vertex = pos;
    }
}