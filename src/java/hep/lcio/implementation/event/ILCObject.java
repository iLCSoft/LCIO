package hep.lcio.implementation.event;

import hep.lcio.event.LCIO;
import hep.lcio.exceptions.ReadOnlyException;


/**
 * An object which can be used as a base class for implementation of LCIO 
 * objects. Supports access to control (ie controls whether clients are allowed
 * to update the event).
 * @author Tony Johnson
 * @version $Id: ILCObject.java,v 1.5 2004-09-24 10:39:29 tonyj Exp $
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
   protected static boolean bitTest(int flag, int bit)
   {
      return (flag & (1<<bit)) != 0;
   }
   protected static int bitSet(int flag, int bit, boolean set)
   {
      int mask = 1<<bit;
      if (set) flag |= mask;
      else flag &= ~mask;
      return flag;
   }
}
