package hep.lcio.implementation.sio;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import hep.io.sio.SIORef;
import hep.lcio.implementation.event.ILCCollection;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOLCCollection.java,v 1.3 2007-11-07 20:46:22 jeremy Exp $
 */
class SIOLCCollection extends ILCCollection
{
   private SIOEvent owner;

   private List tempObjects = null;

   
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

   
   /** for collections that hold subsets we initially store the pointers (SIORefs)*/
   public void addPointer( SIORef ptr )
   {
      if( tempObjects == null )
         tempObjects = new ArrayList() ; 
      tempObjects.add( ptr ) ;
   }
   
   /** for collections that hold subsets resolve the stored pointers (SIORefs)*/
   public void resolve()
   {
      for (Iterator i = tempObjects.iterator(); i.hasNext();)
      {
         add(((SIORef) i.next()).getObject());
      }
      tempObjects = null;

   }
   
   
}