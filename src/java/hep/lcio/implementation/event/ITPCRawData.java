package hep.lcio.implementation.event;
import hep.lcio.event.TPCRawData;

/**
 *
 * @author tonyj
 * @version $Id: ITPCRawData.java,v 1.1 2005-05-10 01:12:55 tonyj Exp $
 */
public class ITPCRawData extends ILCObject implements TPCRawData
{  
   protected int channelID;
   protected int time0;
   protected short[] charge;
   
   public int getChannelID()
   {
      return channelID;
   }

   public short[] getCharge()
   {
      return charge;
   }

   public int getTime0()
   {
      return time0;
   }

   public void setChannelID(int channelID)
   {
      checkAccess();
      this.channelID = channelID;
   }

   public void setTime0(int time0)
   {
      checkAccess();
      this.time0 = time0;
   }

   public void setCharge(short[] charge)
   {
      checkAccess();
      this.charge = charge;
   }
   
}
