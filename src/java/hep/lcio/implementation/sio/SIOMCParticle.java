package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;

import hep.lcio.data.MCParticleData;
import hep.lcio.event.MCParticle;

import hep.lcio.implementation.event.IMCParticle;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOMCParticle.java,v 1.4 2003-06-10 13:10:43 gaede Exp $
 */
class SIOMCParticle extends IMCParticle
{
   SIOMCParticle(SIOInputStream in) throws IOException
   {
      in.readPTag(this);
      parent = in.readPntr();
      secondParent = in.readPntr();

      int nDaughters = in.readInt();
      daughters = new SIORef[nDaughters];
      for (int i = 0; i < nDaughters; i++)
         daughters[i] = in.readPntr();
      pdg = in.readInt();
      hepEvtStatus = in.readInt();
      vertex[0] = in.readDouble();
      vertex[1] = in.readDouble();
      vertex[2] = in.readDouble();
      momentum[0] = in.readFloat();
      momentum[1] = in.readFloat();
      momentum[2] = in.readFloat();
      energy = in.readFloat();
      charge = in.readFloat();
      if(nDaughters == 0 ){
        endpoint[0] = in.readDouble();
        endpoint[1] = in.readDouble();
        endpoint[2] = in.readDouble();
      }
   }

   public MCParticle[] getDaughters()
   {
      if (!(daughters instanceof MCParticle[]))
      {
         for (int i = 0; i < daughters.length; i++)
            daughters[i] = ((SIORef) daughters[i]).getObject();
      }
      return (MCParticle[]) daughters;
   }

   public MCParticle getParent()
   {
      if (parent instanceof SIORef)
         parent = ((SIORef) parent).getObject();
      return (MCParticle) parent;
   }

   public MCParticle getSecondParent()
   {
      if (secondParent instanceof SIORef)
         secondParent = ((SIORef) secondParent).getObject();
      return (MCParticle) secondParent;
   }

   static void write(MCParticleData hit, SIOOutputStream out) throws IOException
   {
      if (hit instanceof SIOMCParticle)
         ((SIOMCParticle) hit).write(out);
      else
      {
         out.writePTag(hit);
         out.writePntr(hit.getParentData());
         out.writePntr(hit.getSecondParentData());

         int nDaughters = hit.getNumberOfDaughters() ;
         out.writeInt(nDaughters) ;
         
         for (int i = 0; i < nDaughters; i++) {
		     out.writePntr( hit.getDaughterData(i)) ;
		 }
         //MCParticle[] daughters = hit.getDaughters();
         //out.writeInt(daughters.length);
         // for (int i = 0; i < daughters.length; i++)
         // out.writePntr(daughters[i]);

         out.writeInt(hit.getPDG());
         out.writeInt(hit.getHepEvtStatus());

         double[] vertex = hit.getVertex();
         out.writeDouble(vertex[0]);
         out.writeDouble(vertex[1]);
         out.writeDouble(vertex[2]);

         float[] momentum = hit.getMomentum();
         out.writeFloat(momentum[0]);
         out.writeFloat(momentum[1]);
         out.writeFloat(momentum[2]);
         out.writeFloat(hit.getEnergy());
         out.writeFloat(hit.getCharge());

		// write endpoints only if particle has no daughters
		if( nDaughters == 0 ){
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
      out.writePntr(parent);
      out.writePntr(secondParent);
      out.writeInt(daughters.length);
      for (int i = 0; i < daughters.length; i++)
         out.writePntr(daughters[i]);
      out.writeInt(pdg);
      out.writeInt(hepEvtStatus);
      out.writeDouble(vertex[0]);
      out.writeDouble(vertex[1]);
      out.writeDouble(vertex[2]);
      out.writeFloat(momentum[0]);
      out.writeFloat(momentum[1]);
      out.writeFloat(momentum[2]);
      out.writeFloat(energy);
      out.writeFloat(charge);
		// write endpoints only if particle has no daughters
		if( daughters.length == 0 ){
           out.writeDouble(endpoint[0]);
           out.writeDouble(endpoint[1]);
           out.writeDouble(endpoint[2]);
		}

   }
}
