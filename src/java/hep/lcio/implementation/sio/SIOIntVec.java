package hep.lcio.implementation.sio;

import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;

import hep.lcio.event.LCIntVec;

import hep.lcio.implementation.event.ILCIntVec;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOIntVec.java,v 1.6 2007-11-07 20:46:22 jeremy Exp $
 */
class SIOIntVec extends ILCIntVec
{
   SIOIntVec(SIOInputStream in, SIOEvent owner,int major,int minor) throws IOException
   {
      setParent(owner);
      size = in.readInt();
      data = new int[size];
      for (int i = 0; i < size; i++)
         data[i] = in.readInt();
      if(SIOVersion.encode(major,minor)>SIOVersion.encode(1,2)) in.readPTag(this);
   }
   
   static void write(LCIntVec vec, SIOOutputStream out) throws IOException
   {
      if (vec instanceof SIOIntVec)
         ((SIOIntVec) vec).write(out);
      else
      {
         int[] data = vec.toArray();
         out.writeInt(data.length);
         for (int i = 0; i < data.length; i++)
            out.writeInt(data[i]);
         out.writePTag(vec);
      }
   }
   
   private void write(SIOOutputStream out) throws IOException
   {
      out.writeInt(size);
      for (int i = 0; i < size; i++)
         out.writeInt(data[i]);
      out.writePTag(this) ;
   }
}
