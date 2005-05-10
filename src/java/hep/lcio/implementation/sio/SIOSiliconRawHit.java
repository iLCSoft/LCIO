package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcio.event.SiliconRawHit;
import hep.lcio.implementation.event.ISiliconRawHit;
import java.io.IOException;

/**
 *
 * @author tonyj
 * @version $Id: SIOSiliconRawHit.java,v 1.1 2005-05-10 01:12:54 tonyj Exp $
 */
class SIOSiliconRawHit extends ISiliconRawHit
{
   public SIOSiliconRawHit(SIOInputStream in, int flags, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);
      cellID0 = in.readInt();
      cellID1 = in.readInt();
      timeStamp = in.readInt();
      adcCount = in.readInt();
      in.readPTag(this);
   }
   static void write(SiliconRawHit hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOSiliconRawHit)
         ((SIOSiliconRawHit) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getCellID0());
         out.writeInt(hit.getCellID1());
         out.writeInt(hit.getTimeStamp());
         out.writeInt(hit.getADCCounts());
         out.writePTag(hit);
      }
   }
   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(cellID0);
      out.writeInt(cellID1);
      out.writeInt(timeStamp);
      out.writeInt(adcCount);
      out.writePTag(this);
   }
}
