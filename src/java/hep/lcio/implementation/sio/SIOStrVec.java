package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;

import hep.lcio.event.LCStrVec;

import hep.lcio.implementation.event.ILCStrVec;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOStrVec.java,v 1.1 2004-07-02 08:46:45 hvogt Exp $
 */
class SIOStrVec extends ILCStrVec
{
   SIOStrVec(SIOInputStream in, SIOEvent owner) throws IOException
   {
      setParent(owner);
      size = in.readInt();
      data = new String[size];
      for (int i = 0; i < size; i++)
         data[i] = in.readString();
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
      }
   }

   private void write(SIOOutputStream out) throws IOException
   {
      out.writeInt(size);
      for (int i = 0; i < size; i++)
         out.writeString(data[i]);
   }
}
