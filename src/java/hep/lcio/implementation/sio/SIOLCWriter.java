package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIOWriter;

import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.LCRunHeader;

import hep.lcio.io.LCWriter;

import java.io.FileOutputStream;
import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOLCWriter.java,v 1.3 2003-05-09 15:16:45 gaede Exp $
 */
class SIOLCWriter implements LCWriter
{
   private SIOWriter writer;

   public int close()
   {
      try
      {
         writer.close();
         return LCIO.SUCCESS;
      }
      catch (IOException x)
      {
         throw new LCIOException(x);
      }
   }

   public int open(String filename)
   {
      try
      {
         writer = new SIOWriter(new FileOutputStream(filename));
         return LCIO.SUCCESS;
      }
      catch (IOException x)
      {
         throw new LCIOException(x);
      }
   }

   public int writeEvent(LCEvent evt)
   {
      try
      {
         SIOEvent.write(evt, writer);
         return LCIO.SUCCESS;
      }
      catch (IOException x)
      {
         throw new LCIOException(x);
      }
   }

   public int writeRunHeader(LCRunHeader hdr)
   {
      try
      {
         writer.createRecord(SIOFactory.runRecordName, SIOFactory.compressionMode);

         SIOOutputStream out = writer.createBlock(SIOFactory.runBlockName, LCIO.MAJORVERSION, LCIO.MINORVERSION);
         SIORunHeader.write(hdr, out);
         return LCIO.SUCCESS;
      }
      catch (IOException x)
      {
         throw new LCIOException(x);
      }
   }
}
