package hep.lcio.implementation.event;

import hep.lcio.event.LCIntVec;


/**
 *
 * @author Tony Johnson
 * @version $Id: ILCIntVec.java,v 1.4 2004-09-24 10:39:29 tonyj Exp $
 */
public class ILCIntVec extends ILCObject implements LCIntVec
{
   protected int[] data = new int[10];
   protected int size = 0;

   public void add(int i)
   {
      if (size == data.length)
      {
         int[] newData = new int[data.length * 2];
         System.arraycopy(data, 0, newData, 0, size);
         data = newData;
      }
      data[size++] = i;
   }

   public int[] toArray()
   {
      if (size != data.length)
      {
         int[] result = new int[size];
         System.arraycopy(data, 0, result, 0, size);
         data = result;
      }
      return data;
   }
}
