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
 * @version $Id: SimJob.java,v 1.6 2003-06-12 08:58:54 gaede Exp $
 */
public class SimJob
{
   private final static int NRUN = 10;
   private final static int NEVENT = 10;
   private final static int NMCPART = 10; // mc particles per event
   private final static int NHITS = 50; // calorimeter hits per event
   private final static String detName = "D09TileHcal";
   private final static String ecalName = "ECAL007";
   private final static String tpcName = "TPC4711";

   /**
    * @param args the command line arguments
    */
   public static void main(String[] args)
   {
      // create sio writer
      Random random = new Random();
      LCWriter lcWrt = LCFactory.getInstance().createLCWriter();

      try{ 
      	lcWrt.open(args[0]) ;
      }
      catch(IOException e){
      	System.out.println( "cannot open file" +  args[0] 
      	                    + e.getMessage() ) ;
	    System.exit(1) ;
      }	  

      // loop over runs
      for (int rn = 0; rn < NRUN; rn++)
      {
         ILCRunHeader runHdr = new ILCRunHeader();
         runHdr.setRunNumber(rn);
         runHdr.setDetectorName(detName);
         runHdr.setDescription(" these are just dummy runs for testing lcio - no physics whatsoever !");
         runHdr.addActiveSubdetector(ecalName);
         runHdr.addActiveSubdetector(tpcName);

         try{
         	lcWrt.writeRunHeader(runHdr);
         }
         catch(IOException e){
         	System.out.println("Couldn't write run header " + rn ) ;
         }

         // EventLoop - create some events and write them to the file
         for (int i = 0; i < NEVENT; i++)
         {
            // we need to use the implementation classes here
            ILCEvent evt = new ILCEvent();

            evt.setRunNumber(rn);
            evt.setEventNumber(i);
            evt.setDetectorName("D09TileHcal");

            // create and add some mc particles
            ILCCollection mcVec = new ILCCollection(LCIO.MCPARTICLE);

            IMCParticle mom = null;
            for (int j = 0; j < NMCPART; j++)
            {
               IMCParticle mcp = new IMCParticle();

               mcp.setPDG(101 + (j * 100));
               if (mom != null) mcp.setParent(mom);

               float[] p = { 2.f / 1024.f, 4.f / 1024.f, 8.f / 1024.f };
               mcp.setMomentum(p);

               mom = mcp; // one body decays :-)
               mcVec.add(mcp);
            }

            // now add some calorimeter hits
            ILCCollection calVec = new ILCCollection(LCIO.SIMCALORIMETERHIT);

     	    // set flag for long format - including position
     	    int flag = 1 << LCIO.CHBIT_LONG  ; 
	        flag = flag | ( 1<< LCIO.CHBIT_PDG )  ; // include pdg as well
	        calVec.setFlag(  flag ) ;

            for (int j = 0; j < NHITS; j++)
            {
               ISimCalorimeterHit hit = new ISimCalorimeterHit();
               hit.setEnergy(3.1415f * random.nextFloat());

               float[] pos = 
               {
                  1.1f * random.nextFloat(), 2.2f * random.nextFloat(),
                  3.3f * random.nextFloat()
               };

               hit.setPosition(pos);

               calVec.add(hit);

               // assign the hits randomly to MC particles
               int mcIndx = random.nextInt(NMCPART);

               // in order to access a MCParticle,  we need a dynamic cast as the
               // LCCollection returns an LCIOObject - this is like vectors in Java
               hit.addMCParticleContribution((MCParticle) mcVec.getElementAt(mcIndx), 0.314159f, 0.1155f, 121212);
            }

            // and finally some tracker hits
            // with some user extensions (4 floats) per track:
            // we just create a parallel collection of float vectors
            ILCCollection trkVec = new ILCCollection(LCIO.SIMTRACKERHIT);
            ILCCollection extVec = new ILCCollection(LCIO.LCFLOATVEC);

            for (int j = 0; j < NHITS; j++)
            {
               ISimTrackerHit hit = new ISimTrackerHit();
               ILCFloatVec ext = new ILCFloatVec();

               hit.setdEdx(30e-9f);

               double[] pos = 
               {
                  1.1 * random.nextDouble(), 2.2 * random.nextDouble(),
                  3.3 * random.nextDouble()
               };

               // fill the extension vector
               ext.add(3.14159f);
               for (int k = 0; k < 3; k++)
                  ext.add((float) pos[k] * 0.1f);

               hit.setPosition(pos);

               // assign the hits randomly to MC particles
               int mcIndx = random.nextInt(NMCPART);

               hit.setMCParticle((MCParticle) mcVec.getElementAt(mcIndx));

               trkVec.add(hit);
               extVec.add(ext);
            }

            // add all collection to the event
            evt.addCollection(mcVec, "MCParticle");
            evt.addCollection(calVec, ecalName);
            evt.addCollection(trkVec, tpcName);
            evt.addCollection(extVec, tpcName + "UserExtension");

            // dump the event to the screen
            LCTools.dumpEvent(evt);

            // write the event to the file
         	try{
         		lcWrt.writeEvent(evt);
         	}
         	catch(IOException e){
         		System.out.println("Couldn't write event " + i ) ;
         	}
        }
          // evt loop
      }
       // run loop

      System.out.println();
      System.out.println(" created  " + NRUN + " runs with  " + (NRUN * NEVENT) + " events");
      try {
		lcWrt.close();
	  } catch (Exception e) {
	  }
   }
}
