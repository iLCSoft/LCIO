package hep.lcio.implementation.sio;

/** Simple helper class to encode/decode the LCIO version number.
 *  @author gaede
 *  @version $Id: SIOVersion.java,v 1.2 2004-09-17 04:37:43 tonyj Exp $
 */
public class SIOVersion
{
   
   /** Returns the version encoded in one word.
    */
   public static int encode(int major, int minor)
   {
      
      return (major << 16) | ( 0xffff & minor ) ;
   }
}
