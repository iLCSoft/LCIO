package hep.lcio.implementation.sio;

import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;

import hep.lcio.event.LCIO;
import hep.lcio.event.LCRunHeader;

import hep.lcio.implementation.event.ILCRunHeader;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIORunHeader.java,v 1.9 2007-11-07 20:46:23 jeremy Exp $
 */
class SIORunHeader extends ILCRunHeader
{
   SIORunHeader(SIOInputStream in,int major,int minor) throws IOException
   {
      runNumber = in.readInt();
      detectorName = in.readString();
      description = in.readString();

      int n = in.readInt();
      activeSubdetectors = new String[n];
      for (int i = 0; i < n; i++)
         activeSubdetectors[i] = in.readString();
   
	if( (major<<16 | minor ) > (1<<16|1)  ){
      parameters = new SIOLCParameters(in) ;
	} 
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
 
//		if( (LCIO.MAJORVERSION<<16 | LCIO.MINORVERSION ) > (1<<16|1)  ){
         SIOLCParameters.write( header.getParameters() , out ) ;
//		}
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
//	  if( (LCIO.MAJORVERSION<<16 | LCIO.MINORVERSION ) > (1<<16|1)  ){
   	   SIOLCParameters.write( parameters , out ) ;
//	  }
   }
}
