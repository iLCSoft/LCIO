package hep.lcio.implementation.sio;

import hep.lcio.implementation.io.LCFactory;

import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOFactory.java,v 1.2 2003-05-06 06:22:12 tonyj Exp $
 */
public class SIOFactory extends LCFactory
{
   final static String runRecordName = "LCRunHeader";
   final static String eventRecordName = "LCEvent";
   final static String eventHeaderRecordName = "LCEventHeader";

   public LCReader createLCReader()
   {
      return new SIOLCReader();
   }

   public LCWriter createLCWriter()
   {
      return new SIOLCWriter();
   }
}
