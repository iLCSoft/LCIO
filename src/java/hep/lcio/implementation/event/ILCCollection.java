package hep.lcio.implementation.event;

import hep.lcio.data.LCObject;

import hep.lcio.event.LCCollection;
import hep.lcio.event.LCIO;

import hep.lcio.exceptions.ReadOnlyException;

import java.util.ArrayList;
import java.util.List;


/**
 * A default implementation of LCCollection
 * @author Tony Johnson
 * @version $Id: ILCCollection.java,v 1.6 2003-09-15 21:44:31 tonyj Exp $
 */
public class ILCCollection extends ArrayList implements LCCollection
{
   private String type;
   private int flag;

   public ILCCollection(String type)
   {
      this.type = type;
   }

   public ILCCollection(String type, int flag, int size)
   {
      super(size);
      this.flag = flag;
      this.type = type;
   }

   public LCObject getElementAt(int index)
   {
      return (LCObject) get(index);
   }

   public void setFlag(int flag)
   {
      this.flag = flag;
   }

   public int getFlag()
   {
      return flag;
   }

   public int getNumberOfElements()
   {
      return size();
   }

   public String getTypeName()
   {
      return type;
   }

   public void add(LCObject object)
   {
      checkAccess();
      super.add(object);
   }

   public void addElement(LCObject obj) throws ReadOnlyException
   {
      checkAccess();
      super.add(obj);
   }

   public void removeElementAt(int i) throws ReadOnlyException
   {
      checkAccess();
      super.remove(i);
   }

   protected void checkAccess()
   {
      
   }
}
