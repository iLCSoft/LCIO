package hep.lcio.implementation.event;

import hep.lcio.event.LCStrVec;


/**
 *
 * @author Tony Johnson
 * @version $Id: ILCStrVec.java,v 1.2 2004-09-24 10:39:29 tonyj Exp $
 */
public class ILCStrVec extends ILCObject implements LCStrVec
{
   protected String[] data = new String[10];
   protected int size = 0;

   public void add(String str)
   {
      if (size == data.length)
      {
         String[] newData = new String[size * 2];
         System.arraycopy(data, 0, newData, 0, size);
         data = newData;
      }
      data[size++] = str;
   }

   public String[] toArray()
   {
      if (size != data.length)
      {
         String[] result = new String[size];
         System.arraycopy(data, 0, result, 0, size);
         data = result;
      }
      return data;   
   }
}
