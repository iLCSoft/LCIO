package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;

import hep.lcio.event.MCParticle;
import hep.lcio.event.TrackerHit;

import hep.lcio.implementation.event.ITrackerHit;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOTrackerHit.java,v 1.2 2003-05-06 06:22:12 tonyj Exp $
 */
public class SIOTrackerHit extends ITrackerHit
{
   SIOTrackerHit(SIOInputStream in) throws IOException
   {
      cellID = in.readInt();
      position[0] = in.readDouble();
      position[1] = in.readDouble();
      position[2] = in.readDouble();
      dEdx = in.readFloat();
      time = in.readFloat();
      particle = in.readPntr();
   }

   public MCParticle getMCParticle()
   {
      if (particle instanceof SIORef)
         particle = ((SIORef) particle).getObject();
      return (MCParticle) particle;
   }

   static void write(TrackerHit hit, SIOOutputStream out) throws IOException
   {
      if (hit instanceof SIOTrackerHit)
         ((SIOTrackerHit) hit).write(out);
      else
      {
         out.writeInt(hit.getCellID());

         double[] pos = hit.getPosition();
         out.writeDouble(pos[0]);
         out.writeDouble(pos[1]);
         out.writeDouble(pos[2]);
         out.writeFloat(hit.getdEdx());
         out.writeFloat(hit.getTime());
         out.writePntr(hit.getMCParticle());
      }
   }

   private void write(SIOOutputStream out) throws IOException
   {
      out.writeInt(cellID);
      out.writeDouble(position[0]);
      out.writeDouble(position[1]);
      out.writeDouble(position[2]);
      out.writeFloat(dEdx);
      out.writeFloat(time);
      out.writePntr(particle);
   }
}
