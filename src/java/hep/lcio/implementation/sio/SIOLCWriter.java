package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIOWriter;

import hep.lcio.data.LCEventData;
import hep.lcio.data.LCRunHeaderData;

//import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;

//import hep.lcio.event.LCRunHeader;
import hep.lcio.io.LCWriter;

import java.io.FileOutputStream;
import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOLCWriter.java,v 1.6 2003-09-04 04:27:00 tonyj Exp $
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
      // FIX ME - what is the default behavior 
      // for existing files ?
      writer = new SIOWriter(new FileOutputStream(filename));
   }

   public void open(String filename, int writeMode) throws IOException
   {
      // FIX ME - need implementation for append mode
      throw new IOException("append not yet supported");
   }

   public void writeEvent(LCEventData evt) throws IOException
   {
      SIOEvent.write(evt, writer);
   }

   public void writeRunHeader(LCRunHeaderData hdr) throws IOException
   {
      writer.createRecord(SIOFactory.runRecordName, SIOFactory.compressionMode);

      SIOOutputStream out = writer.createBlock(SIOFactory.runBlockName, LCIO.MAJORVERSION, LCIO.MINORVERSION);
      SIORunHeader.write(hdr, out);
   }
}
