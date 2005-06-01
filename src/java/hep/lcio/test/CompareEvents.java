package hep.lcio.test;

import hep.lcio.util.ObjectComparator;
import hep.lcio.test.RandomEvent;
import hep.lcio.event.LCEvent;
import hep.lcio.io.ILCFactory;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;
import java.io.File;
import java.io.IOException;

/** Simple program that compares the events in two lcio files, e.g. containing random events.
 * 
 * @author gaede
 * @version $Id: CompareEvents.java,v 1.1 2005-06-01 15:33:12 gaede Exp $
 */
public class CompareEvents
{
   public static void main(String[] args) throws IOException
   {
      if (args.length != 2 ){
         System.out.println("usage: CompareEvents filename1 filename2" ) ;
         System.exit(1);
      }

      ObjectComparator cmp = new ObjectComparator();

      ILCFactory factory = LCFactory.getInstance();

      File file1 = new File( args[0] );
      File file2 = new File( args[1] );

      LCReader lcRead1 = factory.createLCReader();
      lcRead1.open(file1.getAbsolutePath());

      LCReader lcRead2 = factory.createLCReader();
      lcRead2.open(file2.getAbsolutePath());
      
      int evtNum = 0 ;
      for (;;)
      {
         LCEvent e1 = lcRead1.readNextEvent();
         LCEvent e2 = lcRead2.readNextEvent();

         if (e1 == null || e2 == null ){
            if( e1 != null || e2 != null )
               System.out.println(" number of events differ ! ") ;
            break;	
         }
         // compare the events
         cmp.reset();
         cmp.compareLCEvent(e1, e2);
            
         System.out.println("Comparison of event [" + evtNum + "] : "  + cmp.getResultString() );
         evtNum++ ;
      }
            
      lcRead1.close();
      lcRead2.close();
   }
}
