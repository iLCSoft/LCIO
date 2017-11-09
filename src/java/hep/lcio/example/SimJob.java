package hep.lcio.example;

import hep.lcio.event.*;

import hep.lcio.implementation.event.*;
import hep.lcio.implementation.io.LCFactory;

import hep.lcio.io.*;

import java.io.IOException;
import java.util.Iterator;
import java.util.Random;


/**
 *
 * @author Tony Johnson
 * @version $Id: SimJob.java,v 1.22 2010-06-30 17:35:30 jeremy Exp $
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
    public static void main(String[] args) throws IOException
    {
      if (args.length == 0)
         help();

      // create sio writer
      Random random = new Random();
      LCWriter lcWrt = LCFactory.getInstance().createLCWriter();

      lcWrt.open(args[0]);

      // loop over runs
      for (int rn = 0; rn < NRUN; rn++)
      {
         ILCRunHeader runHdr = new ILCRunHeader();
         runHdr.setRunNumber(rn);
         runHdr.setDetectorName(detName);
         runHdr.setDescription(" these are just dummy runs for testing lcio - no physics whatsoever !");
         runHdr.addActiveSubdetector(ecalName);
         runHdr.addActiveSubdetector(tpcName);
         
         runHdr.getParameters().setValue("SimulationProgram","SimJob.java") ;
		 int[] idx = { 1,2,3,4,5 } ;
         runHdr.getParameters().setValues("SomeIndices",idx) ;

         lcWrt.writeRunHeader(runHdr);

         // EventLoop - create some events and write them to the file
         for (int i = 0; i < NEVENT; i++)
         {
            // we need to use the implementation classes here
            ILCEvent evt = new ILCEvent();

            evt.setRunNumber(rn);
            evt.setEventNumber(i);
            evt.setDetectorName("D09TileHcal");
	    evt.setWeight(  random.nextFloat() ) ;

            // create and add some mc particles
            ILCCollection mcVec = new ILCCollection(LCIO.MCPARTICLE);
  
            // debug: don't write MCParticle collection
            // mcVec.setTransient() ;


            IMCParticle mom = null;
            for (int j = 0; j < NMCPART; j++)
            {
               IMCParticle mcp = new IMCParticle();

               mcp.setPDG(101 + (j * 100));
               if (mom != null) mom.addDaughter(mcp);

               float[] p = { 2.f / 1024.f, 4.f / 1024.f, 8.f / 1024.f };
               mcp.setMomentum(p);
               mcp.setMass((float) 3.01);

               mom = mcp; // one body decays :-)
               mcVec.add(mcp);
            }

            // now add some calorimeter hits
            ILCCollection calVec = new ILCCollection(LCIO.SIMCALORIMETERHIT);

            // set flag for long format - including position
            int flag = 1 << LCIO.CHBIT_LONG;
            flag = flag | (1 << LCIO.CHBIT_PDG); // include pdg as well
            calVec.setFlag(flag);

            for (int j = 0; j < NHITS; j++)
            {
               ISimCalorimeterHit hit = new ISimCalorimeterHit();
               hit.setEnergy(3.1415f * random.nextFloat());
               hit.setCellID0(random.nextInt());
               
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
               float[] stepPos = new float[3] ;
               stepPos[0] = (float) 1.1 ;
               stepPos[1] = (float) 2.2 ;
               stepPos[2] = (float) 3.3 ;
               
               
               hit.addMCParticleContribution((MCParticle) mcVec.getElementAt(mcIndx), 0.314159f, 0.1155f, 42.f , 121212, stepPos );
            }

            // and finally some tracker hits
            // with some user extensions (4 floats) per track:
            // we just create a parallel collection of float vectors
            ILCCollection trkVec = new ILCCollection(LCIO.SIMTRACKERHIT);
            ILCCollection extFVec = new ILCCollection(LCIO.LCFLOATVEC);
            ILCCollection extIVec = new ILCCollection(LCIO.LCINTVEC);
            ILCCollection extStrVec = new ILCCollection(LCIO.LCSTRVEC);

            for (int j = 0; j < NHITS; j++)
            {
               ISimTrackerHit hit = new ISimTrackerHit();
               ILCFloatVec   extF = new ILCFloatVec();
               ILCIntVec     extI = new ILCIntVec();
               ILCStrVec     extS = new ILCStrVec();

               hit.setdEdx(30e-9f);

               double[] pos = 
               {
                  1.1 * random.nextDouble(), 2.2 * random.nextDouble(),
                  3.3 * random.nextDouble()
               };
               hit.setPosition(pos);

               // assign the hits randomly to MC particles
               int mcIndx = random.nextInt(NMCPART);

               hit.setMCParticle((MCParticle) mcVec.getElementAt(mcIndx));

               // fill the extension vectors (4 floats, 2 ints, 3 strings)
               extF.add(3.14159f);
               for (int k = 0; k < 3; k++)
                  extF.add((float) pos[k] * 0.1f);
               
               extI.add( 123456789 );
               extI.add( mcIndx );
               extS.add("SimTrackerHit");
               extS.add("trackhit " + j);
               extS.add("MCParticle index " + mcIndx);
               
               // add the hit and the extensions to their corresponding collections
               trkVec.add(hit);
               extFVec.add(extF);
               extIVec.add(extI);
               extStrVec.add(extS);
            }

            // add all collection to the event
             
            evt.addCollection(mcVec, "MCParticle");
            evt.addCollection(calVec, ecalName);
            evt.addCollection(trkVec, tpcName);
            evt.addCollection(extFVec, tpcName + "UserFloatExtension");
            evt.addCollection(extIVec, tpcName + "UserIntExtension");
            evt.addCollection(extStrVec, tpcName + "UserStringExtension");


    // test: add a collection for one event only:
    if( rn == NRUN-1 && i == 0 ) { // first event o last run
      ILCCollection addExtVec = new ILCCollection( LCIO.LCFLOATVEC )  ;
      ILCFloatVec addExt = new ILCFloatVec() ;
      addExt.add( 1.f );
      addExt.add( 2.f );
      addExt.add( 3.f );
      addExt.add( 4.f );
      addExtVec.add( addExt ) ;
      evt.addCollection( addExtVec , "AdditionalExtension" ) ;
    }
    
//cng    
    // even though this is a simjob we can store 'real data' objects :)
    // --- for example we can store TPC hits ------------

    ILCCollection TPCVec = new ILCCollection( LCIO.TRACKERRAWDATA )  ;


    int tpcFlag = 0 ;

    // store the cellId1
    tpcFlag = 1 << LCIO.TRAWBIT_ID1 ;
    TPCVec.setFlag( tpcFlag  ) ;
    
    for(int j=0;j<NHITS;j++){
      
      ITrackerRawData tpcHit = new ITrackerRawData();
      
      tpcHit.setCellID0( j ) ;
      tpcHit.setTime( 1234567 ) ;
//       tpcHit.setCharge( 3.14159f ) ;
//       tpcHit.setQuality(  0xbad ) ;


      // generate a random number of datawords less than 10
      int size = 0 ; //     FIXME: there is a bug in writing the short array !!!    random.nextInt(10);
// 	  int size = random.nextInt(10);
      short[] rawData = new short[size] ;
      // fill some random numbers into the array;   
      for(int k=0;k<size;k++){
          rawData[k] = (short) random.nextInt() ;   
      }
      // set the raw data
      tpcHit.setADCValues( rawData ) ;
    

      TPCVec.add( tpcHit ) ;
    }   
    evt.addCollection( TPCVec , "TrackerRawDataExample" ) ;
    



    //-------------- write example for subset collection --------------------    
    ILCCollection mcSubVec = new ILCCollection(LCIO.MCPARTICLE);
    mcSubVec.setSubset(true) ;
    for (Iterator iter = mcVec.iterator(); iter.hasNext();) {
      MCParticle p = (MCParticle) iter.next();
      if( p.getDaughters().size() == 0)
         mcSubVec.addElement(p) ;
    }
    evt.addCollection(mcSubVec, "FinalMCParticles" ) ;
    // ------- end subset -------------------------------------------
    
//cng
            // write the event to the file
            lcWrt.writeEvent(evt);

            // dump the event to the screen
            LCTools.dumpEvent(evt);
         }

         // evt loop
      }

      // run loop
      System.out.println();
      System.out.println(" created  " + NRUN + " runs with  " + (NRUN * NEVENT) + " events");
      lcWrt.close();
   }

   private static void help()
   {
      System.out.println("java " + SimJob.class.getName() + " <output-file>");
      System.exit(1);
   }
}
