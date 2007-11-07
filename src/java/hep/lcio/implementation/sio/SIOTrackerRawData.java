package hep.lcio.implementation.sio;
import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;
import hep.lcio.event.LCIO;
import hep.lcio.event.TrackerRawData;
import hep.lcio.implementation.event.ITrackerRawData;
import java.io.IOException;

/**
 *
 * @author tonyj
 * @version $Id: SIOTrackerRawData.java,v 1.3 2007-11-07 20:46:23 jeremy Exp $
 */
class SIOTrackerRawData extends ITrackerRawData
{
   
   public SIOTrackerRawData(SIOInputStream in, int flags, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);

      cellID0 = in.readInt();
      if ((flags & (1 << LCIO.TRAWBIT_ID1)) != 0)  cellID1 = in.readInt();
      else cellID1 = 0;

      time = in.readInt();
      int n = in.readInt();
      if (n > 0)
      {
         aDCValues = new short[n];
         for (int i=0; i<n; i++) aDCValues[i] = in.readShort();
      }
      in.pad();
      in.readPTag(this);
   }
   static void write(TrackerRawData hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOTrackerRawData)
         ((SIOTrackerRawData) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getCellID0());
         if ((flags & (1 << LCIO.TRAWBIT_ID1)) != 0) out.writeInt(hit.getCellID1()); 
      
         out.writeInt(hit.getTime());
         short[] c = hit.getADCValues();
         if (c == null)
         {
            out.writeInt(0);
         }
         else
         {
            out.writeInt(c.length);
            for (int i=0; i<c.length; i++) out.writeShort(c[i]);
         }
         out.pad();
         out.writePTag(hit);
      }
   }
   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(cellID0);
      if ((flags & (1 << LCIO.TRAWBIT_ID1)) != 0) out.writeInt(cellID1);
      out.writeInt(time);
      if (aDCValues == null)
      {
         out.writeInt(0);
      }
      else
      {
         out.writeInt(aDCValues.length);
         for (int i=0; i<aDCValues.length; i++) out.writeShort(aDCValues[i]);
      }
      out.writePTag(this);
   }
   
}
