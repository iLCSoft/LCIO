package hep.lcio.implementation.event;

import hep.lcio.event.Cluster;
import hep.lcio.event.MCParticle;
import hep.lcio.event.ParticleID;
import hep.lcio.event.ReconstructedParticle;
import hep.lcio.event.Track;
import java.util.List;

import java.util.Vector;

/**Implementation of ReconstructedParticle
 * @author gaede
 *
 */
public class IReconstructedParticle implements ReconstructedParticle
{
   public void addParticleID(ParticleID pid)
   {
      particleIDs.add( pid) ;
   }
   public void addParticle(ReconstructedParticle particle)
   {
      addParticle( particle ) ;
      
   }
//   public void addParticle(ReconstructedParticle particle, float weight)
//   {
//      particles.add( particle ) ;
//      //TODO: particleWeights.add( new Float(weight) ) ;
//   }

   public void addCluster(Cluster cluster)
   {
      addCluster( cluster  ) ;
   }

//   public void addCluster(Cluster cluster, float weight)
//   {
//      clusters.add( cluster  ) ;
//      //TODO: clusterWeights.add( new Float(weight)) ;
//      
//   }
   
   public void addTrack(Track track)
   {
      addTrack( track ) ;
   }
   
//   public void addTrack(Track track, float weight)
//   {
//      tracks.add( track ) ;
//      //TODO: trackWeights.add( new Float( weight) ) ;
//      
//   }

//   public void addMCParticle(MCParticle mcParticle)
//   {
//      addMCParticle( mcParticle, (float) 1. ) ;
//   }
//   
//   public void addMCParticle(MCParticle mcParticle, float weight)
//   {
//      mCParticles.add(mcParticle) ;
//      
//   }
   
   protected int type ;
//   protected boolean primary ;
   protected float[] momentum = new float[3] ;
   protected float[] referencePoint = new float[3] ;
   protected float mass ;
   protected float energy ;
   protected float charge ;
   protected Vector particleIDs ;
   protected ParticleID particleIDUsed ;  // Fixme: need IParticleID
//   protected Vector mCParticles ;
//   protected float[] mCParticleWeights ;
   protected Vector particles ;
//   protected float[] particleWeights ;
   protected Vector tracks ;
//   protected float[] trackWeights ;
   protected Vector clusters;
//   protected float[] clusterWeights ;
   protected float[] covMatrix ;
   
   public float[] getMomentum()
   {
      return momentum;
   }
   
//   public boolean isPrimary()
//  {
//      return primary;
//   }
   
   public int getType()
   {
      return type;
   }
   
   public void setMomentum(float[] fs)
   {
      momentum = fs;
   }
   
//   public void setPrimary(boolean b)
//   {
//      primary = b;
//   }
   
   public void setType(int i)
   {
      type = i;
   }
   
//   public List getMCParticles()
//   {
//      return mCParticles;
//   }
   
   public List getClusters()
   {
      return clusters;
   }
   
//   public float[] getClusterWeights()
//   {
//      return clusterWeights;
//   }
   
   public float getEnergy()
   {
      return energy;
   }

   public float getMass()
   {
      return mass;
   }
   
//   public float[] getMCParticleWeights()
//   {
//      return mCParticleWeights;
//   }
   
   public List getParticles()
   {
      return particles;
   }
   
//   public float[] getParticleWeights()
//   {
//      return particleWeights;
//   }
   
   public List getTracks()
   {
      return tracks;
   }
   
//   public float[] getTrackWeights()
//   {
//      return trackWeights;
//   }
   
   public void setClusters(Vector vector)
   {
      clusters = vector;
   }

   public void setEnergy(float f)
   {
      energy = f;
   }

   public void setMass(float f)
   {
      mass = f;
   }
   
   public float[] getReferencePoint()
   {
      return referencePoint;
   }
   
   public void setReferencePoint(float[] fs)
   {
      referencePoint = fs;
   }
   
   public float getCharge()
   {
      return charge;
   }
   
   public float[] getCovMatrix()
   {
      return covMatrix;
   }
   
   public void setCharge(float f)
   {
      charge = f;
   }
   
   public List getParticleIDs()
   {
      return particleIDs;
   }
   
   public void setCovMatrix(float[] cov)
   {
      covMatrix = cov;
   }
   
	public ParticleID getParticleIDUsed() {
		return particleIDUsed ;
	}

	public boolean isCompound() {
		return particles.size() > 0 ;
	}

}
