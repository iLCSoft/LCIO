package hep.lcio.implementation.event;

import hep.lcio.event.LCRunHeader;
import java.util.Arrays;
import java.util.List;

/**
 * A default implementation of LCRunHeader
 * @author Tony Johnson
 * @version $Id: ILCRunHeader.java,v 1.1 2003-05-06 04:58:55 tonyj Exp $
 */
public class ILCRunHeader extends ILCObject implements LCRunHeader {
    
    public String[] getActiveSubdetectors() {
        return activeSubdetectors;
    }
    public void addActiveSubdetector(String det) {
        checkAccess();
        int l = activeSubdetectors.length;
        String[] newActive = new String[l+1];
        System.arraycopy(activeSubdetectors,0, newActive, 0, l);
        newActive[l] = det;
        activeSubdetectors = newActive;
    }
    public String getDescription()
    {
        return description;
    }
    public void setDescription(String desc)
    {
        checkAccess();
        this.description = desc;
    }
    
    public String getDetectorName() {
        return detectorName;
    }
    public void setDetectorName(String name)
    {
        checkAccess();
        this.detectorName = name;
    }
    
    public int getRunNumber() {
        return runNumber;
    }
    public void setRunNumber(int run) {
        checkAccess();
        this.runNumber = run;
    }
    protected int runNumber;
    protected String detectorName;
    protected String description;
    protected String[] activeSubdetectors = new String[0];
}