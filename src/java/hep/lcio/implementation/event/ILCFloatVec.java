package hep.lcio.implementation.event;

import hep.lcio.event.LCFloatVec;


/**
 *
 * @author Tony Johnson
 * @version $Id: ILCFloatVec.java,v 1.9 2004-09-24 10:39:29 tonyj Exp $
 */
public class ILCFloatVec extends ILCObject implements LCFloatVec
{
   protected float[] data = new float[10];
   protected int size = 0;

   public void add(float f)
   {
      if (size == data.length)
      {
         float[] newData = new float[data.length * 2];
         System.arraycopy(data, 0, newData, 0, size);
         data = newData;
      }
      data[size++] = f;
   }

   public float[] toArray()
   {
      if (size != data.length)
      {
         float[] result = new float[size];
         System.arraycopy(data, 0, result, 0, size);
         data = result;
      }
      return data;
   }
}
