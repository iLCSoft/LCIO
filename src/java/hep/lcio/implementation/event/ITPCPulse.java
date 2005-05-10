package hep.lcio.implementation.event;

import hep.lcio.event.TPCCorrectedData;
import hep.lcio.event.TPCPulse;

/**
 *
 * @author tonyj
 * @version $Id: ITPCPulse.java,v 1.1 2005-05-10 01:12:55 tonyj Exp $
 */
public class ITPCPulse extends ILCObject implements TPCPulse
{
   protected int channel;
   protected float charge;
   protected int quality;
   protected TPCCorrectedData correctedData;
   protected float time;
   
   public int getChannelID()
   {
      return channel;
   }
   
   public float getCharge()
   {
      return charge;
   }
   
   public int getQuality()
   {
      return quality;
   }
   
   public TPCCorrectedData getTPCCorrectedData()
   {
      return correctedData;
   }
   
   public float getTime()
   {
      return time;
   }
   
   public int getChannel()
   {
      return channel;
   }
   
   public void setChannelID(int channel)
   {
      checkAccess();
      this.channel = channel;
   }
   
   public void setCharge(float charge)
   {
      checkAccess();
      this.charge = charge;
   }
   
   public void setQuality(int quality)
   {
      checkAccess();
      this.quality = quality;
   }
   
   
   public void setTPCCorrectedData(TPCCorrectedData correctedData)
   {
      checkAccess();
      this.correctedData = correctedData;
   }
   
   public void setTime(float time)
   {
      checkAccess();
      this.time = time;
   }
}
