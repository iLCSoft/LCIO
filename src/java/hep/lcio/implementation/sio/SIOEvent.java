package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOBlock;
import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORecord;
import hep.lcd.io.sio.SIOWriter;

import hep.lcio.event.SimCalorimeterHit;
import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.MCParticle;
import hep.lcio.event.SimTrackerHit;

import hep.lcio.implementation.event.ILCCollection;
import hep.lcio.implementation.event.ILCEvent;
import hep.lcio.implementation.event.ILCFloatVec;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOEvent.java,v 1.6 2003-06-06 13:05:57 gaede Exp $
 */
class SIOEvent extends ILCEvent
{
   private Map blockMap;

   SIOEvent(SIORecord record) throws IOException
   {

      SIOBlock block = record.getBlock();
      //fg 20030509 versions older than v00-03 no longer supported
      if( block.getMajorVersion() <1 && block.getMinorVersion() < 3)
	  throw new IOException("Sorry: files created with versions older than v00-03"
				+" are no longer supported !" ) ;

      SIOInputStream in = block.getData();
      runNumber = in.readInt();
      eventNumber = in.readInt();

      timeStamp = in.readLong();
      detectorName = in.readString();

      int nBlockNames = in.readInt();
      blockMap = new HashMap();
      for (int i = 0; i < nBlockNames; i++)
      {

	  // fg20030508 from v00-03 on, we ommit the trailing '\0's for strings
	  String blockName = in.readString() ;
	  String blockType = in.readString() ;

         blockMap.put(blockName, blockType);
      }
   }

   void readData(SIORecord record) throws IOException
   {
      for (;;)
      {
         SIOBlock block = record.getBlock();
         if (block == null)
            break;
	 //fg 20030509 versions older than v00-03 no longer supported
	 if( block.getMajorVersion() <1 && block.getMinorVersion() < 3)
	     throw new IOException("Sorry: files created with versions older than v00-03"
				   +" are no longer supported !" ) ;


         SIOInputStream in = block.getData();
         String name = block.getBlockName();
         String type = (String) blockMap.get(name);
         if (type == null)
            continue;
         if (type.equals(LCIO.MCPARTICLE))
         {
            int flags = in.readInt();
            int nMC = in.readInt();
            ILCCollection ilc = new ILCCollection(type, flags, nMC);
            for (int i = 0; i < nMC; i++)
               ilc.add(new SIOMCParticle(in));
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.SIMTRACKERHIT))
         {
            int flags = in.readInt();
            int n = in.readInt();
            ILCCollection ilc = new ILCCollection(type, flags, n);
            for (int i = 0; i < n; i++)
               ilc.add(new SIOSimTrackerHit(in));
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.SIMCALORIMETERHIT))
         {
            int flags = in.readInt();
            int n = in.readInt();
            ILCCollection ilc = new ILCCollection(type, flags, n);
            for (int i = 0; i < n; i++)
               ilc.add(new SIOSimCalorimeterHit(in, flags));
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.LCFLOATVEC))
         {
            int flags = in.readInt();
            int n = in.readInt();
            ILCCollection ilc = new ILCCollection(type, flags, n);
            for (int i = 0; i < n; i++)
               ilc.add(new SIOFloatVec(in));
            addCollection(ilc, name);
         }
      }
   }

   static void write(LCEvent event, SIOWriter writer, boolean headerOnly) throws IOException
   {
       // fg20030513 - changed back to tony's original version
       // we don't duplicate the event header in the C++ implementation any more ...
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
            out.writeString( blockName);
            out.writeString(event.getCollection(blockName).getTypeName());
         }
      }
      else
      {
         String[] blockNames = event.getCollectionNames();
         for (int j = 0; j < blockNames.length; j++)
         {
            String blockName = blockNames[j];
            SIOOutputStream out = writer.createBlock(blockName, LCIO.MAJORVERSION, LCIO.MINORVERSION);
            LCCollection col = event.getCollection(blockName);
            String type = col.getTypeName();
            int flags = col.getFlag();
            out.writeInt(flags);
 
            int n = col.getNumberOfElements();
            out.writeInt(n);
            if (type.equals(LCIO.MCPARTICLE))
            {
               for (int i = 0; i < n; i++)
                  SIOMCParticle.write((MCParticle) col.getElementAt(i), out);
            }
            else if (type.equals(LCIO.SIMTRACKERHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOSimTrackerHit.write((SimTrackerHit) col.getElementAt(i), out);
            }
            else if (type.equals(LCIO.SIMCALORIMETERHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOSimCalorimeterHit.write((SimCalorimeterHit) col.getElementAt(i), out, flags);
            }
            else if (type.equals(LCIO.LCFLOATVEC))
            {
               for (int i = 0; i < n; i++)
                  SIOFloatVec.write((ILCFloatVec) col.getElementAt(i), out);
            }
         }
      }
   }

   static void write(LCEvent event, SIOWriter writer) throws IOException
   {
      writer.createRecord(SIOFactory.eventHeaderRecordName, SIOFactory.compressionMode);
      SIOEvent.write(event, writer, true);
      writer.createRecord(SIOFactory.eventRecordName,SIOFactory.compressionMode);
      SIOEvent.write(event, writer, false);
   }

    /**@obsolete - fg20030508     
     */
   private static void hackString(SIOOutputStream out, String str) throws IOException
   {
      out.writeInt(str.length());

      byte[] ascii = str.getBytes();
      out.write(ascii);
      out.writeByte(0); // write irrelevant null
      out.pad();
   }


     

    /**@obsolete - fg20030508     
     */
   private static String stringHack(SIOInputStream in) throws IOException
   {
      int l = in.readInt();
      if (l > 32767)
         throw new IOException("String too long: " + l);

      byte[] ascii = new byte[l];
      in.readFully(ascii);
      in.readByte(); // skip irrelevant null
      in.pad();
      return new String(ascii); //BUG: what if default locale is not US-ASCII
   }
}
