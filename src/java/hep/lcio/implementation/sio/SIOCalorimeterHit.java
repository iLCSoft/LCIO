package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;

import hep.lcio.event.CalorimeterHit;
import hep.lcio.event.LCIO;

import hep.lcio.implementation.event.ICalorimeterHit;

import java.io.IOException;

// FIXME: need to add time word !

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOCalorimeterHit.java,v 1.15 2004-09-09 15:57:51 gaede Exp $
 */
class SIOCalorimeterHit extends ICalorimeterHit
{
   SIOCalorimeterHit(SIOInputStream in, int flags, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);
      cellId0 = in.readInt();
      if ((flags & (1 << LCIO.RCHBIT_ID1)) != 0 || ( major == 0 && minor == 8) )  cellId1 = in.readInt();
      else cellId1 = 0;
      energy = in.readFloat();

      if ((flags & (1 << LCIO.RCHBIT_LONG)) != 0)
      {
         position[0] = in.readFloat();
         position[1] = in.readFloat();
         position[2] = in.readFloat();
      }
	 double version  = (double) major + ( (double) minor ) /  10. ;
	
	 if( version > 1.2 ) { // the logic has been reverted in v1.3 !
	     if ( (flags & (1 << LCIO.RCHBIT_NO_PTR)) == 0 )  in.readPTag(this) ;
	 } else {
	     if ( (flags & (1 << LCIO.RCHBIT_NO_PTR)) != 0 )  in.readPTag(this) ;
	 }

   }


   static void write(CalorimeterHit hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOCalorimeterHit)
         ((SIOCalorimeterHit) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getCellID0());
         if ((flags & (1 << LCIO.RCHBIT_ID1)) != 0) out.writeInt(hit.getCellID1());
         out.writeFloat(hit.getEnergy());

         if ((flags & (1 << LCIO.RCHBIT_LONG)) != 0)
         {
            float[] pos = hit.getPosition();
            out.writeFloat(pos[0]);
            out.writeFloat(pos[1]);
            out.writeFloat(pos[2]);
         }
	 if ((flags & (1 << LCIO.RCHBIT_NO_PTR)) == 0)
	     out.writePTag(hit) ;
      }
   }

   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(cellId0);
      if ((flags & (1 << LCIO.RCHBIT_ID1)) != 0) out.writeInt(cellId1);
      out.writeFloat(energy);

      if ((flags & (1 << LCIO.RCHBIT_LONG)) != 0)
      {
         out.writeFloat(position[0]);
         out.writeFloat(position[1]);
         out.writeFloat(position[2]);
      }
      if ((flags & (1 << LCIO.RCHBIT_NO_PTR)) == 0)
	  out.writePTag(this) ;
   }
}
