package hep.lcio.implementation.event;

import hep.lcio.event.LCCollection;
import hep.lcio.event.LCObject;

import java.util.ArrayList;
import java.util.List;


/**
 * A default implementation of LCCollection
 * @author Tony Johnson
 * @version $Id: ILCCollection.java,v 1.2 2003-05-06 06:22:11 tonyj Exp $
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
      super.add(object);
   }
}
