package hep.lcio.implementation.io;

import hep.lcio.implementation.sio.SIOFactory;

import hep.lcio.io.ILCFactory;


/**
 *
 * @author Tony Johnson
 * @version $Id: LCFactory.java,v 1.3 2004-04-08 09:58:01 gaede Exp $
 */
public abstract class LCFactory implements ILCFactory
{
   public static ILCFactory getInstance()
   {
      return new SIOFactory();
   }
}
