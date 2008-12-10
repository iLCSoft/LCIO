package hep.lcio.implementation.sio;

import hep.lcio.implementation.io.LCFactory;

import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOFactory.java,v 1.6 2008-12-10 08:10:59 gaede Exp $
 */
public class SIOFactory extends LCFactory
{
   final static String runRecordName = "LCRunHeader";
   final static String runBlockName = "RunHeader";
   final static String eventRecordName = "LCEvent";
   final static String eventBlockName = "Event";
   final static String eventHeaderRecordName = "LCEventHeader";
   final static String eventHeaderBlockName = "EventHeader";
   final static boolean compressionMode = true;

   public LCReader createLCReader()
   {
      return new SIOLCReader();
   }
   public LCReader createLCReader(int lcReaderFlag)
   { //fg: dumy implementation - could use readerFlag in the future (now only C++)
      return new SIOLCReader();
   }

   
   public LCWriter createLCWriter()
   {
      return new SIOLCWriter();
   }
}
