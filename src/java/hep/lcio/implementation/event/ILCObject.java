package hep.lcio.implementation.event;

import hep.lcio.event.LCIO;

/**
 * 
 * @author Tony Johnson
 * @version $Id: ILCObject.java,v 1.1 2003-05-06 04:58:55 tonyj Exp $
 */
class ILCObject {

    private int accessMode = LCIO.UPDATE;

    void checkAccess()
    {
        if (accessMode != LCIO.UPDATE) throw new LCIOReadOnlyException();
    }
}
