package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;

import hep.lcio.event.LCObject;
import hep.lcio.event.TrackerHit;

import hep.lcio.implementation.event.ITrackerHit;

import java.io.IOException;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOTrackerHit.java,v 1.6 2004-09-13 18:02:57 tonyj Exp $
 */
class SIOTrackerHit extends ITrackerHit
{
   SIOTrackerHit(SIOInputStream in, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);
      for (int i = 0; i < 3; i++)
         position[i] = in.readDouble();
      for (int i = 0; i < 6; i++)
         covMatrix[i] = in.readFloat();
      dEdx = in.readFloat();
      time = in.readFloat();
      rawDataHit = in.readPntr();
      in.readPTag(this);
   }

   public LCObject getRawDataHit()
   {
      if (rawDataHit instanceof SIORef)
      {
         rawDataHit = ((SIORef) rawDataHit).getObject();
      }

      return (LCObject) rawDataHit;
   }

   static void write(TrackerHit hit, SIOOutputStream out) throws IOException
   {
      if (hit instanceof SIOTrackerHit)
      {
         ((SIOTrackerHit) hit).write(out);
      }
      else
      {
         double[] pos = hit.getPosition();
         for (int i = 0; i < 3; i++)
            out.writeDouble(pos[i]);

         float[] matrix = hit.getCovMatrix();
         for (int i = 0; i < 6; i++)
            out.writeFloat(matrix[i]);
         out.writeFloat(hit.getdEdx());
         out.writeFloat(hit.getTime());
         out.writePntr(hit.getRawDataHit());
         out.writePTag(hit);
      }
   }

   private void write(SIOOutputStream out) throws IOException
   {
      for (int i = 0; i < 3; i++)
         out.writeDouble(position[i]);
      for (int i = 0; i < 6; i++)
         out.writeFloat(covMatrix[i]);
      out.writeFloat(dEdx);
      out.writeFloat(time);
      out.writePntr(rawDataHit);
      out.writePTag(this);
   }
}
