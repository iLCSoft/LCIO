package hep.lcio.test;

import hep.lcio.test.RandomEvent;
import hep.lcio.io.ILCFactory;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCWriter;
import java.io.File;
import java.io.IOException;

/** Simple program that creates an slcio file containing a random event.
 * 
 * @author gaede
 * @version $Id: CreateRandomEvent.java,v 1.1 2005-06-01 15:33:12 gaede Exp $
 */
public class CreateRandomEvent
{
   public static void main(String[] args) throws IOException
   {
      if (args.length != 1 ){
         System.out.println("usage: CreateRandomEvent filename" ) ;
         System.exit(1);
      }
             
      RandomEvent e = new RandomEvent();
      
      ILCFactory factory = LCFactory.getInstance();
      LCWriter lcWrt = factory.createLCWriter();
      File file = new File( args[0] );

      lcWrt.open(file.getAbsolutePath());
      lcWrt.writeEvent(e);
      lcWrt.close();
   }
}
