package hep.lcio.implementation.sio;

import java.io.IOException;

/**
 *
 * @author Tony Johnson
 * @version $Id: LCIOException.java,v 1.1 2003-05-06 04:58:55 tonyj Exp $
 */
public class LCIOException extends RuntimeException {
    
    /** Creates a new instance of LCIOException */
    LCIOException(IOException x) {
        initCause(x);
    }
    
}
