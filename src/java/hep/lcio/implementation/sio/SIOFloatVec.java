package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;

import hep.lcio.event.LCFloatVec;

import hep.lcio.implementation.event.ILCFloatVec;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOFloatVec.java,v 1.4 2003-05-06 17:26:46 tonyj Exp $
 */
class SIOFloatVec extends ILCFloatVec
{
   SIOFloatVec(SIOInputStream in) throws IOException 
   {
     size = in.readInt();
     data = new float[size];
     for(int i=0; i<size; i++) data[i] = in.readFloat();
   }
   private void write(SIOOutputStream out) throws IOException
   {
     out.writeInt(size);
     for(int i=0; i<size; i++) out.writeFloat(data[i]);
   }
   static void write(LCFloatVec vec, SIOOutputStream out) throws IOException
   {
      if (vec instanceof SIOFloatVec)
         ((SIOFloatVec) vec).write(out);
      else 
      {
         float[] data = vec.toArray();
         out.writeInt(data.length);
         for(int i=0; i<data.length; i++) out.writeFloat(data[i]);        
      }
   }
}
