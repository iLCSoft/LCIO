package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;

import hep.lcio.data.LCFloatVec;

import hep.lcio.implementation.event.ILCFloatVec;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOFloatVec.java,v 1.6 2003-09-04 04:27:00 tonyj Exp $
 */
class SIOFloatVec extends ILCFloatVec
{
   SIOFloatVec(SIOInputStream in) throws IOException
   {
      size = in.readInt();
      data = new float[size];
      for (int i = 0; i < size; i++)
         data[i] = in.readFloat();
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
      }
   }

   private void write(SIOOutputStream out) throws IOException
   {
      out.writeInt(size);
      for (int i = 0; i < size; i++)
         out.writeFloat(data[i]);
   }
}
