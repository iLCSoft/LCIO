package hep.lcio.implementation.event;

import hep.lcio.event.Cluster;
import hep.lcio.event.ParticleID;
import hep.lcio.event.ReconstructedParticle;
import hep.lcio.event.Track;

import java.util.List;


/**Implementation of ReconstructedParticle
 * @author gaede
 *
 */
public class IReconstructedParticle extends ILCObject implements ReconstructedParticle
{
   protected ParticleID particleIDUsed;
   protected List clusters;
   protected List particleIDs;
   protected List particles;
   protected List tracks;
   protected float[] covMatrix;
   protected float[] momentum = new float[3];
   protected float[] referencePoint = new float[3];
   protected float charge;
   protected float energy;
   protected float mass;
   protected int type;

   public void setCharge(float charge)
   {
      checkAccess();
      this.charge = charge;
   }

   public float getCharge()
   {
      return charge;
   }

   public void setClusters(List clusters)
   {
      checkAccess();
      this.clusters = clusters;
   }

   public List getClusters()
   {
      return clusters;
   }

   public boolean isCompound()
   {
      return !particles.isEmpty();
   }

   public void setCovMatrix(float[] cov)
   {
      if (cov.length != 10) throw new IllegalArgumentException();
      checkAccess();
      this.covMatrix = cov;
   }

   public float[] getCovMatrix()
   {
      return covMatrix;
   }

   public void setEnergy(float e)
   {
      checkAccess();
      this.energy = e;
   }

   public float getEnergy()
   {
      return energy;
   }

   public void setMass(float f)
   {
      mass = f;
   }

   public float getMass()
   {
      return mass;
   }

   public void setMomentum(float[] fs)
   {
      if (fs.length != 3) throw new IllegalArgumentException();
      checkAccess();
      momentum = fs;
   }

   public float[] getMomentum()
   {
      return momentum;
   }

   public ParticleID getParticleIDUsed()
   {
      return particleIDUsed;
   }

   public List getParticleIDs()
   {
      return particleIDs;
   }

   public List getParticles()
   {
      return particles;
   }

   public void setReferencePoint(float[] fs)
   {
      if (fs.length != 3) throw new IllegalArgumentException();
      checkAccess();
      referencePoint = fs;
   }

   public float[] getReferencePoint()
   {
      return referencePoint;
   }

   public List getTracks()
   {
      return tracks;
   }

   public void setType(int type)
   {
      checkAccess();
      this.type = type;
   }

   public int getType()
   {
      return type;
   }

   public void addCluster(Cluster cluster)
   {
      addCluster(cluster);
   }

   public void addParticle(ReconstructedParticle particle)
   {
      addParticle(particle);
   }

   public void addParticleID(ParticleID pid)
   {
      particleIDs.add(pid);
   }

   public void addTrack(Track track)
   {
      addTrack(track);
   }
}
