package hep.lcio.example;

import hep.lcio.event.*;

import hep.lcio.implementation.io.LCFactory;

import hep.lcio.io.*;


/**
 *
 * @author Tony Johnson
 */
public class AnalysisJob
{
   /**
    * @param args the command line arguments
    */
   public static void main(String[] args)
   {
      LCReader lcReader = LCFactory.getInstance().createLCReader();
      lcReader.open(args[0]);

      for (;;)
      {
         LCRunHeader runHdr = lcReader.readNextRunHeader();
         if (runHdr == null)
            break;
         System.out.println("  Run : " + runHdr.getRunNumber() + " - " + runHdr.getDetectorName() + ":  " + runHdr.getDescription());
      }

      // close and reopen the file
      lcReader.close();
      lcReader.open(args[0]);

      // dump all events in the file
      int nEvents = 0;
      for (;;)
      {
         LCEvent evt = lcReader.readNextEvent();
         if (evt == null)
            break;

         // the following code will fail at runtime - event is read only !
         // if we use " (const LCEvent*) evt " it won't even compile 
         // so we have a twofold protection against data corruption
         // int status = evt.addCollection( new LCCollectionVec(LCIO::CALORIMETERHIT ),"NewCol" ) ; 
         //  if( status != LCIO::SUCCESS ){
         //    cout << " error - couldn't add new collection to the event ! " << endl ;
         //    exit(1) ;
         //  }
         LCTools.dumpEvent(evt);
         nEvents++;
      }

      System.out.println("  " + nEvents + " events read from file : " + args[0]);
      lcReader.close();
   }
}
