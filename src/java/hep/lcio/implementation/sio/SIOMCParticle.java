package hep.lcio.implementation.sio;

import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;
import hep.io.sio.SIORef;


import hep.lcio.event.MCParticle;

import hep.lcio.implementation.event.IMCParticle;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOMCParticle.java,v 1.18 2007-11-07 20:46:23 jeremy Exp $
 */
class SIOMCParticle extends IMCParticle
{

   private List temp = new ArrayList();
   SIOMCParticle(SIOInputStream in, SIOEvent owner, int major, int minor) throws IOException
   {      
      boolean hasEndPoint = false;
      setParent(owner);
      in.readPTag(this);
      boolean is0Dot8 = major==0 && minor == 8;
      
      if (is0Dot8)
      {
         in.readPntr();
         in.readPntr();

         int nDaughters = in.readInt();
         for (int i = 0; i < nDaughters; i++)
            temp.add(in.readPntr());
         
         hasEndPoint = (nDaughters == 0);
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
      if (!is0Dot8) 
      {
         simulatorStatus = in.readInt();
         hasEndPoint = (simulatorStatus & (1<<31)) != 0;
      }
      vertex[0] = in.readDouble();
      vertex[1] = in.readDouble();
      vertex[2] = in.readDouble();
      
      if( SIOVersion.encode(major,minor) > SIOVersion.encode(1,2) ){
         time = in.readFloat() ;
      }

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
         
         if( SIOVersion.encode(major,minor) > SIOVersion.encode(2,6)) {
	   momentumAtEndpoint[0] = in.readFloat();
	   momentumAtEndpoint[1] = in.readFloat();
	   momentumAtEndpoint[2] = in.readFloat();
         }  
       }
      if( SIOVersion.encode(major,minor) > SIOVersion.encode(1,51) ){

          spin[0] = in.readFloat();
          spin[1] = in.readFloat();
          spin[2] = in.readFloat();

          colorFlow[0] = in.readInt();
          colorFlow[1] = in.readInt();
      }
   }
   void resolve(int major, int minor)
   {
      boolean is0Dot8 = major==0 && minor == 8;
      if (is0Dot8)
      {
         for (int i=0; i<temp.size(); i++)
         {
            IMCParticle daughter = (IMCParticle) ((SIORef) temp.get(i)).getObject();
            this.addDaughter(daughter);
         }
      }
      else
      {
         for (int i=0; i<temp.size(); i++)
         {
            IMCParticle parent = (IMCParticle) ((SIORef) temp.get(i)).getObject();
            parent.addDaughter(this);
         }
      }
      temp = null;
   }
   static void write(MCParticle hit, SIOOutputStream out) throws IOException
   {
      if (hit instanceof SIOMCParticle)
         ((SIOMCParticle) hit).write(out);
      else
      {
         out.writePTag(hit);
         int nParents = hit.getParents().size();
         out.writeInt(nParents);

         for (int i = 0; i < nParents; i++)
         {
            out.writePntr(hit.getParents().get(i));
         }

         out.writeInt(hit.getPDG());
         out.writeInt(hit.getGeneratorStatus());
         int simulatorStatus = hit.getSimulatorStatus();
         out.writeInt(simulatorStatus);

         double[] vertex = hit.getVertex();
         out.writeDouble(vertex[0]);
         out.writeDouble(vertex[1]);
         out.writeDouble(vertex[2]);
 
         out.writeFloat( hit.getTime() ) ;

         double[] momentum = hit.getMomentum();
         out.writeFloat((float)momentum[0]);
         out.writeFloat((float)momentum[1]);
         out.writeFloat((float)momentum[2]);
         out.writeFloat((float)hit.getMass());
         out.writeFloat(hit.getCharge());

         if ((simulatorStatus & (1<<31)) != 0)
         {
            double[] endpoint = hit.getEndpoint();
            out.writeDouble(endpoint[0]);
            out.writeDouble(endpoint[1]);
            out.writeDouble(endpoint[2]);
            
            double[] momentumAtEndpoint = hit.getMomentumAtEndpoint();
            out.writeFloat((float) momentumAtEndpoint[0]);
            out.writeFloat((float) momentumAtEndpoint[1]);
            out.writeFloat((float) momentumAtEndpoint[2]);
         }

         float[] spin = hit.getSpin();
         out.writeFloat((float)spin[0]);
         out.writeFloat((float)spin[1]);
         out.writeFloat((float)spin[2]);

         int[] colorFlow = hit.getColorFlow();
         out.writeInt(colorFlow[0]);
         out.writeInt(colorFlow[1]);
      }
   }

   private void write(SIOOutputStream out) throws IOException
   {
      out.writePTag(this);
      out.writeInt(parents.size());
      for (int i = 0; i < parents.size(); i++)
         out.writePntr(getParent(i));
      out.writeInt(pdg);
      out.writeInt(generatorStatus);
      out.writeInt(simulatorStatus);
      out.writeDouble(vertex[0]);
      out.writeDouble(vertex[1]);
      out.writeDouble(vertex[2]);
       
      out.writeFloat( time ) ;
      
      out.writeFloat((float)momentum[0]);
      out.writeFloat((float)momentum[1]);
      out.writeFloat((float)momentum[2]);
      out.writeFloat((float)mass);
      out.writeFloat(charge);

      if ((simulatorStatus & (1<<31)) != 0)
      {
         out.writeDouble(endpoint[0]);
         out.writeDouble(endpoint[1]);
         out.writeDouble(endpoint[2]);
         out.writeFloat((float) momentumAtEndpoint[0]);
         out.writeFloat((float) momentumAtEndpoint[1]);
         out.writeFloat((float) momentumAtEndpoint[2]);
      }

     out.writeFloat((float)spin[0]);
     out.writeFloat((float)spin[1]);
     out.writeFloat((float)spin[2]);

     out.writeInt(colorFlow[0]);
     out.writeInt(colorFlow[1]);

   }
}
