package hep.lcio.implementation.sio;

import hep.lcio.implementation.io.LCFactory;

import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOFactory.java,v 1.4 2003-05-12 16:37:31 gaede Exp $
 */
public class SIOFactory extends LCFactory
{
   final static String runRecordName = "LCRunHeader";
   final static String runBlockName = "RunHeader";
   final static String eventRecordName = "LCEvent";
   final static String eventBlockName  = "Event";
   final static String eventHeaderRecordName = "LCEventHeader";
   final static String eventHeaderBlockName  = "EventHeader";

    final static boolean  compressionMode = true ;

   public LCReader createLCReader()
   {
      return new SIOLCReader();
   }

   public LCWriter createLCWriter()
   {
      return new SIOLCWriter();
   }
}
