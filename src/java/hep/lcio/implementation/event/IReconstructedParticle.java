package hep.lcio.implementation.event;

import hep.lcio.event.Cluster;
import hep.lcio.event.ParticleID;
import hep.lcio.event.ReconstructedParticle;
import hep.lcio.event.Track;
import hep.lcio.event.Vertex ;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.ListIterator;
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
   protected float[] covMatrix = new float[10];
   protected double[] momentum = new double[3];
   protected float[] referencePoint = new float[3];
   protected float charge;
   protected double energy;
   protected double mass;
   protected float goodnessOfPID;
   protected int type;
   protected Vertex startVertex;
   

   public float getGoodnessOfPID()
   {
      return goodnessOfPID;
   }
   
   public void setGoodnessOfPID(float goodnessOfPID)
   {
      checkAccess();
      this.goodnessOfPID = goodnessOfPID;
   }
   
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
      return clusters == null ? Collections.EMPTY_LIST : clusters;
   }
   
   public boolean isCompound()
   {
      return particles != null && !particles.isEmpty();
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
   
   public double getEnergy()
   {
      return energy;
   }
   
   public void setMass(float f)
   {
      mass = f;
   }
   
   public double getMass()
   {
      return mass;
   }
   
   public void setMomentum(float[] fs)
   {
      if (fs.length != 3) throw new IllegalArgumentException();
      checkAccess();
      setMomentum( momentum[0], momentum[1], momentum[2]) ;
   }
   public void setMomentum(double[] momentum)
   {
      checkAccess();
      if (momentum.length != 3)
         throw new IllegalArgumentException();
      this.momentum = momentum;
   }
   
   public void setMomentum(double px , double py, double pz){
      this.momentum[0] = px ;
      this.momentum[1] = py ;
      this.momentum[2] = pz ;
   }
   
   public double[] getMomentum()
   {
      return momentum;
   }
   
   public ParticleID getParticleIDUsed()
   {
      return particleIDUsed;
   }
   public void setParticleIDs(List ids)
   {
      checkAccess();
//      this.particleIDs = ids;
    //fg: add elmenets one by one to sort wrt. likelihood...
      ListIterator iter = ids.listIterator();
      while (iter.hasNext()) {
         ParticleID element = (ParticleID) iter.next();
         addParticleID(element) ;
      }
   
   }
   
   public List getParticleIDs()
   {
      return particleIDs == null ? Collections.EMPTY_LIST : particleIDs;
   }
   public void setParticles(List particles)
   {
      checkAccess();
      this.particles = particles;
   }   
   public List getParticles()
   {
      return particles == null ? Collections.EMPTY_LIST : particles;
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
   
   public Vertex getStartVertex(){
	   return startVertex;
   }
   
   public void setStartVertex(Vertex vertex){
	   checkAccess();
	   this.startVertex = vertex;
   }
   
   public Vertex getEndVertex(){
	   if(getParticles().size() != 0)
		   return ((ReconstructedParticle)getParticles().get(0)).getStartVertex();
	   return null;
   }

   public void setTracks(List tracks)
   {
      checkAccess();
      this.tracks = tracks;
   }
   public List getTracks()
   {
      return tracks == null ? Collections.EMPTY_LIST : tracks;
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
      if (clusters == null) clusters = new ArrayList();
      clusters.add(cluster);
   }
   public void addTrack(Track track)
   {
      if (tracks == null) tracks = new ArrayList();
      tracks.add(track);
   }  
   public void addParticle(ReconstructedParticle particle)
   {
      if (particles == null) particles = new ArrayList();
      particles.add(particle);
   }
   
   public void addParticleID(ParticleID pid)
   {
      if (particleIDs ==  null) particleIDs = new ArrayList();

      //fg: insert pid sorted wrt. to likelihood
      ListIterator it = particleIDs.listIterator() ;
      while( it.hasNext() ) {
         ParticleID aPid = (ParticleID) it.next() ;                
         if( aPid.getLikelihood() < pid.getLikelihood() ){
            particleIDs.add( it.nextIndex()-1  , pid ) ;
            return ;
         }
      }
      particleIDs.add(pid);
   }
   
}
