package hep.lcio.implementation.sio;

import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;

import hep.lcio.event.LCStrVec;

import hep.lcio.implementation.event.ILCStrVec;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOStrVec.java,v 1.4 2007-11-07 20:46:23 jeremy Exp $
 */
class SIOStrVec extends ILCStrVec
{
   SIOStrVec(SIOInputStream in, SIOEvent owner,int major,int minor) throws IOException
   {
      setParent(owner);
      size = in.readInt();
      data = new String[size];
      for (int i = 0; i < size; i++)
         data[i] = in.readString();
      
      if(SIOVersion.encode(major,minor)>SIOVersion.encode(1,2)) in.readPTag(this);
   }
   
   static void write(LCStrVec vec, SIOOutputStream out) throws IOException
   {
      if (vec instanceof SIOStrVec)
         ((SIOStrVec) vec).write(out);
      else
      {
         String[] data = vec.toArray();
         out.writeInt(data.length);
         for (int i = 0; i < data.length; i++)
            out.writeString(data[i]);
         out.writePTag(vec) ;
      }
   }
   
   private void write(SIOOutputStream out) throws IOException
   {
      out.writeInt(size);
      for (int i = 0; i < size; i++)
         out.writeString(data[i]);
      out.writePTag(this) ;
   }
}
