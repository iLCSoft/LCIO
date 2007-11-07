package hep.lcio.implementation.sio;

import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;
import hep.lcio.event.LCIO;
import hep.lcio.event.RawCalorimeterHit;
import hep.lcio.implementation.event.IRawCalorimeterHit;

import java.io.IOException;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIORawCalorimeterHit.java,v 1.2 2007-11-07 20:46:23 jeremy Exp $
 */
class SIORawCalorimeterHit extends IRawCalorimeterHit
{
   SIORawCalorimeterHit(SIOInputStream in, int flags, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);
      cellId0 = in.readInt();
      if ((flags & (1 << LCIO.RCHBIT_ID1)) != 0)  cellId1 = in.readInt();
      else cellId1 = 0;
      amplitude = in.readInt();
      if ((flags & (1 << LCIO.RCHBIT_TIME)) != 0) timeStamp = in.readInt();
      if ((flags & (1 << LCIO.RCHBIT_NO_PTR)) == 0 )  in.readPTag(this) ;
   }
   
   static void write(RawCalorimeterHit hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIORawCalorimeterHit)
         ((SIORawCalorimeterHit) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getCellID0());
         if ((flags & (1 << LCIO.RCHBIT_ID1)) != 0) out.writeInt(hit.getCellID1());
         out.writeInt(hit.getAmplitude());
         if ((flags & (1 << LCIO.RCHBIT_TIME)) != 0) out.writeInt(hit.getTimeStamp());
         if ((flags & (1 << LCIO.RCHBIT_NO_PTR)) == 0) out.writePTag(hit);
      }
   }
   
   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(cellId0);
      if ((flags & (1 << LCIO.RCHBIT_ID1)) != 0) out.writeInt(cellId1);
      out.writeInt(amplitude);
      if ((flags & (1 << LCIO.RCHBIT_TIME)) != 0) out.writeInt(timeStamp);
      if ((flags & (1 << LCIO.RCHBIT_NO_PTR)) == 0) out.writePTag(this);
   }
}
