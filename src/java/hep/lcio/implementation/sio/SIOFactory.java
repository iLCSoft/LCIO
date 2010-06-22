package hep.lcio.implementation.sio;

import hep.lcio.implementation.io.LCFactory;

import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOFactory.java,v 1.7 2010-06-22 13:49:56 gaede Exp $
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
      return new SIOLCRandomAccessReader();
   }
   public LCReader createLCReader(int lcReaderFlag)
   { 
      return new SIOLCRandomAccessReader();
   }

   
   public LCWriter createLCWriter()
   {
      return new SIOLCWriter();
   }
}
