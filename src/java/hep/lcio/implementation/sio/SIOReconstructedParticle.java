package hep.lcio.implementation.sio;

import hep.lcio.event.ParticleID;
import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;
import hep.io.sio.SIORef;
import hep.lcio.event.ReconstructedParticle;
import hep.lcio.event.Vertex;
import hep.lcio.implementation.event.IReconstructedParticle;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author tonyj
 * @version $Id: SIOReconstructedParticle.java,v 1.7 2007-11-07 20:46:23 jeremy Exp $
 */
class SIOReconstructedParticle extends IReconstructedParticle
{
   private List tempParticles;
   private List tempTracks;
   private List tempClusters;
   
   /** Creates a new instance of SIOReconstructedParticle */
   public SIOReconstructedParticle(SIOInputStream in, SIOEvent owner, int flag, int major, int minor) throws IOException
   {
      // Note, I have not bothered with supporting intermediate "beta" formats.
      // This could be added later if necessary.
      this.type = in.readInt();
      for (int i=0; i<3; i++) this.momentum[i] = in.readFloat();
      this.energy = in.readFloat();
      for (int i=0; i<10; i++) this.covMatrix[i] = in.readFloat();
      this.mass = in.readFloat();
      this.charge = in.readFloat();
      for (int i=0; i<3; i++) this.referencePoint[i] = in.readFloat();
      int nPid = in.readInt();
      this.particleIDs = new ArrayList(nPid);
      for (int i=0; i<nPid; i++)
      {
         ParticleID id = new SIOParticleID(in,owner,flag,major,minor);
         particleIDs.add(id);
         in.readPTag(id);
      }
      this.particleIDUsed = (ParticleID) in.readPntr().getObject();
      goodnessOfPID = in.readFloat();
      int nReco = in.readInt();
      tempParticles = new ArrayList(nReco);
      this.particles = null;
      for (int i=0; i<nReco; i++)
      {
         tempParticles.add(in.readPntr());
      }
      int nTracks = in.readInt();
      tempTracks = new ArrayList(nTracks);
      this.tracks = null;
      for (int i=0; i<nTracks; i++)
      {
         tempTracks.add(in.readPntr());
      }
      int nClust = in.readInt();
      tempClusters = new ArrayList(nClust);
      this.clusters = null;
      for (int i=0; i<nClust; i++)
      {
         tempClusters.add(in.readPntr());
      }
      if( SIOVersion.encode(major,minor) > SIOVersion.encode(1,7))
      {
    	  this.startVertex = (Vertex) in.readPntr().getObject();
      }
      in.readPTag(this);
   }
   static void write(ReconstructedParticle particle, SIOOutputStream out, int flag) throws IOException
   {
      if (particle instanceof SIOReconstructedParticle)
         ((SIOReconstructedParticle) particle).write(out,flag);
      else
      {
         out.writeInt(particle.getType());
         double[] mom = particle.getMomentum();
         for (int i=0; i<3; i++) out.writeFloat( (float) mom[i]);
         out.writeFloat((float) particle.getEnergy());
         float[] matrix = particle.getCovMatrix();
         for (int i=0; i<10; i++) out.writeFloat(matrix[i]);
         out.writeFloat((float) particle.getMass());
         out.writeFloat(particle.getCharge());
         float[] ref = particle.getReferencePoint();
         for (int i=0; i<3; i++) out.writeFloat(ref[i]);
         List ids = particle.getParticleIDs();
         out.writeInt(ids.size());
         for (Iterator i = ids.iterator(); i.hasNext(); )
         { 
            ParticleID pid = (ParticleID) i.next();
            SIOParticleID.write(pid,out);
            out.writePTag(pid);
         }
         out.writePntr(particle.getParticleIDUsed());
         out.writeFloat(particle.getGoodnessOfPID());
         List particles = particle.getParticles();
         out.writeInt(particles.size());
         for (Iterator i = particles.iterator(); i.hasNext(); ) out.writePntr(i.next());
         List tracks = particle.getTracks();
         out.writeInt(tracks.size());
         for (Iterator i = tracks.iterator(); i.hasNext(); ) out.writePntr(i.next());
         List clusters = particle.getClusters();
         out.writeInt(clusters.size());
         for (Iterator i = clusters.iterator(); i.hasNext(); ) out.writePntr(i.next());
         out.writePntr(particle.getStartVertex());
         out.writePTag(particle);
      }
   }
   private void write(SIOOutputStream out, int flag) throws IOException
   {
      out.writeInt(type);
      for (int i=0; i<3; i++) out.writeFloat((float)momentum[i]);
      out.writeFloat((float)energy);
      for (int i=0; i<10; i++) out.writeFloat(covMatrix[i]);
      out.writeFloat((float)mass);
      out.writeFloat(charge);
      for (int i=0; i<3; i++) out.writeFloat(referencePoint[i]);
      out.writeInt(particleIDs.size());
      for (Iterator i = particleIDs.iterator(); i.hasNext(); ) 
      {
         ParticleID pid = (ParticleID) i.next();
         SIOParticleID.write(pid,out);
         out.writePntr(pid);
      }
      out.writePntr(particleIDUsed);
      out.writeFloat(goodnessOfPID);
      out.writeInt(particles.size());
      for (Iterator i = particles.iterator(); i.hasNext(); ) out.writePntr(i.next());
      out.writeInt(tracks.size());
      for (Iterator i = tracks.iterator(); i.hasNext(); ) out.writePntr(i.next());
      out.writeInt(clusters.size());
      for (Iterator i = clusters.iterator(); i.hasNext(); ) out.writePntr(i.next());
      out.writePntr(startVertex);
      out.writePTag(this);
   }
   
   public List getClusters()
   {      
      if (clusters == null && tempClusters != null)
      {
         clusters = new ArrayList(tempClusters.size());
         for (Iterator i = tempClusters.iterator(); i.hasNext();)
         {
            clusters.add(((SIORef) i.next()).getObject());
         }
         tempClusters = null;
      }
      return super.getClusters();
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
   
   public List getParticles()
   {
      if (particles == null && tempParticles != null)
      {
         particles = new ArrayList(tempTracks.size());
         for (Iterator i = tempParticles.iterator(); i.hasNext();)
         {
            particles.add(((SIORef) i.next()).getObject());
         }
         tempParticles = null;
      }
      return super.getParticles();
   }
   
}
