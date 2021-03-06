package hep.lcio.implementation.sio;

import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;
import hep.io.sio.SIORef;

import hep.lcio.event.LCIO;
import hep.lcio.event.TrackerHitPlane;

import hep.lcio.implementation.event.ITrackerHitPlane;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.ListIterator;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOTrackerHitPlane.java,v 1.16 2010-06-02 10:59:34 engels Exp $
 */
class SIOTrackerHitPlane extends ITrackerHitPlane
{
   private List tempHits;
   SIOTrackerHitPlane(SIOInputStream in, int flags, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);
      
      if( SIOVersion.encode(major,minor) > SIOVersion.encode(1,51)){
          cellID0 = in.readInt();
          if( (flags & (1 << LCIO.RTHPBIT_ID1)) != 0 ){
              cellID1 = in.readInt();
          }
      }

      type = in.readInt();
      for (int i = 0; i < 3; i++)
         position[i] = in.readDouble();
      //for (int i = 0; i < 6; i++)
      //   covMatrix[i] = in.readFloat();
      //dEdx = in.readFloat();
      u[0] = in.readFloat();
      u[1] = in.readFloat();
      v[0] = in.readFloat();
      v[1] = in.readFloat();
      dU = in.readFloat();
      dV = in.readFloat();
      EDep = in.readFloat();
      EDepError = in.readFloat();
      time = in.readFloat();
      quality = in.readInt();
      
      int nRawHits = in.readInt() ;
      
      tempHits = new ArrayList( nRawHits ) ;
      for (int i = 0; i < nRawHits ; i++)
      {
         tempHits.add(in.readPntr());
      }
      rawHits = null;
      
      in.readPTag(this);
   }
   public List getRawHits()
   {
      if (rawHits == null && tempHits != null)
      {
         for (ListIterator i = tempHits.listIterator(); i.hasNext(); )
         {
            SIORef ref = (SIORef) i.next();
            i.set(ref.getObject());
         }
         rawHits = tempHits;
         tempHits = null;
      }
      return super.getRawHits();
   }   
   
   static void write(TrackerHitPlane hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOTrackerHitPlane)
      {
         ((SIOTrackerHitPlane) hit).write(out,flags);
      }
      else
      {
         out.writeInt(hit.getCellID0());
         if( (flags & (1 << LCIO.RTHPBIT_ID1)) != 0 ){
            out.writeInt(hit.getCellID1());
         }
         out.writeInt(hit.getType());
         double[] pos = hit.getPosition();
         for (int i = 0; i < 3; i++)
            out.writeDouble(pos[i]);
         
         //float[] matrix = hit.getCovMatrix();
         //for (int i = 0; i < 6; i++)
         //   out.writeFloat(matrix[i]);
         out.writeFloat(hit.getU()[0]);
         out.writeFloat(hit.getU()[1]);
         out.writeFloat(hit.getV()[0]);
         out.writeFloat(hit.getV()[1]);
         out.writeFloat(hit.getdU());
         out.writeFloat(hit.getdV());
         out.writeFloat(hit.getEDep());
         out.writeFloat(hit.getEDepError());
         out.writeFloat(hit.getTime());
         out.writeInt(hit.getQuality());
         
         List rawHits = hit.getRawHits() ;
         out.writeInt( rawHits.size()) ;
         for (int i = 0; i < rawHits.size() ; i++)
         {
            out.writePntr( rawHits.get(i) );
         }
         
         out.writePTag(hit);
      }
   }
   
   private void write(SIOOutputStream out, int flags) throws IOException
   {
     out.writeInt(getCellID0());
     if( (flags & (1 << LCIO.RTHPBIT_ID1)) != 0 ){
        out.writeInt(getCellID1());
     }

      out.writeInt(type);
      for (int i = 0; i < 3; i++)
         out.writeDouble(position[i]);
      //for (int i = 0; i < 6; i++)
      //   out.writeFloat(covMatrix[i]);
      out.writeFloat(u[0]);
      out.writeFloat(u[1]);
      out.writeFloat(v[0]);
      out.writeFloat(v[1]);
      out.writeFloat(dU);
      out.writeFloat(dV);
      out.writeFloat(EDep);
      out.writeFloat(EDepError);
      out.writeFloat(time);
      out.writeInt(quality);
      
      out.writeInt( rawHits.size()) ;
      for (int i = 0; i < rawHits.size() ; i++)
      {
         out.writePntr( rawHits.get(i) );
      }
      out.writePTag(this);
   }   
}
