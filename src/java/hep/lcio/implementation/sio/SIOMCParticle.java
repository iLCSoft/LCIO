package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;

import hep.lcio.data.MCParticleData;

import hep.lcio.event.MCParticle;

import hep.lcio.implementation.event.IMCParticle;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOMCParticle.java,v 1.9 2003-11-08 03:08:51 tonyj Exp $
 */
class SIOMCParticle extends IMCParticle
{
   private List temp = new ArrayList();
   SIOMCParticle(SIOInputStream in, SIOEvent owner, int major, int minor) throws IOException
   {      
      boolean hasEndPoint = false;
      setParent(owner);
      in.readPTag(this);
      
      if (minor == 8)
      {
         in.readPntr();
         in.readPntr();

         int nDaughters = in.readInt();
         for (int i = 0; i < nDaughters; i++)
            temp.add(in.readPntr());
      }
      else
      {
         int nParents = in.readInt();
         for (int i=0; i<nParents; i++)
         {
            temp.add(in.readPntr());
         }
      }
      pdg = in.readInt();
      generatorStatus = in.readInt();
      if (minor > 8) 
      {
         simulatorStatus = in.readInt();
         hasEndPoint = (simulatorStatus & (1<<31)) != 0;
      }
      vertex[0] = in.readDouble();
      vertex[1] = in.readDouble();
      vertex[2] = in.readDouble();
      momentum[0] = in.readFloat();
      momentum[1] = in.readFloat();
      momentum[2] = in.readFloat();
      mass = in.readFloat();
      charge = in.readFloat();
      if (hasEndPoint)
      {
         endpoint[0] = in.readDouble();
         endpoint[1] = in.readDouble();
         endpoint[2] = in.readDouble();
      }
   }
   void resolve(int major, int minor)
   {
      if (minor == 8)
      {
         for (int i=0; i<parents.size(); i++)
         {
            IMCParticle daughter = (IMCParticle) ((SIORef) parents.get(i)).getObject();
            this.addDaughter(daughter);
         }
      }
      else
      {
         for (int i=0; i<parents.size(); i++)
         {
            IMCParticle parent = (IMCParticle) ((SIORef) parents.get(i)).getObject();
            parent.addDaughter(this);
         }
      }
      temp = null;
   }
   static void write(MCParticleData hit, SIOOutputStream out) throws IOException
   {
      if (hit instanceof SIOMCParticle)
         ((SIOMCParticle) hit).write(out);
      else
      {
         out.writePTag(hit);
         int nParents = hit.getNumberOfParents();
         out.writeInt(nParents);

         for (int i = 0; i < nParents; i++)
         {
            out.writePntr(hit.getParentData(i));
         }

         out.writeInt(hit.getPDG());
         out.writeInt(hit.getGeneratorStatus());
         int simulatorStatus = hit.getSimulatorStatus();
         out.writeInt(simulatorStatus);

         double[] vertex = hit.getVertex();
         out.writeDouble(vertex[0]);
         out.writeDouble(vertex[1]);
         out.writeDouble(vertex[2]);

         float[] momentum = hit.getMomentum();
         out.writeFloat(momentum[0]);
         out.writeFloat(momentum[1]);
         out.writeFloat(momentum[2]);
         out.writeFloat(hit.getMass());
         out.writeFloat(hit.getCharge());

         if ((simulatorStatus & (1<<31)) != 0)
         {
            double[] endpoint = hit.getEndpoint();
            out.writeDouble(endpoint[0]);
            out.writeDouble(endpoint[1]);
            out.writeDouble(endpoint[2]);
         }
      }
   }

   private void write(SIOOutputStream out) throws IOException
   {
      out.writePTag(this);
      out.writeInt(parents.size());
      for (int i = 0; i < parents.size(); i++)
         out.writePntr(getDaughter(i));
      out.writeInt(pdg);
      out.writeInt(generatorStatus);
      out.writeInt(simulatorStatus);
      out.writeDouble(vertex[0]);
      out.writeDouble(vertex[1]);
      out.writeDouble(vertex[2]);
      out.writeFloat(momentum[0]);
      out.writeFloat(momentum[1]);
      out.writeFloat(momentum[2]);
      out.writeFloat(mass);
      out.writeFloat(charge);

      if ((simulatorStatus & (1<<31)) != 0)
      {
         out.writeDouble(endpoint[0]);
         out.writeDouble(endpoint[1]);
         out.writeDouble(endpoint[2]);
      }
   }
}
