package hep.lcio.implementation.io;

import hep.lcio.implementation.sio.SIOFactory;

import hep.lcio.io.ILCFactory;
import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;


/**
 *
 * @author Tony Johnson
 * @version $Id: LCFactory.java,v 1.2 2003-05-06 06:22:11 tonyj Exp $
 */
public abstract class LCFactory implements ILCFactory
{
   public static ILCFactory getInstance()
   {
      return new SIOFactory();
   }
}
