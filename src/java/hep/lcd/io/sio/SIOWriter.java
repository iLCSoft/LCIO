/*
 * SIOWriter.java
 *
 * Created on March 7, 2002, 6:32 PM
 */

package hep.lcd.io.sio;
import hep.io.xdr.*;
import java.io.*;
import java.util.zip.*;

/**
 *
 * @author  tonyj
 */
public class SIOWriter
{
   private XDROutputStream xdr;
   private ByteArrayOutputStream blockBytes = new ByteArrayOutputStream();
   private ByteArrayOutputStream recordBytes = new ByteArrayOutputStream();
   private SIOOutputStream block = new SIOOutputStream(blockBytes);
   private XDROutputStream record = new XDROutputStream(recordBytes);
   private DeflaterOutputStream compressor;
   private final int recordFrame = 0xabadcafe;
   private final int blockFrame = 0xdeadbeef;
   
   private String blockName;
   private int blockVersion;
   private String recordName;
   private boolean recordCompress = false;
   
   /** Creates a new instance of SIOWriter */
   public SIOWriter(OutputStream out) throws IOException
   {
      this.xdr = new XDROutputStream(out);
   }
   public void createRecord(String name, boolean compress) throws IOException
   {
      flushRecord();
      recordName = name;
      if (compress)
      {
         compressor = new DeflaterOutputStream(recordBytes);
         record = new XDROutputStream(compressor);
      }
      else
      {
         compressor = null;
         record = new XDROutputStream(recordBytes);
      }
      recordCompress = compress;
   }
   public SIOOutputStream createBlock(String name, int major, int minor) throws IOException
   {
      flushBlock();
      blockName = name;
      blockVersion = (major << 16) + minor;
      return block;
   }
   private void flushBlock() throws IOException
   {
      if (blockName == null) return;
      block.flush();
      int blockLength = pad(blockBytes.size()) + 16 + pad(blockName.length());
      record.writeInt(blockLength);
      record.writeInt(blockFrame);
      record.writeInt(blockVersion);
      record.writeString(blockName);
      blockBytes.writeTo(record);
      blockBytes.reset();
      blockName = null;
   }
   private void flushRecord() throws IOException
   {
      if (recordName == null) return;
      flushBlock();
      block.clear();
      record.flush();
      if (recordCompress) compressor.finish();
      //      int headerLength = pad(recordBytes.size()) + 24 + pad(recordName.length());
      // fg20030508 - header length doesn't include the header data
      int headerLength = 24 + pad(recordName.length());
      // Write the record header
      xdr.writeInt(headerLength);
		xdr.writeInt(recordFrame);
		xdr.writeInt(recordCompress ? 1 : 0);
		xdr.writeInt(recordBytes.size());
		xdr.writeInt((int) record.getBytesWritten());
		xdr.writeString(recordName);
		recordBytes.writeTo(xdr);
      recordBytes.reset();
      xdr.pad();
      recordName = null;
   }
   public void close() throws IOException
   {
      flushRecord();
      xdr.close();
   }
   private int pad(int size)
   {
      int r = size % 4;
      if (r == 0) return size;
      return size + 4 - r;
   }
}
