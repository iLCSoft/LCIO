package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOReader;
import hep.lcd.io.sio.SIORecord;
import hep.lcd.io.sio.SIOBlock;

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
 * @version $Id: SIOLCReader.java,v 1.5 2003-09-04 01:16:40 tonyj Exp $
 */
class SIOLCReader implements LCReader
{
   private List eventListeners = new ArrayList();
   private List runListeners = new ArrayList();
   private SIOReader reader;

   public void close() throws IOException
   {
      reader.close();
   }

   public void open(String filename) throws IOException
   {
      reader = new SIOReader(new FileInputStream(filename));
   }

   public LCEvent readNextEvent() throws IOException
   {
      try
      {
         for (;;)
         {
            SIORecord record = reader.readRecord();
            String name = record.getRecordName();
            if (!name.equals(SIOFactory.eventHeaderRecordName))
               continue;

            SIOEvent event = new SIOEvent(record);
            event.readData(reader.readRecord());
            return event;
         }
      }
      catch (EOFException x)
      {
         return null;
      }
   }

   public LCEvent readNextEvent(int accessMode) throws IOException
   {
      //FIXME: What to do with access mode
      return readNextEvent();
   }

   public LCEvent readEvent(int runNumber, int evtNumber) throws IOException
   {
      //FIXME: need implementation
      return readNextEvent();
   }

   public LCRunHeader readNextRunHeader() throws IOException
   {
      try
      {
         for (;;)
         {
            SIORecord record = reader.readRecord();
            String name = record.getRecordName();
            if (!name.equals(SIOFactory.runRecordName))
               continue;

	    SIOBlock block = record.getBlock() ;
	    //fg 20030509 versions older than v00-03 no longer supported
	    if( block.getMajorVersion() <1 && block.getMinorVersion() < 3)
		throw new IOException("Sorry: files created with versions older than v00-03"
				      +" are no longer supported !" ) ;
	    
            return new SIORunHeader(block.getData());
         }
      }
      catch (EOFException x)
      {
         return null;
      }
   }
   public void readStream() throws IOException
   {
      readStream(-1);
   }
   public void readStream(int maxRecords) throws IOException
   {
      try
      {
         for (int nRead = 0; maxRecords < 0 || nRead < maxRecords; nRead++)
         {
            SIORecord record = reader.readRecord();
            if (record.getRecordName().equals(SIOFactory.runRecordName))
            {
               int l = runListeners.size();
               if (l > 0)
               {
		   SIOBlock block = record.getBlock() ;
		   //fg 20030509 versions older than v00-03 no longer supported
		   if( block.getMajorVersion() <1 && block.getMinorVersion() < 3)
		       throw new IOException("Sorry: files created with versions older than v00-03"
					     +" are no longer supported !" ) ;

                  SIORunHeader header = new SIORunHeader(block.getData());
                  for (int i = 0; i < l; i++)
                     ((LCRunListener) runListeners.get(i)).analyze(header);
               }
            }
            else if (record.getRecordName().equals(SIOFactory.eventHeaderRecordName))
            {
               int l = eventListeners.size();
               if (l > 0)
               {
                  SIOEvent event = new SIOEvent(record);
                  event.readData(reader.readRecord());
                  for (int i = 0; i < l; i++)
                     ((LCEventListener) eventListeners.get(i)).update(event);
               }
            }
         }
      }
      catch (EOFException x)
      {
         if (maxRecords < 0) return;
         else throw x;
      }
   }

   public void registerLCEventListener(LCEventListener ls)
   {
      eventListeners.add(ls);
   }

   public void registerLCRunListener(LCRunListener ls)
   {
      runListeners.add(ls);
   }

   public void removeLCEventListener(LCEventListener ls)
   {
      eventListeners.remove(ls);
   }

   public void removeLCRunListener(LCRunListener ls)
   {
      runListeners.remove(ls);
   }
}
