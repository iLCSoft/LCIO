package hep.lcio.implementation.event;

import hep.lcio.event.MCParticle;
import hep.lcio.event.TrackerHit;

/**
 * A default implementation of TrackerHit
 * @author Tony Johnson
 * @version $Id: ITrackerHit.java,v 1.1 2003-05-06 04:58:55 tonyj Exp $
 */
public class ITrackerHit extends ILCObject implements TrackerHit {
    
    protected int cellID;
    protected Object particle;
    protected double[] position = new double[3];
    protected float time;
    protected float dEdx;
    
    public int getCellID() {
        return cellID;
    }
    public void setCellID(int cellID)
    {
        checkAccess();
        this.cellID = cellID;
    }
    
    public MCParticle getMCParticle() {
        return (MCParticle) particle;
    }
    public void setMCParticle(MCParticle mc)
    {
        checkAccess();
        particle = mc;
    }
    
    public double[] getPosition() {
        return position;
    }
    public void setPosition(double[] pos)
    {
        checkAccess();
        if (pos.length != 3) throw new IllegalArgumentException();
        position = pos;
    }
    
    public float getTime() {
        return time;
    }
    public void setTime(float time) {
        checkAccess();
        this.time = time;
    }
    public float getdEdx() {
        return dEdx;
    }
    public void setdEdx(float dEdx) {
        checkAccess();
        this.dEdx = dEdx;
    }
}