package hep.lcio.implementation.sio;

import hep.lcio.event.ParticleID;
import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcio.event.ReconstructedParticle;
import hep.lcio.implementation.event.IReconstructedParticle;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author tonyj
 * @version $Id: SIOReconstructedParticle.java,v 1.2 2004-09-16 07:15:34 gaede Exp $
 */
class SIOReconstructedParticle extends IReconstructedParticle
{
   
   /** Creates a new instance of SIOReconstructedParticle */
   public SIOReconstructedParticle(SIOInputStream in, SIOEvent owner, int flag, int major, int minor) throws IOException
   {
      // Note, I have not bothered with supporting intermediate "beta" formats.
      // This could be added later if necessarty.
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
      }
      this.particleIDUsed = (ParticleID) in.readPntr();
	  goodnessOfPID = in.readFloat();
      int nReco = in.readInt();
      this.particles = new ArrayList(nReco);
      for (int i=0; i<nReco; i++)
      {
         particles.add(in.readPntr());
      }
      int nTracks = in.readInt();
      this.tracks = new ArrayList(nTracks);
      for (int i=0; i<nTracks; i++)
      {
         tracks.add(in.readPntr());
      }
      int nClust = in.readInt();
      this.clusters = new ArrayList(nClust);
      for (int i=0; i<nClust; i++)
      {
         clusters.add(in.readPntr());
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
         float[] mom = particle.getMomentum();
         for (int i=0; i<3; i++) out.writeFloat(mom[i]);
         out.writeFloat(particle.getEnergy());
         float[] matrix = particle.getCovMatrix();
         for (int i=0; i<10; i++) out.writeFloat(matrix[i]);
         out.writeFloat(particle.getMass());
         out.writeFloat(particle.getCharge());
         float[] ref = particle.getReferencePoint();
         for (int i=0; i<3; i++) out.writeFloat(ref[i]);
         List ids = particle.getParticleIDs();
         out.writeInt(ids.size());
         for (Iterator i = ids.iterator(); i.hasNext(); ) SIOParticleID.write((ParticleID) i.next(),out);
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
         out.writePTag(particle);
      }
   }
   private void write(SIOOutputStream out, int flag) throws IOException
   {
      out.writeInt(type);
      for (int i=0; i<3; i++) out.writeFloat(momentum[i]);
      out.writeFloat(energy);
      for (int i=0; i<10; i++) out.writeFloat(covMatrix[i]);
      out.writeFloat(mass);
      out.writeFloat(charge);
      for (int i=0; i<3; i++) out.writeFloat(referencePoint[i]);
      out.writeInt(particleIDs.size());
      for (Iterator i = particleIDs.iterator(); i.hasNext(); ) SIOParticleID.write((ParticleID) i.next(),out);
      out.writePntr(particleIDUsed);
      out.writeFloat(goodnessOfPID);
      out.writeInt(particles.size());
      for (Iterator i = particles.iterator(); i.hasNext(); ) out.writePntr(i.next());
      out.writeInt(tracks.size());
      for (Iterator i = tracks.iterator(); i.hasNext(); ) out.writePntr(i.next());
      out.writeInt(clusters.size());
      for (Iterator i = clusters.iterator(); i.hasNext(); ) out.writePntr(i.next());
      out.writePTag(this);
   }   
}
