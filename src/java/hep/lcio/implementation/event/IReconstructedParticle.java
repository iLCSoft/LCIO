/*
 * Created on Apr 6, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package hep.lcio.implementation.event;

import hep.lcio.event.Cluster;
import hep.lcio.event.MCParticle;
import hep.lcio.event.ParticleID;
import hep.lcio.event.ReconstructedParticle;
import hep.lcio.event.Track;

import java.util.Vector;

/**Implementation of ReconstructedParticle
 * @author gaede
 *
 */
public class IReconstructedParticle implements ReconstructedParticle {


	/* (non-Javadoc)
	 * @see hep.lcio.event.ReconstructedParticle#addParticleID(hep.lcio.event.ParticleID)
	 */
	public void addParticleID(ParticleID pid) {
		particleIDs.add( pid) ;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.ReconstructedParticle#addParticle(hep.lcio.event.ReconstructedParticle)
	 */
	public void addParticle(ReconstructedParticle particle) {
		addParticle( particle , (float) 1.0 ) ;

	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.ReconstructedParticle#addParticle(hep.lcio.event.ReconstructedParticle, float)
	 */
	public void addParticle(ReconstructedParticle particle, float weight) {
		particles.add( particle ) ;
		particleWeights.add( new Float(weight) ) ;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.ReconstructedParticle#addCluster(hep.lcio.event.Cluster)
	 */
	public void addCluster(Cluster cluster) {
	  addCluster( cluster , (float) 1. ) ;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.ReconstructedParticle#addCluster(hep.lcio.event.Cluster, float)
	 */
	public void addCluster(Cluster cluster, float weight) {
	 clusters.add( cluster  ) ;
	 clusterWeights.add( new Float(weight)) ;

	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.ReconstructedParticle#addTrack(hep.lcio.event.Track)
	 */
	public void addTrack(Track track) {
		addTrack( track, (float) 1. ) ;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.ReconstructedParticle#addTrack(hep.lcio.event.Track, float)
	 */
	public void addTrack(Track track, float weight) {
		tracks.add( track ) ;
		trackWeights.add( new Float( weight) ) ;

	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.ReconstructedParticle#addMCParticle(hep.lcio.event.MCParticle)
	 */
	public void addMCParticle(MCParticle mcParticle) {
		addMCParticle( mcParticle, (float) 1. ) ;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.ReconstructedParticle#addMCParticle(hep.lcio.event.MCParticle, float)
	 */
	public void addMCParticle(MCParticle mcParticle, float weight) {
		mCParticles.add(mcParticle) ;
		
	}
 
	protected int type ;
	protected boolean primary ;
	protected float[] momentum = new float[3] ;
	protected float[] referencePoint = new float[3] ;
	protected float mass ;
	protected float energy ;
	protected float charge ;
	protected Vector particleIDs ;
	protected Vector mCParticles ;
	protected Vector mCParticleWeights ;
	protected Vector particles ;
	protected Vector particleWeights ;
	protected Vector tracks ;
	protected Vector trackWeights ;
	protected Vector clusters;
	protected Vector clusterWeights ;
	protected Vector covMatrix ;
	

	/**
	 * @return
	 */
	public float[] getMomentum() {
		return momentum;
	}

	/**
	 * @return
	 */
	public boolean isPrimary() {
		return primary;
	}

	/**
	 * @return
	 */
	public int getType() {
		return type;
	}

	/**
	 * @param fs
	 */
	public void setMomentum(float[] fs) {
		momentum = fs;
	}

	/**
	 * @param b
	 */
	public void setPrimary(boolean b) {
		primary = b;
	}

	/**
	 * @param i
	 */
	public void setType(int i) {
		type = i;
	}

	/**
	 * @return
	 */
	public Vector getMCParticles() {
		return mCParticles;
	}


	/**
	 * @return
	 */
	public Vector getClusters() {
		return clusters;
	}

	/**
	 * @return
	 */
	public Vector getClusterWeights() {
		return clusterWeights;
	}

	/**
	 * @return
	 */
	public float getEnergy() {
		return energy;
	}

	/**
	 * @return
	 */
	public float getMass() {
		return mass;
	}

	/**
	 * @return
	 */
	public Vector getMCParticleWeights() {
		return mCParticleWeights;
	}

	/**
	 * @return
	 */
	public Vector getParticles() {
		return particles;
	}

	/**
	 * @return
	 */
	public Vector getParticleWeights() {
		return particleWeights;
	}


	/**
	 * @return
	 */
	public Vector getTracks() {
		return tracks;
	}

	/**
	 * @return
	 */
	public Vector getTrackWeights() {
		return trackWeights;
	}

	/**
	 * @param vector
	 */
	public void setClusters(Vector vector) {
		clusters = vector;
	}

	/**
	 * @param f
	 */
	public void setEnergy(float f) {
		energy = f;
	}

	/**
	 * @param f
	 */
	public void setMass(float f) {
		mass = f;
	}


	/**
	 * @return
	 */
	public float[] getReferencePoint() {
		return referencePoint;
	}

	/**
	 * @param fs
	 */
	public void setReferencePoint(float[] fs) {
		referencePoint = fs;
	}

	/**
	 * @return
	 */
	public float getCharge() {
		return charge;
	}

	/**
	 * @return
	 */
	public Vector getCovMatrix() {
		return covMatrix;
	}

	/**
	 * @param f
	 */
	public void setCharge(float f) {
		charge = f;
	}

	/**
	 * @return
	 */
	public Vector getParticleIDs() {
		return particleIDs;
	}

	/**
	 * @param vector
	 */
	public void setCovMatrix(Vector vector) {
		covMatrix = vector;
	}

	public void setCovMatrix(float[] cov) {
		for (int i = 0; i < cov.length; i++) {
			covMatrix.set(i , new Float (cov[i] )  ) ;
		}
	}

}
