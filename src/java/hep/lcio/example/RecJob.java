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
 * @version $Id: RecJob.java,v 1.15 2006-09-21 06:10:49 gaede Exp $
 */
public class RecJob implements LCRunListener, LCEventListener
{
   private final static int NHITS = 50;
   private final static int NCLUSTERS = 50;
   private final static int NTRACKS = 50;
   private final static int NVERTICES = 10;

   private LCWriter lcWrt;
   private Random random = new Random();
   private int nEvent;

   private RecJob(String file) throws IOException
   {
      lcWrt = LCFactory.getInstance().createLCWriter();
      lcWrt.open(file);
   }

   /**
    * @param args the command line arguments
    */
   public static void main(String[] args) throws IOException
   {
      if (args.length < 2)
         help();

      // create reader and writer for input and output streams 
      LCReader lcReader = LCFactory.getInstance().createLCReader();
      lcReader.open(args[0]);

      // create a new RunEventProcessor, register it with the reader
      // and read and proccess the whole stream 
      RecJob recjob = new RecJob(args[1]);

      lcReader.registerLCRunListener(recjob);
      lcReader.registerLCEventListener(recjob);
      lcReader.readStream();
      lcReader.close();
      recjob.close();
   }

   public void processRunHeader(LCRunHeader run)
   {
      try
      {
         lcWrt.writeRunHeader(run);
      }
      catch (IOException x)
      {
         throw new RuntimeException("Error writing event", x);
      }
   }

   public void processEvent(LCEvent evt)
   {
      // don't modify events (except for adding new collections)
   	 
   	// add some calibration constants (user class!) to the event
   	 if( evt.getEventNumber()==0){
       ILCCollection col = new ILCCollection( LCIO.LCGENERICOBJECT ) ; 
       for (int i = 0; i < 50 ; i++) {	
   	     CalibrationConstant cal =
   	     	new CalibrationConstant( 65563+i,(float)(i*3.141592),(float) (0.1*i) ) ;	
   	     col.addElement( cal ) ;
       }
   	   evt.addCollection( col , "Calibration" ) ;
   	 }
   }

   public void modifyRunHeader(LCRunHeader run)
   {
      // don't manipulate run headers 
   }

   public void modifyEvent(LCEvent evt)
   {
      // this is our event loop code
      // read collection with MCParticles
      LCCollection mcVec = evt.getCollection(LCIO.MCPARTICLE);

      int NMCPART = mcVec.getNumberOfElements();

      // create a new collection to be added to the event
      ILCCollection calVec = new ILCCollection(LCIO.SIMCALORIMETERHIT);

      int flag = 1 << LCIO.CHBIT_LONG;
      flag = flag | (1 << LCIO.CHBIT_PDG); // include pdg as well
      calVec.setFlag(flag);

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
         hit.addMCParticleContribution((MCParticle) mcVec.getElementAt(mcIndx), 0.2876f, 0.007f, 565656);
      }

      // add sth. to en existing collection - this needs a cast to the 
      // collection implementation type
      // not the nicest solution, but it keeps the API a lot simpler...
      ILCCollection colVec = (ILCCollection) mcVec;

      IMCParticle part = new IMCParticle();

      part.setPDG(1234);
      ((IMCParticle) colVec.getElementAt(0)).addDaughter(part);

      colVec.add(part);
      evt.addCollection(calVec, "HCALReco");

      LCTools.dumpEvent(evt);
      System.out.println("------------------------------------");

      // ****NEW*** Add some clusters 
      
      ILCCollection clusterVec = new ILCCollection(LCIO.CLUSTER);
      for (int j = 0; j < NCLUSTERS; j++)
      {
         ICluster cluster = new ICluster();
         cluster.setEnergy(j+1.0f);
         cluster.setIPhi(j+2.0f);
         cluster.setITheta(j+3.0f);        
         float[] subDetEnergies = { 10.0f , 20.0f, 30.0f, 40.0f, 50.0f  } ; 
         cluster.setSubdetectorEnergies(subDetEnergies) ;
         clusterVec.add(cluster);
      }
      evt.addCollection(clusterVec, "Clusters");      

      ILCCollection trackVec = new ILCCollection(LCIO.TRACK);
      for (int j = 0; j < NTRACKS; j++)
      {
         ITrack track = new ITrack();
         track.setDEdx(j);
         track.setPhi(j);
         track.setTanLambda(j);        
         trackVec.add(track);
      }
      evt.addCollection(trackVec, "Tracks");
      
      ILCCollection vertexVec = new ILCCollection(LCIO.VERTEX);
      for (int j = 0; j < NVERTICES; j++)
      {
         IVertex v = new IVertex();
         v.setPrimary((j == 0 ? true : false));
         
         switch((random.nextInt()%7)){
         	case 0: v.setAlgorithmType( "ZvTop" ); break;
         	case 1: v.setAlgorithmType( "ZvKin" ); break;
         	case 2: v.setAlgorithmType( "42" ); break;
         	case 3: v.setAlgorithmType( "SimAnnealing" ); break;
         	case 5: v.setAlgorithmType( "_Test" ); break;
         	default: break;
         }
         
         v.setChi2(j+1.0f);
         v.setProbability(j+2.0f);
         float f[] = {j+j*1.0f,j+j*2.0f,j+j*3.0f};
         v.setPosition(f);
         float[] cov = { 10.0f , 20.0f, 30.0f, 40.0f, 50.0f, 60.0f };
         v.setCovMatrix(cov);
         vertexVec.add(v);
      }
      evt.addCollection(vertexVec, "Vertices");
      
      try
      {
         lcWrt.writeEvent(evt);
         nEvent++;
      }
      catch (IOException x)
      {
         throw new RuntimeException("Error writing event", x);
      }
   }

   private void close() throws IOException
   {
      lcWrt.close();
      System.out.println("Analyzed " + nEvent + " events");
   }

   private static void help()
   {
      System.out.println("java " + RecJob.class.getName() + " <input-file> <output-file>");
      System.exit(1);
   }
}
