package hep.lcio.implementation.event;

import hep.lcio.event.LCIO;


/**
 *
 * @author Tony Johnson
 * @version $Id: ILCObject.java,v 1.2 2003-05-06 06:22:11 tonyj Exp $
 */
class ILCObject
{
   private int accessMode = LCIO.UPDATE;

   void checkAccess()
   {
      if (accessMode != LCIO.UPDATE)
         throw new LCIOReadOnlyException();
   }
}
