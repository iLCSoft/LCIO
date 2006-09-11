package hep.lcio.implementation.event;

import hep.lcio.event.ReconstructedParticle;
import hep.lcio.event.Vertex;

/**Implementation of Vertex
 * @author engels
 *
 */
public class IVertex extends ILCObject implements Vertex {

	private static float[] null0 = new float[0];
	protected int primary;
	protected String type = "Unknown";
	protected float chi2;
	protected float probability;
	protected float[] position = new float[3];
	protected float[] covMatrix = new float[6];
	protected float[] parameters = null0;
	protected ReconstructedParticle aRecP ;
	
	public ReconstructedParticle getAssociatedParticle() {
		return aRecP ;
	}
	
	public void setAssociatedParticle(ReconstructedParticle aRecP) {
		checkAccess();
		this.aRecP = aRecP;
	}

	public float getChi2() {
		return chi2;
	}
	
	public void setChi2(float chi2) {
		checkAccess();
		this.chi2 = chi2;
	}
	
	public float[] getCovMatrix() {
		return covMatrix;
	}
	
	public void setCovMatrix(float[] cov) {
		if (cov.length != 6) throw new IllegalArgumentException();
		checkAccess();
		this.covMatrix = cov;
	}

	public float[] getParameters() {
		return parameters;
	}
	
	public void setParameters(float[] parameters) {
		checkAccess();
		this.parameters = parameters;
	}

	public float[] getPosition() {
		return position;
	}
	
	public void setPosition(float[] position) {
      if (position.length != 3) throw new IllegalArgumentException();
      checkAccess();
      this.position = position;
   }

	public float getProbability() {
		return probability;
	}
	
	public void setProbability(float probability){
		checkAccess();
		this.probability = probability;
	}

	public boolean isPrimary() {
		return primary!=0 ? true : false;
	}
	
	public void setPrimary(boolean primary){
		checkAccess();
		this.primary = (primary == true ? 1 : 0 );
	}
	
	public String getAlgorithmType() {
		return type;
	}
	
	public void setAlgorithmType(String type) {
		checkAccess();
		this.type=type;
	}
}
