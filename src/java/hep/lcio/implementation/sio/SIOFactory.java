package hep.lcio.implementation.sio;

import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOFactory.java,v 1.1 2003-05-06 04:58:56 tonyj Exp $
 */
public class SIOFactory extends LCFactory {
    
    public LCReader createLCReader() {
        return new SIOLCReader();
    }
    
    public LCWriter createLCWriter() {
        return new SIOLCWriter();
    }
    final static String runRecordName = "LCRunHeader";
    final static String eventRecordName = "LCEvent";
    final static String eventHeaderRecordName = "LCEventHeader";
}