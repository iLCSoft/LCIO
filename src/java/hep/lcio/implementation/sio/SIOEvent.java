package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOBlock;
import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORecord;
import hep.lcd.io.sio.SIOWriter;

import hep.lcio.data.CalorimeterHitData;
import hep.lcio.data.LCCollectionData;
import hep.lcio.data.LCEventData;
import hep.lcio.data.MCParticleData;
import hep.lcio.data.SimCalorimeterHitData;
import hep.lcio.data.SimTrackerHitData;
import hep.lcio.data.TPCHitData;

import hep.lcio.event.CalorimeterHit;
import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.MCParticle;
import hep.lcio.event.SimCalorimeterHit;
import hep.lcio.event.SimTrackerHit;
import hep.lcio.event.TPCHit;

import hep.lcio.implementation.event.ILCCollection;
import hep.lcio.implementation.event.ILCEvent;
import hep.lcio.implementation.event.ILCFloatVec;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOEvent.java,v 1.12 2003-09-15 21:44:32 tonyj Exp $
 */
class SIOEvent extends ILCEvent
{
   private Map blockMap;

   SIOEvent(SIORecord record, int accessMode) throws IOException
   {
      setAccess(accessMode);
      SIOBlock block = record.getBlock();

      if ((block.getMajorVersion() < 1) && (block.getMinorVersion() < 8))
         throw new IOException("Sorry: files created with versions older than v00-08" + " are no longer supported !");

      SIOInputStream in = block.getData();
      runNumber = in.readInt();
      eventNumber = in.readInt();

      timeStamp = in.readLong();
      detectorName = in.readString();

      int nBlockNames = in.readInt();
      blockMap = new HashMap();
      for (int i = 0; i < nBlockNames; i++)
      {
         String blockName = in.readString();
         String blockType = in.readString();

         blockMap.put(blockName, blockType);
      }
   }
   void setReadOnly(boolean mode)
   {
      super.setAccess(mode ? LCIO.READ_ONLY : LCIO.UPDATE);
   }
   void checkSIOAccess()
   {
      super.checkAccess();
   }
   void readData(SIORecord record) throws IOException
   {
      for (;;)
      {
         SIOBlock block = record.getBlock();
         if (block == null)
            break;

         if ((block.getMajorVersion() < 1) && (block.getMinorVersion() < 8))
            throw new IOException("Sorry: files created with versions older than v00-08" + " are no longer supported !");

         SIOInputStream in = block.getData();
         String name = block.getBlockName();
         String type = (String) blockMap.get(name);
         if (type == null)
            continue;
         if (type.equals(LCIO.MCPARTICLE))
         {
            int flags = in.readInt();
            int nMC = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, nMC);
            for (int i = 0; i < nMC; i++)
               ilc.add(new SIOMCParticle(in, this));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.SIMTRACKERHIT))
         {
            int flags = in.readInt();
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            for (int i = 0; i < n; i++)
               ilc.add(new SIOSimTrackerHit(in, this));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.TPCHIT))
         {
            int flags = in.readInt();
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            for (int i = 0; i < n; i++)
               ilc.add (new SIOTPCHit(in,flags, this) );
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.SIMCALORIMETERHIT))
         {
            int flags = in.readInt();
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            for (int i = 0; i < n; i++)
               ilc.add(new SIOSimCalorimeterHit(in, flags, this));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.CALORIMETERHIT))
         {
            int flags = in.readInt();
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            for (int i = 0; i < n; i++)
               ilc.add(new SIOCalorimeterHit(in, flags, this));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.LCFLOATVEC))
         {
            int flags = in.readInt();
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            for (int i = 0; i < n; i++)
               ilc.add(new SIOFloatVec(in, this));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.LCINTVEC))
         {
            int flags = in.readInt();
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            for (int i = 0; i < n; i++)
               ilc.add(new SIOIntVec(in, this));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
      }
   }

   static void write(LCEventData event, SIOWriter writer, boolean headerOnly) throws IOException
   {
      if (headerOnly)
      {
         SIOOutputStream out = writer.createBlock(SIOFactory.eventHeaderBlockName, LCIO.MAJORVERSION, LCIO.MINORVERSION);
         out.writeInt(event.getRunNumber());
         out.writeInt(event.getEventNumber());

         out.writeLong(event.getTimeStamp());
         out.writeString(event.getDetectorName());

         String[] blockNames = event.getCollectionNames();
         out.writeInt(blockNames.length);
         for (int i = 0; i < blockNames.length; i++)
         {
            String blockName = blockNames[i];
            out.writeString(blockName);
            out.writeString(event.getCollectionData(blockName).getTypeName());
         }
      }
      else
      {
         String[] blockNames = event.getCollectionNames();
         for (int j = 0; j < blockNames.length; j++)
         {
            String blockName = blockNames[j];
            SIOOutputStream out = writer.createBlock(blockName, LCIO.MAJORVERSION, LCIO.MINORVERSION);
            LCCollectionData col = event.getCollectionData(blockName);
            String type = col.getTypeName();
            int flags = col.getFlag();
            out.writeInt(flags);

            int n = col.getNumberOfElements();
            out.writeInt(n);
            if (type.equals(LCIO.MCPARTICLE))
            {
               for (int i = 0; i < n; i++)
                  SIOMCParticle.write((MCParticleData) col.getElementAt(i), out);
            }
            else if (type.equals(LCIO.SIMTRACKERHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOSimTrackerHit.write((SimTrackerHitData) col.getElementAt(i), out);
            }
            else if (type.equals(LCIO.TPCHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOTPCHit.write((TPCHitData) col.getElementAt(i), out, flags );
            }
            else if (type.equals(LCIO.SIMCALORIMETERHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOSimCalorimeterHit.write((SimCalorimeterHitData) col.getElementAt(i), out, flags);
            }
            else if (type.equals(LCIO.CALORIMETERHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOCalorimeterHit.write((CalorimeterHitData) col.getElementAt(i), out, flags);
            }
            else if (type.equals(LCIO.LCFLOATVEC))
            {
               for (int i = 0; i < n; i++)
                  SIOFloatVec.write((ILCFloatVec) col.getElementAt(i), out);
            }
         }
      }
   }

   static void write(LCEventData event, SIOWriter writer) throws IOException
   {
      writer.createRecord(SIOFactory.eventHeaderRecordName, SIOFactory.compressionMode);
      SIOEvent.write(event, writer, true);
      writer.createRecord(SIOFactory.eventRecordName, SIOFactory.compressionMode);
      SIOEvent.write(event, writer, false);
   }
}
