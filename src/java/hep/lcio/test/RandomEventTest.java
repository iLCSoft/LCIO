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

public class RandomEventTest
{
   public static void main(String[] args) throws IOException
   {
      RandomEvent e = new RandomEvent();
      ObjectComparator cmp = new ObjectComparator();

      // self compare
      cmp.compare(e,e);

      System.out.println("Compare result of same random event: " + cmp.getResultString() );

      ILCFactory factory = LCFactory.getInstance();
      LCWriter lcWrt = factory.createLCWriter();
      File file = new File("temp.slcio");
   //   file.deleteOnExit();
      lcWrt.open(file.getAbsolutePath());
      lcWrt.writeEvent(e);
      lcWrt.close();

//      File inFile = new File("temp.slcio");
      LCReader lcRead = factory.createLCReader();
      lcRead.open(file.getAbsolutePath());
      LCEvent e2 = lcRead.readNextEvent();
      lcRead.close();

      // compare objects
      cmp.reset();
      cmp.compare(e, e2);

      System.out.println("Compare result of random event and temp file copy: " + cmp.getResultString() );

      System.out.println( cmp.getResultString() );
   }
}
