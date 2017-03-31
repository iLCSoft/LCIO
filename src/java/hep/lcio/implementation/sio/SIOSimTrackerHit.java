package hep.lcio.implementation.sio;

import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;
import hep.io.sio.SIORef;


import hep.lcio.event.LCIO;
import hep.lcio.event.MCParticle;
import hep.lcio.event.SimTrackerHit;

import hep.lcio.implementation.event.ISimTrackerHit;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOSimTrackerHit.java,v 1.16 2010-06-02 10:59:34 engels Exp $
 */
class SIOSimTrackerHit extends ISimTrackerHit
{
   SIOSimTrackerHit(SIOInputStream in,int flags, SIOEvent owner,  int major, int minor) throws IOException
   {
      setParent(owner);
      cellID0 = in.readInt();
      if( SIOVersion.encode(major,minor) > SIOVersion.encode(1,51) ){
          if( (flags & (1 << LCIO.THBIT_ID1)) != 0 ){
            cellID1 = in.readInt();
          }
      }
      position[0] = in.readDouble();
      position[1] = in.readDouble();
      position[2] = in.readDouble();
      //dEdx = in.readFloat();
      EDep = in.readFloat();
      time = in.readFloat();
      particle = in.readPntr();
      
      int version = SIOVersion.encode(major,minor); 
      
      if ((flags & (1 << LCIO.THBIT_MOMENTUM)) != 0)
      {
         momentum[0] = in.readFloat();
         momentum[1] = in.readFloat();
         momentum[2] = in.readFloat();
         
         if (version > SIOVersion.encode(1,6)) pathLength = in.readFloat();
      }

      quality = 0 ;
      if( SIOVersion.encode(major,minor) > SIOVersion.encode(2,7))
        quality = in.readInt();

      if( version > SIOVersion.encode(1,0)) in.readPTag(this);
   }
   
   public MCParticle getMCParticle()
   {
      if (particle instanceof SIORef)
         particle = ((SIORef) particle).getObject();
      return (MCParticle) particle;
   }
   
   static void write(SimTrackerHit hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOSimTrackerHit)
         ((SIOSimTrackerHit) hit).write(out,flags);
      else
      {
         out.writeInt(hit.getCellID0());
         if( (flags & (1 << LCIO.THBIT_ID1)) != 0 ){
            out.writeInt(hit.getCellID1());
         }
         
         double[] pos = hit.getPosition();
         out.writeDouble(pos[0]);
         out.writeDouble(pos[1]);
         out.writeDouble(pos[2]);
         //out.writeFloat(hit.getdEdx());
         out.writeFloat(hit.getEDep());
         out.writeFloat(hit.getTime());
         out.writePntr(hit.getMCParticle());
         if ((flags & (1 << LCIO.THBIT_MOMENTUM)) != 0)
         {
            float[] p = hit.getMomentum() ;
            out.writeFloat(p[0] ) ;
            out.writeFloat(p[1] ) ;
            out.writeFloat(p[2] ) ;
            out.writeFloat(hit.getPathLength());
         }

         out.writeInt(hit.getQuality());

         out.writePTag(hit);
      }
   }
   
   private void write(SIOOutputStream out,int flags) throws IOException
   {
      out.writeInt(getCellID0());
      if( (flags & (1 << LCIO.THBIT_ID1)) != 0 ){
        out.writeInt(getCellID1());
      }
      out.writeDouble(position[0]);
      out.writeDouble(position[1]);
      out.writeDouble(position[2]);
      //out.writeFloat(dEdx);
      out.writeFloat(EDep);
      out.writeFloat(time);
      out.writePntr(getMCParticle());
      if ((flags & (1 << LCIO.THBIT_MOMENTUM)) != 0)
      {
         out.writeFloat(momentum[0] ) ;
         out.writeFloat(momentum[1] ) ;
         out.writeFloat(momentum[2] ) ;
         out.writeFloat(pathLength);
      }
      out.writeInt(quality);
      out.writePTag(this);
   }
}
