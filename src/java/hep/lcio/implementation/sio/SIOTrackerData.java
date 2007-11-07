package hep.lcio.implementation.sio;
import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;
import hep.lcio.event.LCIO;
import hep.lcio.event.TrackerData;
import hep.lcio.implementation.event.ITrackerData;
import java.io.IOException;

/**
 *
 * @author tonyj
 * @version $Id: SIOTrackerData.java,v 1.2 2007-11-07 20:46:23 jeremy Exp $
 */
class SIOTrackerData extends ITrackerData
{
   
   public SIOTrackerData(SIOInputStream in, int flags, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);

      cellID0 = in.readInt();
      if ((flags & (1 << LCIO.TRAWBIT_ID1)) != 0)  cellID1 = in.readInt();
      else cellID1 = 0;

      time = in.readFloat();
      int n = in.readInt();
      if (n > 0)
      {
         chargeValues = new float[n];
         for (int i=0; i<n; i++) chargeValues[i] = in.readFloat();
      }
      in.readPTag(this);
   }
   static void write(TrackerData hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOTrackerData)
         ((SIOTrackerData) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getCellID0());
         if ((flags & (1 << LCIO.TRAWBIT_ID1)) != 0) out.writeInt(hit.getCellID1()); 
         out.writeFloat(hit.getTime());
         float[] c = hit.getChargeValues();
         if (c == null)
         {
            out.writeInt(0);
         }
         else
         {
            out.writeInt(c.length);
            for (int i=0; i<c.length; i++) out.writeFloat(c[i]);
         }
         out.writePTag(hit);
      }
   }
   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(cellID0);
      if ((flags & (1 << LCIO.TRAWBIT_ID1)) != 0) out.writeInt(cellID1);
      out.writeFloat(time);
      if (chargeValues == null)
      {
         out.writeInt(0);
      }
      else
      {
         out.writeInt(chargeValues.length);
         for (int i=0; i<chargeValues.length; i++) out.writeFloat(chargeValues[i]);
      }
      out.writePTag(this);
   }
   
}
