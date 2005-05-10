package hep.lcio.implementation.sio;
import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcio.event.TPCRawData;
import hep.lcio.implementation.event.ITPCRawData;
import java.io.IOException;

/**
 *
 * @author tonyj
 * @version $Id: SIOTPCRawData.java,v 1.2 2005-05-10 02:11:29 tonyj Exp $
 */
class SIOTPCRawData extends ITPCRawData
{
   
   public SIOTPCRawData(SIOInputStream in, int flags, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);

      channelID = in.readInt();
      time0 = in.readInt();
      int n = in.readInt();
      if (n > 0)
      {
         charge = new short[n];
         for (int i=0; i<n; i++) charge[i] = in.readShort();
      }
      in.readPTag(this);
   }
   static void write(TPCRawData hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOTPCRawData)
         ((SIOTPCRawData) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getChannelID());
         out.writeInt(hit.getTime0());
         short[] c = hit.getCharge();
         if (c == null)
         {
            out.writeInt(0);
         }
         else
         {
            out.writeInt(c.length);
            for (int i=0; i<c.length; i++) out.writeShort(c[i]);
         }
         out.writePTag(hit);
      }
   }
   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(channelID);
      out.writeInt(time0);
      if (charge == null)
      {
         out.writeInt(0);
      }
      else
      {
         out.writeInt(charge.length);
         for (int i=0; i<charge.length; i++) out.writeShort(charge[i]);
      }
      out.writePTag(this);
   }
   
}
