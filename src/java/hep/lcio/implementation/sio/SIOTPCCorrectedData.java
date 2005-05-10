package hep.lcio.implementation.sio;
import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcio.event.TPCCorrectedData;
import hep.lcio.implementation.event.ITPCCorrectedData;
import java.io.IOException;

/**
 *
 * @author tonyj
 * @version $Id: SIOTPCCorrectedData.java,v 1.1 2005-05-10 01:12:54 tonyj Exp $
 */
class SIOTPCCorrectedData extends ITPCCorrectedData
{
   
   public SIOTPCCorrectedData(SIOInputStream in, int flags, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);

      channelID = in.readInt();
      time0 = in.readInt();
      int n = in.readInt();
      charge = new float[n];
      for (int i=0; i<n; i++) charge[i] = in.readFloat();
      in.readPTag(this);
   }
   static void write(TPCCorrectedData hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOTPCCorrectedData)
         ((SIOTPCCorrectedData) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getChannelID());
         out.writeInt(hit.getTime0());
         float[] c = hit.getCharge();
         out.writeInt(c.length);
         for (int i=0; i<c.length; i++) out.writeFloat(c[i]);
         out.writePTag(hit);
      }
   }
   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(channelID);
      out.writeInt(time0);
      out.writeInt(charge.length);
      for (int i=0; i<charge.length; i++) out.writeFloat(charge[i]);
      out.writePTag(this);
   }
   
}
