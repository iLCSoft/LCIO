package hep.lcio.implementation.sio;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: LCIOException.java,v 1.2 2003-05-06 06:22:12 tonyj Exp $
 */
public class LCIOException extends RuntimeException
{
   /** Creates a new instance of LCIOException */
   LCIOException(IOException x)
   {
      initCause(x);
   }
}
