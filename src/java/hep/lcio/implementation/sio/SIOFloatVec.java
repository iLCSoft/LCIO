package hep.lcio.implementation.sio;

import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;

import hep.lcio.event.LCFloatVec;

import hep.lcio.implementation.event.ILCFloatVec;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOFloatVec.java,v 1.11 2007-11-07 20:46:22 jeremy Exp $
 */
class SIOFloatVec extends ILCFloatVec
{
   SIOFloatVec(SIOInputStream in, SIOEvent owner,int major,int minor) throws IOException
   {
      setParent(owner);
      size = in.readInt();
      data = new float[size];
      for (int i = 0; i < size; i++)
         data[i] = in.readFloat();
      if(SIOVersion.encode(major,minor)>SIOVersion.encode(1,2)) in.readPTag(this);
   }
   
   static void write(LCFloatVec vec, SIOOutputStream out) throws IOException
   {
      if (vec instanceof SIOFloatVec)
         ((SIOFloatVec) vec).write(out);
      else
      {
         float[] data = vec.toArray();
         out.writeInt(data.length);
         for (int i = 0; i < data.length; i++)
            out.writeFloat(data[i]);
         out.writePTag(vec) ;
      }
   }
   
   private void write(SIOOutputStream out) throws IOException
   {
      out.writeInt(size);
      for (int i = 0; i < size; i++)
         out.writeFloat(data[i]);
      out.writePTag(this) ;
   }
}
