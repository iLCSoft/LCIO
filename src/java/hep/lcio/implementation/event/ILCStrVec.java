package hep.lcio.implementation.event;

import hep.lcio.event.LCStrVec;


/**
 *
 * @author Tony Johnson
 * @version $Id: ILCStrVec.java,v 1.1 2004-07-02 08:46:45 hvogt Exp $
 */
public class ILCStrVec extends ILCObject implements LCStrVec
{
   protected String[] data = new String[0];
   protected int size = 0;

   public void add(String str)
   {
      size = data.length;
      String[] newData = new String[size + 1];
      System.arraycopy(data, 0, newData, 0, size);
      newData[size] = str;
      data = newData;
      data[size++] = str;
   }

   public String[] toArray()
   {
      return data;
   }
}
