package hep.lcio.implementation.event;

import hep.lcio.event.LCIO;
import hep.lcio.exceptions.ReadOnlyException;


/**
 * An object which can be used as a base class for implementation of LCIO 
 * objects. Supports access to control (ie controls whether clients are allowed
 * to update the event).
 * @author Tony Johnson
 * @version $Id: ILCObject.java,v 1.3 2003-09-15 21:44:31 tonyj Exp $
 */
class ILCObject
{
   private int accessMode = LCIO.UPDATE;
   private ILCObject parent;

   protected void checkAccess()
   {
      if (parent != null) parent.checkAccess();
      else if (accessMode != LCIO.UPDATE) throw new ReadOnlyException();
   }
   protected void setAccess(int mode)
   {
      accessMode = mode;
   }
   protected void setParent(ILCObject parent)
   {
      this.parent = parent;
   }
}
