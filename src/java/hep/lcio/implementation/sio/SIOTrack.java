package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcio.implementation.event.ITrack;
import java.io.IOException;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOTrack.java,v 1.1 2004-05-24 03:37:41 tonyj Exp $
 */
class SIOTrack extends ITrack
{
   SIOTrack(SIOInputStream in, SIOEvent owner, int flag, int major, int minor) throws IOException
   {
      type = in.readInt();
      momentum = in.readFloat();
      theta = in.readFloat();
      phi = in.readFloat();
      d0 = in.readFloat();
      z0 = in.readFloat();
      covMatrix = new float[15];
      for (int i=0; i<covMatrix.length; i++) covMatrix[i] = in.readFloat();
      referencePoint = new float[3];
      referencePoint[0] = in.readFloat();
      referencePoint[1] = in.readFloat();
      referencePoint[2] = in.readFloat();
      chi2 = in.readFloat();
      dEdx = in.readFloat();
      dEdxError = in.readFloat();
      int nTracks = in.readInt();
      for (int i=0; i<nTracks; i++)
      {
         in.readPntr(); // FixMe:
         if ((flag & 1<<31) != 0)
         {
            int nHits = in.readInt();
            in.readPntr(); // Fixme;
         }
      }
      in.readPTag(this);
   }
   static void write(ITrack track, SIOOutputStream out, int flag) throws IOException
   {
      if (track instanceof SIOTrack)
         ((SIOTrack) track).write(out,flag);
      else
      {
         out.writeInt(track.getType());
         out.writeFloat(track.getMomentum());
         out.writeFloat(track.getTheta());
         out.writeFloat(track.getPhi());
         out.writeFloat(track.getD0());
         out.writeFloat(track.getZ0());
         float[] covMatrix = track.getCovMatrix();
         for (int i=0; i<covMatrix.length; i++) out.writeFloat(covMatrix[i]);
         float[] referencePoint = track.getReferencePoint();
         out.writeFloat(referencePoint[0]);
         out.writeFloat(referencePoint[1] );
         out.writeFloat(referencePoint[2]);
         out.writeFloat(track.getChi2());
         out.writeFloat(track.getdEdx());
         out.writeFloat(track.getdEdxError());
         out.writeInt(0); // Fixme:
         out.writePTag(track);
      }
   }
   private void write(SIOOutputStream out, int flag) throws IOException
   {
      out.writeInt(type);
      out.writeFloat(momentum);
      out.writeFloat(theta);
      out.writeFloat(phi);
      out.writeFloat(d0);
      out.writeFloat(z0);
      for (int i=0; i<covMatrix.length; i++) out.writeFloat(covMatrix[i]);
      out.writeFloat(referencePoint[0]);
      out.writeFloat(referencePoint[1] );
      out.writeFloat(referencePoint[2]);
      out.writeFloat(chi2);
      out.writeFloat(dEdx);
      out.writeFloat(dEdxError);
      out.writeInt(0); // Fixme:
      out.writePTag(this);
   }
}
