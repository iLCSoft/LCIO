package hep.lcio.implementation.sio;

import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;
import hep.io.sio.SIORef;
import hep.lcio.event.Track;
import hep.lcio.event.TrackerHit;
import hep.lcio.implementation.event.ITrack;
import hep.lcio.implementation.event.ITrackerHit;
import hep.lcio.event.LCIO;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOTrack.java,v 1.12 2007-11-07 20:46:23 jeremy Exp $
 */
class SIOTrack extends ITrack
{
   private List tempHits;
   private List tempTracks;
   SIOTrack(SIOInputStream in, SIOEvent owner, int flag, int major, int minor) throws IOException
   {
      int typeWord = in.readInt();
      setType( typeWord) ;
      
      d0 = in.readFloat();
      phi = in.readFloat();
      omega = in.readFloat();
      z0 = in.readFloat();
      tanLambda = in.readFloat();
      covMatrix = new float[15];
      for (int i=0; i<covMatrix.length; i++) covMatrix[i] = in.readFloat();
      referencePoint = new float[3];
      referencePoint[0] = in.readFloat();
      referencePoint[1] = in.readFloat();
      referencePoint[2] = in.readFloat();
      chi2 = in.readFloat();
      ndf = in.readInt() ;
      dEdx = in.readFloat();
      dEdxError = in.readFloat();
      radiusOfInnermostHit = in.readFloat() ;
      int nHitNumbers = in.readInt() ;
      int[] hitNumbers = new int[nHitNumbers] ;
      for (int i = 0; i < nHitNumbers; i++)
      {
         hitNumbers[i] = in.readInt() ;
      }
      setSubdetectorHitNumbers(hitNumbers) ;
      int nTracks = in.readInt();
      tempTracks = new ArrayList(nTracks);
      tracks = null;
      for (int i=0; i<nTracks; i++)
      {
         tempTracks.add(in.readPntr());
      }
      if (bitTest(flag,LCIO.TRBIT_HITS))
      {
         int nHits = in.readInt();
         tempHits = new ArrayList(nHits);
         hits = null;
         for (int i = 0; i < nHits; i++)
         {
            tempHits.add(in.readPntr());
         }
      }
      
      in.readPTag(this);
   }
   static void write(Track track, SIOOutputStream out, int flag) throws IOException
   {
      if (track instanceof SIOTrack)
         ((SIOTrack) track).write(out,flag);
      else
      {
         out.writeInt(track.getType());
         out.writeFloat(track.getD0());
         out.writeFloat(track.getPhi());
         out.writeFloat(track.getOmega());
         out.writeFloat(track.getZ0());
         out.writeFloat(track.getTanLambda());
         float[] covMatrix = track.getCovMatrix();
         for (int i=0; i<covMatrix.length; i++) out.writeFloat(covMatrix[i]);
         float[] referencePoint = track.getReferencePoint();
         out.writeFloat(referencePoint[0]);
         out.writeFloat(referencePoint[1] );
         out.writeFloat(referencePoint[2]);
         out.writeFloat(track.getChi2());
         out.writeInt(track.getNdf());
         out.writeFloat(track.getdEdx());
         out.writeFloat(track.getdEdxError());
         out.writeFloat(track.getRadiusOfInnermostHit()) ;
         int[] hitNumbers = track.getSubdetectorHitNumbers();
         out.writeInt( hitNumbers.length ) ;
         for (int i = 0; i < hitNumbers.length; i++)
         {
            out.writeInt( hitNumbers[i] ) ;
         }
         List tracks = track.getTracks() ;
         out.writeInt( tracks.size()  ) ;
         for (Iterator iter = tracks.iterator(); iter.hasNext();)
         {
            out.writePntr( (Track) iter.next() );
         }
         if(bitTest(flag,LCIO.TRBIT_HITS))
         {
            List hits = track.getTrackerHits();
            out.writeInt(hits.size());
            for (Iterator iter = hits.iterator(); iter.hasNext();)
            {
               out.writePntr( (TrackerHit) iter.next() ) ;
            }
         }
         out.writePTag(track);
      }
   }
   private void write(SIOOutputStream out, int flag) throws IOException
   {
      out.writeInt( getType() );
      out.writeFloat(d0);
      out.writeFloat(phi);
      out.writeFloat(omega);
      out.writeFloat(z0);
      out.writeFloat(tanLambda);
      for (int i=0; i<covMatrix.length; i++) out.writeFloat(covMatrix[i]);
      out.writeFloat(referencePoint[0]);
      out.writeFloat(referencePoint[1] );
      out.writeFloat(referencePoint[2]);
      out.writeFloat(chi2);
      out.writeFloat(ndf);
      out.writeFloat(dEdx);
      out.writeFloat(dEdxError);
      out.writeFloat(radiusOfInnermostHit) ;
      out.writeInt( subdetectorHitNumbers.length ) ;
      for (int i = 0; i < subdetectorHitNumbers.length; i++)
      {
         out.writeInt( subdetectorHitNumbers[i] ) ;
      }
      if (tracks == null)
      {
    	  tracks = getTracks();
      }
      out.writeInt( tracks.size()  ) ;
      for (Iterator iter = tracks.iterator(); iter.hasNext();)
      {
         out.writePntr( (Track) iter.next() );
      }
      if(bitTest(flag,LCIO.TRBIT_HITS))
      {
         out.writeInt(hits.size());
         for (Iterator iter = hits.iterator(); iter.hasNext();)
         {
            out.writePntr(iter.next());
         }
      }
      out.writePTag(this);
   }
   
   public List getTrackerHits()
   {
      if (hits == null && tempHits != null)
      {
         hits = new ArrayList(tempHits.size());
         for (Iterator i = tempHits.iterator(); i.hasNext();)
         {
            hits.add(((SIORef) i.next()).getObject());
         }
         tempHits = null;
      }
      return super.getTrackerHits();
   }
   
   public List getTracks()
   {
      if (tracks == null && tempTracks != null)
      {
         tracks = new ArrayList(tempTracks.size());
         for (Iterator i = tempTracks.iterator(); i.hasNext();)
         {
            tracks.add(((SIORef) i.next()).getObject());
         }
         tempTracks = null;
      }
      return super.getTracks();
   } 
}
