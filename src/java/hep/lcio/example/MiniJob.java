package hep.lcio.example;

import hep.lcio.event.*;

import hep.lcio.implementation.event.*;
import hep.lcio.implementation.io.LCFactory;

import hep.lcio.io.*;

import java.io.IOException;
import java.util.Random;


/**
 *
 * @author Tony Johnson
 * @version $Id: MiniJob.java,v 1.5 2003-11-08 03:08:50 tonyj Exp $
 */
public class MiniJob
{
   private final static int NRUN = 1;
   private final static int NEVENT = 1;
   private final static int NMCPART = 1; // mc particles per event
   private final static int NHITS = 1; // calorimeter hits per event
   private final static String detName = "MiniDet";
   private final static String ecalName = "ECALmini";

   /**
    * @param args the command line arguments
    */
   public static void main(String[] args)
   {
      // create sio writer
      Random random = new Random();
      LCWriter lcWrt = LCFactory.getInstance().createLCWriter();

      try
      {
         lcWrt.open(args[0]);
      }
      catch (IOException e)
      {
         System.out.println("cannot open file" + args[0] + e.getMessage());
         System.exit(1);
      }

      // loop over runs
      for (int rn = 0; rn < NRUN; rn++)
      {
         ILCRunHeader runHdr = new ILCRunHeader();
         runHdr.setRunNumber(rn);
         runHdr.setDetectorName(detName);
         runHdr.setDescription("minimal file");
         runHdr.addActiveSubdetector(ecalName);

         try
         {
            lcWrt.writeRunHeader(runHdr);
         }
         catch (IOException e)
         {
            System.out.println("Couldn't write run header " + rn);
         }

         // EventLoop - create some events and write them to the file
         for (int i = 0; i < NEVENT; i++)
         {
            // we need to use the implementation classes here
            ILCEvent evt = new ILCEvent();

            evt.setRunNumber(rn);
            evt.setEventNumber(i);
            evt.setDetectorName(detName);

            // create and add some mc particles
            ILCCollection mcVec = new ILCCollection(LCIO.MCPARTICLE);

            IMCParticle mom = null;
            for (int j = 0; j < NMCPART; j++)
            {
               IMCParticle mcp = new IMCParticle();

               mcp.setPDG(101 + (j * 100));
               mom.addDaughter(mcp);

               float[] p = { 2.f * 1024.f, 4.f * 1024.f, 8.f * 1024.f };
               mcp.setMomentum(p);

               mom = mcp; // one body decays :-)
               mcVec.add(mcp);
            }

            // now add some calorimeter hits
            ILCCollection calVec = new ILCCollection(LCIO.SIMCALORIMETERHIT);

            int flag = 1 << LCIO.CHBIT_LONG; //
            flag = flag | (1 << LCIO.CHBIT_PDG);
            calVec.setFlag(flag);

            for (int j = 0; j < NHITS; j++)
            {
               ISimCalorimeterHit hit = new ISimCalorimeterHit();
               hit.setEnergy(3.1415f);

               float[] pos = { 1.f, 2.f, 4.f };

               hit.setPosition(pos);

               calVec.add(hit);

               hit.addMCParticleContribution((MCParticle) mcVec.getElementAt(0), 1.f, 2.f, 4);
            }

            // add all collection to the event
            evt.addCollection(mcVec, "MCParticle");
            evt.addCollection(calVec, ecalName);

            // dump the event to the screen
            LCTools.dumpEvent(evt);

            // write the event to the file
            try
            {
               lcWrt.writeEvent(evt);
            }
            catch (IOException e)
            {
               System.out.println("Couldn't write event " + i);
            }
         }

         // evt loop
      }

      // run loop
      System.out.println();
      System.out.println(" created  " + NRUN + " runs with  " + (NRUN * NEVENT) + " events");
      try
      {
         lcWrt.close();
      }
      catch (Exception e) {}
   }
}
