package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;

import hep.lcio.event.CalorimeterHit;
import hep.lcio.event.LCIO;
import hep.lcio.event.LCObject;

import hep.lcio.implementation.event.ICalorimeterHit;

import java.io.IOException;

// FIXME: need to add time word !

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOCalorimeterHit.java,v 1.16 2004-09-23 17:07:40 gaede Exp $
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
	 
	 
	 if( SIOVersion.encode(major,minor) > SIOVersion.encode(1,2)){
       type = in.readInt() ;
       rawHit =  (LCObject) ((SIORef) in.readPntr()).getObject() ;
  	 
	 	// the logic has been reverted in v1.3 !
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
	 
	     out.writeInt( hit.getType() ) ;
	     out.writePntr( hit.getRawHit()) ;
	     
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
      out.write(type);
      out.writePntr( rawHit ) ;
      
      if ((flags & (1 << LCIO.RCHBIT_NO_PTR)) == 0)
	  out.writePTag(this) ;
   }
}
