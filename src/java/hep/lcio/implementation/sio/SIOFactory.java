package hep.lcio.implementation.sio;

import hep.lcio.implementation.io.LCFactory;

import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOFactory.java,v 1.3 2003-05-09 15:16:45 gaede Exp $
 */
public class SIOFactory extends LCFactory
{
   final static String runRecordName = "LCRunHeader";
   final static String runBlockName = "RunHeader";
   final static String eventRecordName = "LCEvent";
   final static String eventBlockName  = "Event";
   final static String eventHeaderRecordName = "LCEventHeader";
   final static String eventHeaderBlockName  = "EventHeader";

    final static boolean  compressionMode = false ;

   public LCReader createLCReader()
   {
      return new SIOLCReader();
   }

   public LCWriter createLCWriter()
   {
      return new SIOLCWriter();
   }
}
