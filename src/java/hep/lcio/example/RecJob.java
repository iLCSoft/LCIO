package hep.lcio.example;

import hep.lcio.event.*;
import hep.lcio.implementation.event.*;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.*;
import java.util.Random;

/**
 *
 * @author Tony Johnson
 * @version $Id: RecJob.java,v 1.1 2003-05-06 04:58:54 tonyj Exp $
 */
public class RecJob implements LCRunListener, LCEventListener {
    
    private final static int NHITS = 50;
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
         // create reader and writer for input and output streams 
        LCReader lcReader = LCFactory.getInstance().createLCReader();
        lcReader.open(args[0]);

        // create a new RunEventProcessor, register it with the reader
        // and read and proccess the whole stream 

        RecJob recjob = new RecJob(args[1]);

        lcReader.registerLCRunListener( recjob ); 
        lcReader.registerLCEventListener( recjob); 
        lcReader.readStream() ;
        lcReader.close() ;
        recjob.close();
    }
    
    private RecJob(String file)
    {
        lcWrt = LCFactory.getInstance().createLCWriter();
        lcWrt.open(file);
    }
    private void close()
    {
        lcWrt.close();
        System.out.println("Analyzed "+nEvent+" events");
    }
    public void analyze(LCRunHeader run) {
        // just copy run headers to the outputfile
        lcWrt.writeRunHeader(run) ;
    }
    
    public void update(LCRunHeader run) {
        // don't manipulate run headers - use analyze
    }
    
    public void analyze(LCEvent evt) {
        // used for read only access
    }
    
    public void update(LCEvent evt) {
        // this is our event loop code
    
        // read collection with MCParticles
        LCCollection mcVec = evt.getCollection( LCIO.MCPARTICLE );
    
        int NMCPART = mcVec.getNumberOfElements() ;
    
        // create a new collection to be added to the event
        // for simplicity just add some calorimeter hits (don't have cluster class yet) 
    
        ILCCollection calVec = new ILCCollection( LCIO.CALORIMETERHIT );
    
        for(int j=0;j<NHITS;j++){
          ICalorimeterHit hit = new ICalorimeterHit();
          hit.setEnergy( 3.1415f * random.nextFloat())  ;
          float pos[] = { 1.1f* random.nextFloat(), 2.2f* random.nextFloat() , 3.3f* random.nextFloat() } ;
          hit.setPosition( pos ) ;
          calVec.add( hit ) ;
      
          // assign the hits randomly to MC particles
          int mcIndx = random.nextInt(NMCPART) ;
          hit.addMCParticleContribution( (MCParticle) mcVec.getElementAt( mcIndx ) , 
				       0.2876f , 0.007f , 565656 ) ;
      
        }
    
        // add sth. to en existing collection - this needs a cast to the 
        // collection implementation type
        // not the nicest solution, but it keeps the API a lot simpler...
        ILCCollection colVec = (ILCCollection) mcVec;
      
        IMCParticle part = new IMCParticle() ;
      
        part.setPDG( 1234 ) ;
        part.setParent( (MCParticle) colVec.getElementAt(0)) ;
      
        colVec.add( part ) ;
        evt.addCollection( calVec , "HCALReco" );

        LCTools.dumpEvent( evt ) ;
        lcWrt.writeEvent( evt ) ;
        nEvent ++ ;
    }
    private LCWriter lcWrt;
    private Random random = new Random();
    private int nEvent;
}
