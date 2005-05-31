package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;
import hep.lcio.event.LCIO;
import hep.lcio.event.TrackerData;
import hep.lcio.event.TrackerPulse;
import hep.lcio.implementation.event.ITrackerPulse;
import java.io.IOException;

/**
 *
 * @author tonyj
 * @version $Id: SIOTrackerPulse.java,v 1.1 2005-05-31 07:43:31 gaede Exp $
 */
public class SIOTrackerPulse extends ITrackerPulse
{
   private SIORef data;
   public SIOTrackerPulse(SIOInputStream in, int flags, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);
      
      cellID0 = in.readInt();
      if ((flags & (1 << LCIO.TRAWBIT_ID1)) != 0)  cellID1 = in.readInt();
      else cellID1 = 0;

      time = in.readFloat();
      charge = in.readFloat();
      quality = in.readInt();
      data = in.readPntr();
      in.readPTag(this);
   }
   static void write(TrackerPulse hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOTrackerPulse)
         ((SIOTrackerPulse) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getCellID0());
         if ((flags & (1 << LCIO.TRAWBIT_ID1)) != 0) out.writeInt(hit.getCellID1()); 
         out.writeFloat(hit.getTime());
         out.writeFloat(hit.getCharge());
         out.writeInt(hit.getQuality());
         out.writePntr(hit.getTrackerData());
         out.writePTag(hit);
      }
   }
   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(cellID0);
      if ((flags & (1 << LCIO.TRAWBIT_ID1)) != 0) out.writeInt(cellID1);
      out.writeFloat(time);
      out.writeFloat(charge);
      out.writeInt(quality);
      out.writePntr(correctedData);
      out.writePTag(this);
   }

   public TrackerData getTrackerData()
   {
      if (correctedData == null && data != null)
      {
         correctedData = (TrackerData) data.getObject();
         data = null;
      }
      return super.getTrackerData();
   }
}
