package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOReader;
import hep.lcd.io.sio.SIORecord;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.LCRunHeader;
import hep.lcio.implementation.event.ILCEvent;
import hep.lcio.io.LCEventListener;
import hep.lcio.io.LCReader;
import hep.lcio.io.LCRunListener;
import java.io.EOFException;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOLCReader.java,v 1.1 2003-05-06 04:58:56 tonyj Exp $
 */
class SIOLCReader implements LCReader {
    
    private SIOReader reader;
    private List runListeners = new ArrayList();
    private List eventListeners = new ArrayList();
    
    public int open(String filename) {
        try
        {
            reader = new SIOReader(new FileInputStream(filename));
            return LCIO.SUCCESS;
        }
        catch (IOException x) { throw new LCIOException(x); }
    }
    public int close() {
        try
        {
            reader.close();
            return LCIO.SUCCESS;
        }
        catch (IOException x) { throw new LCIOException(x); }
    }
    
    public LCEvent readNextEvent() {
        try
        {
            for (;;)
            {
                SIORecord record = reader.readRecord();
                String name = record.getRecordName();
                if (!name.equals(SIOFactory.eventHeaderRecordName)) continue;
                SIOEvent event = new SIOEvent(record);
                event.readData(reader.readRecord());
                return event;
            }
        }
        catch (EOFException x) { return null; }
        catch (IOException x) { throw new LCIOException(x); }
    }
    
    public LCEvent readNextEvent(int accessMode) {
        //FIXME: What to do with access mode
        return readNextEvent();
    }
    
    public LCRunHeader readNextRunHeader() {
        try
        {
            for (;;)
            {
                SIORecord record = reader.readRecord();
                String name = record.getRecordName();
                if (!name.equals(SIOFactory.runRecordName)) continue;
                return new SIORunHeader(record.getBlock().getData());
            }
        }
        catch (EOFException x) { return null; }
        catch (IOException x) { throw new LCIOException(x); }
    }
    
    public int readStream() {
        try{
            for (;;)
            {
                SIORecord record = reader.readRecord();
                if (record.getRecordName().equals(SIOFactory.runRecordName))
                {
                    int l = runListeners.size();
                    if (l > 0)
                    {
                        SIORunHeader header = new SIORunHeader(record.getBlock().getData());
                        for (int i=0; i<l; i++) ((LCRunListener) runListeners.get(i)).analyze(header);
                    }
                }
                else if (record.getRecordName().equals(SIOFactory.eventHeaderRecordName))
                {
                    int l = eventListeners.size();
                    if (l > 0)
                    {
                        SIOEvent event = new SIOEvent(record);
                        event.readData(reader.readRecord());
                        for (int i=0; i<l; i++) ((LCEventListener) eventListeners.get(i)).update(event);
                    }
                }
            }
        }
        catch (EOFException x) { return LCIO.SUCCESS; }
        catch (IOException x) { throw new LCIOException(x); }
    }
    
    public void registerLCEventListener(LCEventListener ls) {
        eventListeners.add(ls);
    }
    
    public void registerLCRunListener(LCRunListener ls) {
         runListeners.add(ls);
    }
    
    public void removeLCEventListener(LCEventListener ls) {
        eventListeners.remove(ls);
    }
    
    public void removeLCRunListener(LCRunListener ls) {
        runListeners.remove(ls);
    }
}
