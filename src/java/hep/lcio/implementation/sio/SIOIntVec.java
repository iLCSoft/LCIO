package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;

import hep.lcio.event.LCIntVec;

import hep.lcio.implementation.event.ILCIntVec;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOIntVec.java,v 1.3 2004-04-08 09:58:02 gaede Exp $
 */
class SIOIntVec extends ILCIntVec
{
   SIOIntVec(SIOInputStream in, SIOEvent owner) throws IOException
   {
      setParent(owner);
      size = in.readInt();
      data = new int[size];
      for (int i = 0; i < size; i++)
         data[i] = in.readInt();
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
      }
   }

   private void write(SIOOutputStream out) throws IOException
   {
      out.writeInt(size);
      for (int i = 0; i < size; i++)
         out.writeInt(data[i]);
   }
}
