package hep.lcio.implementation.event;

import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * A default implementation of LCEvent
 * @author Tony Johnson
 * @version $Id: ILCEvent.java,v 1.1 2003-05-06 04:58:55 tonyj Exp $
 */
public class ILCEvent extends ILCObject implements LCEvent {
     
    public int addCollection(LCCollection col, String name) {
        //FIXME: is this too severe?
        checkAccess();
        collections.put(name,col);
        return LCIO.SUCCESS;
    }
    public LCCollection getCollection(String name) {
        return (LCCollection) collections.get(name);
    }
    public String[] getCollectionNames() {
        String[] keys = new String[collections.size()];
        collections.keySet().toArray(keys);
        return keys;
    }
    public int removeCollection(String name) {
        collections.remove(name);
        return LCIO.SUCCESS;
    }
    public String getDetectorName() {
        return detectorName;
    }   
    public void setDetectorName(String detectorName) {
        checkAccess();
        this.detectorName = detectorName;
    } 
    public int getEventNumber() {
        return eventNumber;
    }
    public void setEventNumber(int event) {
        checkAccess();
        this.eventNumber = event;
    } 
    public int getRunNumber() {
        return runNumber;
    }
    public void setRunNumber(int run) {
        checkAccess();
        this.runNumber = run;
    } 
    public long getTimeStamp() {
        return timeStamp;
    }
    public void setTimeStamp(long timeStamp) {
        checkAccess();
        this.timeStamp = timeStamp;
    }
    
    protected long timeStamp;
    protected String detectorName;
    protected int runNumber;
    protected int eventNumber;
    private Map collections = new LinkedHashMap();
}