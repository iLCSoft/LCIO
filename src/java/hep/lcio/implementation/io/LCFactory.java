package hep.lcio.implementation.io;

import hep.lcio.implementation.sio.SIOFactory;
import hep.lcio.io.ILCFactory;
import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;

/**
 *
 * @author Tony Johnson
 * @version $Id: LCFactory.java,v 1.1 2003-05-06 04:58:55 tonyj Exp $
 */
public abstract class LCFactory implements ILCFactory {

    public static ILCFactory getInstance()
    {
        return new SIOFactory();
    }   
}