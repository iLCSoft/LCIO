package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;

import hep.lcio.data.CalorimeterHitData;
import hep.lcio.event.CalorimeterHit;
import hep.lcio.event.LCIO;

import hep.lcio.implementation.event.ICalorimeterHit;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOCalorimeterHit.java,v 1.5 2003-08-22 13:55:09 gaede Exp $
 */
class SIOCalorimeterHit extends ICalorimeterHit
{
   SIOCalorimeterHit(SIOInputStream in, int flags) throws IOException
   {
      cellId0 = in.readInt();
      cellId1 = in.readInt();
      energy = in.readFloat();

      if ((flags & (1<<LCIO.CHBIT_LONG)) != 0)
	  {
	      position[0] = in.readFloat();
	      position[1] = in.readFloat();
	      position[2] = in.readFloat();

	  }

      in.readPTag(this);

   }

   static void write(CalorimeterHitData hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOCalorimeterHit)
         ((SIOCalorimeterHit) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getCellID0());
         out.writeInt(hit.getCellID1());
         out.writeFloat(hit.getEnergy());

	 if ((flags & (1<<LCIO.CHBIT_LONG) ) != 0)
	     {
		 float[] pos = hit.getPosition();
		 out.writeFloat(pos[0]);
		 out.writeFloat(pos[1]);
		 out.writeFloat(pos[2]);

	     }

         out.writePTag(hit);
      }

   }

   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(cellId0);
      out.writeInt(cellId1);
      out.writeFloat(energy);

      if ((flags & (1<<LCIO.CHBIT_LONG)) != 0)
	  {
	      out.writeFloat(position[0]);
	      out.writeFloat(position[1]);
	      out.writeFloat(position[2]);
	  }

      out.writePTag(this);

   }
}
