package hep.lcio.example;

import hep.lcio.event.*;

import hep.lcio.implementation.io.LCFactory;

import hep.lcio.io.*;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 */
public class AnalysisJob
{
   /**
    * @param args the command line arguments
    */
   public static void main(String[] args) throws IOException
   {
      if (args.length == 0)
         help();

      LCReader lcReader = LCFactory.getInstance().createLCReader();
      lcReader.open(args);

      for (;;)
      {
         LCRunHeader runHdr = lcReader.readNextRunHeader();
         if (runHdr == null)
            break;
         System.out.println("  Run : " + runHdr.getRunNumber() + " - " + runHdr.getDetectorName() + ":  " + runHdr.getDescription());
      }

      // close and reopen the file
      lcReader.close();
      lcReader.open(args);

       //fg: for  debugging only:
      //lcReader.skipNEvents( 97 ) ;

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

      System.out.println("  " + nEvents + " events read from files : ") ; 
      for(int i=0;i<args.length;i++){
	  System.out.println("       " + args[i] ) ;
      }


      lcReader.close();
   }

   private static void help()
   {
      System.out.println("java " + AnalysisJob.class.getName() + " <input-file1> [[input-file2],...]");
      System.exit(1);
   }
}
