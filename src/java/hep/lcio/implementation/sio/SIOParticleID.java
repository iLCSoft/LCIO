package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;

import hep.lcio.event.ParticleID;
import hep.lcio.implementation.event.IParticleID;

import java.io.IOException;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOParticleID.java,v 1.6 2004-11-03 16:30:24 tonyj Exp $
 */
class SIOParticleID extends IParticleID
{
   SIOParticleID(SIOInputStream in, SIOEvent owner, int flag, int major, int minor) throws IOException
   {
      setParent(owner);
      likelihood = in.readFloat();
      type = in.readInt();
      pdg = in.readInt();
      algorithmType =in.readInt() ;
      int n = in.readInt();
      parameters = new float[n];
      for (int i=0; i<n; i++) parameters[i] = in.readInt();
   }

   static void write(ParticleID id, SIOOutputStream out) throws IOException
   {
      if (id instanceof SIOParticleID)
      {
         ((SIOParticleID) id).write(out);
      }
      else
      {
         out.writeFloat(id.getLikelihood());
         out.writeInt(id.getType());
         out.writeInt(id.getPDG());
         out.writeInt(id.getAlgorithmType());
         float[] pars = id.getParameters();
         int n = pars == null ? 0 : pars.length;
         out.writeInt(n);
         for (int i=0; i<n; i++) out.writeFloat(pars[i]);
      }
   }

   private void write(SIOOutputStream out) throws IOException
   {
      out.writeFloat(likelihood);
      out.writeInt(type);
      out.writeInt(pdg);
      out.writeInt(algorithmType);
      int n = parameters == null ? 0 : parameters.length;
      out.writeInt(n);
      for (int i=0; i<n; i++) out.writeFloat(parameters[i]);
   }
}
