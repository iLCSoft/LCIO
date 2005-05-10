package hep.lcio.implementation.event;
import hep.lcio.event.TPCCorrectedData;

/**
 *
 * @author tonyj
 * @version $Id: ITPCCorrectedData.java,v 1.1 2005-05-10 01:12:55 tonyj Exp $
 */
public class ITPCCorrectedData extends ILCObject implements TPCCorrectedData
{  
   protected int channelID;
   protected int time0;
   protected float[] charge;
   
   public int getChannelID()
   {
      return channelID;
   }

   public float[] getCharge()
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

   public void setCharge(float[] charge)
   {
      checkAccess();
      this.charge = charge;
   }
   
}
