package hep.lcio.implementation.sio;

import hep.io.sio.SIOOutputStream;
import hep.io.sio.SIOWriter;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCRunHeader;
import hep.lcio.event.LCIO;
import hep.lcio.io.LCWriter;
import java.io.IOException;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOLCWriter.java,v 1.15 2010-06-22 13:49:56 gaede Exp $
 */
class SIOLCWriter implements LCWriter
{
   private SIOWriter writer;
   private boolean randomAccess = true;
   private RandomAccessBlock fileRandomAccessBlock;
   private IndexBlock indexBlock;
   private RandomAccessBlock previousRandomAccessBlock;

   public void close() throws IOException
   {
      flush();
      fileRandomAccessBlock.write(writer);
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
      writer = new SIOWriter(filename);

      //FIXME: Deal with append option
      if (randomAccess)
      {
         fileRandomAccessBlock = new RandomAccessBlock();
         //FIXME: Set more sensible limit
         indexBlock = new IndexBlock(5000);
      }
   }

   public void setCompressionLevel(int level) {
	    // not yet ...
   }

   public void writeEvent(LCEvent evt) throws IOException
   {
      long recordLocation = SIOEvent.write(evt, writer);
      if (randomAccess)
      {
         boolean isFull = indexBlock.add(recordLocation,evt);
         if (isFull) flush();
      }
   }

   public void writeRunHeader(LCRunHeader hdr) throws IOException
   {
      long recordLocation = writer.createRecord(SIOFactory.runRecordName, SIOFactory.compressionMode);

      SIOOutputStream out = writer.createBlock(SIOFactory.runBlockName, LCIO.MAJORVERSION, LCIO.MINORVERSION);
      SIORunHeader.write(hdr, out);
      if (randomAccess) {
         boolean isFull = indexBlock.add(recordLocation,hdr);
         if (isFull) flush();
      }
   }

   public void flush() throws IOException
   {
      if (randomAccess)
      {
         if (!indexBlock.isEmpty())
         {
            indexBlock.write(writer);
            RandomAccessBlock randomAccessBlock = new RandomAccessBlock();
            randomAccessBlock.setIndexBlock(indexBlock);
            if (previousRandomAccessBlock != null) randomAccessBlock.setPrevious(previousRandomAccessBlock);
            randomAccessBlock.write(writer);

            if (previousRandomAccessBlock != null) {
               previousRandomAccessBlock.setNext(randomAccessBlock);
               previousRandomAccessBlock.flush();
            }
            previousRandomAccessBlock = randomAccessBlock;
            
            fileRandomAccessBlock.add(randomAccessBlock);
            indexBlock.clear();
         }
      }
      writer.flush();
   }
}
