package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;


import hep.lcio.event.LCIO;

import hep.lcio.data.TPCHitData;
import hep.lcio.event.TPCHit;
import hep.lcio.implementation.event.ITPCHit;

import java.io.IOException;

import org.apache.xml.utils.RawCharacterHandler;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOTPCHit.java,v 1.2 2003-09-15 21:44:33 tonyj Exp $
 */
class SIOTPCHit extends ITPCHit
{
   SIOTPCHit(SIOInputStream in, int flags, SIOEvent owner) throws IOException
   {
      setParent(owner);
      cellID = in.readInt();
      time = in.readFloat();
      charge = in.readFloat();
      quality = in.readInt();

      if ((flags & (1 << LCIO.TPCBIT_RAW)) != 0)
      {
	     int size = in.readInt() ;
         rawDataArray = new int[ size ] ;
	     for (int i = 0; i < size; i++) {
	        rawDataArray[i] = in.readInt() ;		
		}
      }
   }

   static void write(TPCHitData hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOTPCHit)
         ((SIOTPCHit) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getCellID());
         out.writeFloat(hit.getTime());
         out.writeFloat(hit.getCharge());
         out.writeInt(hit.getQuality());

         if ((flags & (1 << LCIO.TPCBIT_RAW)) != 0)
         {
           out.writeInt( hit.getNRawDataWords() );
           for (int i = 0; i < hit.getNRawDataWords() ; i++) {
			 out.writeInt( hit.getRawDataWord(i) ) ;
		   }            
         }
      }
   }

   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(cellID);
      out.writeFloat(time);
      out.writeFloat(charge);
      out.writeInt(quality);

      if ((flags & (1 << LCIO.TPCBIT_RAW)) != 0)
      {
         out.writeInt(rawDataArray.length);
	     for (int i = 0; i < rawDataArray.length; i++) {
           out.writeInt( rawDataArray[i]);
	     }
      }
   }
}
