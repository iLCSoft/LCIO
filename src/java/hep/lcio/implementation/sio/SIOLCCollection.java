package hep.lcio.implementation.sio;

import hep.lcio.implementation.event.ILCCollection;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOLCCollection.java,v 1.1 2003-09-15 21:44:32 tonyj Exp $
 */
class SIOLCCollection extends ILCCollection
{
   private SIOEvent owner;
   public SIOLCCollection(String type, int flag, int size)
   {
      super(type,flag, size);
   }
   void setOwner(SIOEvent owner)
   {
      this.owner = owner;
   }
   protected void checkAccess()
   {
      if (owner != null) owner.checkSIOAccess();
   }
}