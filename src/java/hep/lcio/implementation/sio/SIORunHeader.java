package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;

import hep.lcio.event.LCRunHeader;

import hep.lcio.implementation.event.ILCRunHeader;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIORunHeader.java,v 1.5 2004-04-08 09:58:02 gaede Exp $
 */
class SIORunHeader extends ILCRunHeader
{
   SIORunHeader(SIOInputStream in) throws IOException
   {
      runNumber = in.readInt();
      detectorName = in.readString();
      description = in.readString();

      int n = in.readInt();
      activeSubdetectors = new String[n];
      for (int i = 0; i < n; i++)
         activeSubdetectors[i] = in.readString();
   }

   static void write(LCRunHeader header, SIOOutputStream out) throws IOException
   {
      if (header instanceof SIORunHeader)
         ((SIORunHeader) header).write(out);
      else
      {
         out.writeInt(header.getRunNumber());
         out.writeString(header.getDetectorName());
         out.writeString(header.getDescription());

         String[] active = header.getActiveSubdetectors();
         out.writeInt(active.length);
         for (int i = 0; i < active.length; i++)
            out.writeString(active[i]);
      }
   }

   private void write(SIOOutputStream out) throws IOException
   {
      out.writeInt(runNumber);
      out.writeString(detectorName);
      out.writeString(description);
      out.writeInt(activeSubdetectors.length);
      for (int i = 0; i < activeSubdetectors.length; i++)
         out.writeString(activeSubdetectors[i]);
   }
}
