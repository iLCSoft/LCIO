package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOBlock;
import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORecord;
import hep.lcd.io.sio.SIOWriter;
import hep.lcio.event.CalorimeterHit;
import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.MCParticle;
import hep.lcio.event.TrackerHit;
import hep.lcio.implementation.event.ILCEvent;
import hep.lcio.implementation.event.ILCCollection;
import hep.lcio.implementation.event.ILCFloatVec;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOEvent.java,v 1.1 2003-05-06 04:58:56 tonyj Exp $
 */
class SIOEvent extends ILCEvent {
    
    // FIXME: Work around bug in C++ code
    private static String stringHack(SIOInputStream in) throws IOException
    {
        int l = in.readInt();
	if (l > 32767) throw new IOException("String too long: "+l);
	byte[] ascii = new byte[l];
	in.readFully(ascii);
        in.readByte(); // skip irrelevant null
	in.pad();
	return new String(ascii); //BUG: what if default locale is not US-ASCII
    }
    private static void hackString(SIOOutputStream out, String str) throws IOException
    {
        out.writeInt(str.length());
        byte[] ascii = str.getBytes();
	out.write(ascii);
        out.writeByte(0); // write irrelevant null
	out.pad();
    }
    SIOEvent(SIORecord record) throws IOException
    {      
        SIOBlock block = record.getBlock();
        SIOInputStream in = block.getData();
        runNumber = in.readInt();
        eventNumber = in.readInt();
        //FIXME: This is in the wrong order according to lcio.xml
        timeStamp = in.readLong();
        detectorName = in.readString();
        int nBlockNames = in.readInt();
        blockMap = new HashMap();
        for (int i=0; i<nBlockNames; i++)
        {
            String blockName = stringHack(in);
            String blockType = stringHack(in);
            blockMap.put(blockName, blockType);
        }
    }
    void readData(SIORecord record) throws IOException
    {
        for (;;)
        {
            SIOBlock block = record.getBlock();
            if (block == null) break;
            SIOInputStream in = block.getData();
            String name = block.getBlockName();
            String type = (String) blockMap.get(name);
            if (type == null) continue;
            if (type.equals(LCIO.MCPARTICLE)) 
            {
                int flags = in.readInt();
                int nMC = in.readInt();
                ILCCollection ilc = new ILCCollection(type,flags,nMC);
                for (int i=0; i<nMC; i++) ilc.add(new SIOMCParticle(in));
                addCollection(ilc,name);
            }
            else if (type.equals(LCIO.TRACKERHIT)) 
            {
                int flags = in.readInt();
                int n = in.readInt();
                ILCCollection ilc = new ILCCollection(type,flags,n);
                for (int i=0; i<n; i++) ilc.add(new SIOTrackerHit(in));
                addCollection(ilc,name);
            }
            else if (type.equals(LCIO.CALORIMETERHIT)) 
            {
                int flags = in.readInt();
                int n = in.readInt();
                ILCCollection ilc = new ILCCollection(type,flags,n);
                for (int i=0; i<n; i++) ilc.add(new SIOCalorimeterHit(in,flags));
                addCollection(ilc,name);
            }
            else if (type.equals(LCIO.LCFLOATVEC))
            {
                int flags = in.readInt();
                int n = in.readInt();
                ILCCollection ilc = new ILCCollection(type,flags,n);
                for (int i=0; i<n; i++) ilc.add(new SIOFloatVec(in));
                addCollection(ilc,name);
            }
        }
    }
    static void write(LCEvent event, SIOWriter writer, boolean headerOnly) throws IOException
    {
        //FIXME: This isnt what lcio.xml says
        if (headerOnly)
        {
        SIOOutputStream out = writer.createBlock(LCIO.LCEVENT, LCIO.MAJORVERSION, LCIO.MINORVERSION);
        out.writeInt(event.getRunNumber());
        out.writeInt(event.getEventNumber());
        //FIXME: This is in the wrong order according to lcio.xml
        out.writeLong(event.getTimeStamp());
        out.writeString(event.getDetectorName());
        String[] blockNames = event.getCollectionNames();
        out.writeInt(blockNames.length);
        for (int i=0; i<blockNames.length; i++)
        {
            String blockName = blockNames[i];
            hackString(out,blockName);
            hackString(out,event.getCollection(blockName).getTypeName());
        }   
        }
        else
        {
        String[] blockNames = event.getCollectionNames();
        for (int j=0; j<blockNames.length; j++)
        {
            String blockName = blockNames[j];
            SIOOutputStream out = writer.createBlock(blockName,LCIO.MAJORVERSION,LCIO.MINORVERSION);
            LCCollection col = event.getCollection(blockName);
            String type = col.getTypeName();
            int flags = col.getFlag();
            out.writeInt(flags);
            int n = col.getNumberOfElements();
            out.writeInt(n);
            if (type.equals(LCIO.MCPARTICLE)) 
            {
                for (int i=0; i<n; i++) SIOMCParticle.write((MCParticle) col.getElementAt(i),out);
            }
            else if (type.equals(LCIO.TRACKERHIT)) 
            {
                for (int i=0; i<n; i++) SIOTrackerHit.write((TrackerHit) col.getElementAt(i),out);
            }
            else if (type.equals(LCIO.CALORIMETERHIT)) 
            {
                for (int i=0; i<n; i++) SIOCalorimeterHit.write((CalorimeterHit) col.getElementAt(i),out, flags);
            }
            else if (type.equals(LCIO.LCFLOATVEC))
            {
                for (int i=0; i<n; i++) SIOFloatVec.write((ILCFloatVec) col.getElementAt(i),out);
            }        
        }
        }
    }
    static void write(LCEvent event, SIOWriter writer) throws IOException {
            writer.createRecord(SIOFactory.eventHeaderRecordName,true);
            SIOEvent.write(event,writer,true);
            writer.createRecord(SIOFactory.eventRecordName,true);
            SIOEvent.write(event,writer,false);
    }
    private Map blockMap;
}