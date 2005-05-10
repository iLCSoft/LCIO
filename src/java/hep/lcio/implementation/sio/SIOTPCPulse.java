package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;
import hep.lcio.event.TPCCorrectedData;
import hep.lcio.event.TPCPulse;
import hep.lcio.implementation.event.ITPCPulse;
import java.io.IOException;

/**
 *
 * @author tonyj
 * @version $Id: SIOTPCPulse.java,v 1.1 2005-05-10 01:12:54 tonyj Exp $
 */
public class SIOTPCPulse extends ITPCPulse
{
   private SIORef data;
   public SIOTPCPulse(SIOInputStream in, int flags, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);
      
      channel = in.readInt();
      time = in.readFloat();
      charge = in.readFloat();
      quality = in.readInt();
      data = in.readPntr();
      in.readPTag(this);
   }
   static void write(TPCPulse hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOTPCPulse)
         ((SIOTPCPulse) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getChannelID());
         out.writeFloat(hit.getTime());
         out.writeFloat(hit.getCharge());
         out.writeInt(hit.getQuality());
         out.writePntr(hit.getTPCCorrectedData());
         out.writePTag(hit);
      }
   }
   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(channel);
      out.writeFloat(time);
      out.writeFloat(charge);
      out.writeInt(quality);
      out.writePntr(correctedData);
      out.writePTag(this);
   }

   public TPCCorrectedData getTPCCorrectedData()
   {
      if (correctedData == null && data != null)
      {
         correctedData = (TPCCorrectedData) data.getObject();
         data = null;
      }
      return super.getTPCCorrectedData();
   }
}
