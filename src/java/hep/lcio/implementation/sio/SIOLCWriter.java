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
 * @version $Id: SIOLCWriter.java,v 1.2 2003-05-06 06:22:12 tonyj Exp $
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
         writer.createRecord(SIOFactory.runRecordName, true);

         SIOOutputStream out = writer.createBlock(LCIO.LCRUNHEADER, LCIO.MAJORVERSION, LCIO.MINORVERSION);
         SIORunHeader.write(hdr, out);
         return LCIO.SUCCESS;
      }
      catch (IOException x)
      {
         throw new LCIOException(x);
      }
   }
}
