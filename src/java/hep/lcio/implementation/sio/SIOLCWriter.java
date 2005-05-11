package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIOWriter;

import hep.lcio.event.LCEvent;
import hep.lcio.event.LCRunHeader;

//import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;

//import hep.lcio.event.LCRunHeader;
import hep.lcio.io.LCWriter;

import java.io.FileOutputStream;
import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOLCWriter.java,v 1.12 2005-05-11 01:06:42 tonyj Exp $
 */
class SIOLCWriter implements LCWriter
{
   private SIOWriter writer;

   public void close() throws IOException
   {
      writer.close();
   }

   public void open(String filename) throws IOException
   {
      open(filename,LCIO.WRITE_NEW);
   }

   public void open(String filename, int writeMode) throws IOException
   {
      boolean append = writeMode == LCIO.WRITE_APPEND;
      if (!filename.endsWith(".slcio")) filename += ".slcio";
      writer = new SIOWriter(new FileOutputStream(filename,append));
   }

   public void writeEvent(LCEvent evt) throws IOException
   {
      SIOEvent.write(evt, writer);
   }

   public void writeRunHeader(LCRunHeader hdr) throws IOException
   {
      writer.createRecord(SIOFactory.runRecordName, SIOFactory.compressionMode);

      SIOOutputStream out = writer.createBlock(SIOFactory.runBlockName, LCIO.MAJORVERSION, LCIO.MINORVERSION);
      SIORunHeader.write(hdr, out);
   }

   public void flush() throws IOException
   {
      writer.flush();
   }
}
