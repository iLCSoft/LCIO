package hep.lcio.implementation.event;

import hep.lcio.data.LCIntVec;


/**
 *
 * @author Tony Johnson
 * @version $Id: ILCIntVec.java,v 1.2 2003-09-15 21:44:31 tonyj Exp $
 */
public class ILCIntVec extends ILCObject implements LCIntVec
{
   protected int[] data;
   protected int size = 0;

   public void add(int i)
   {
      if (data == null)
         data = new int[10];
      else if (size == data.length)
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
