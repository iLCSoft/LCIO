package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcio.event.Vertex;
import hep.lcio.event.ReconstructedParticle;
import hep.lcio.implementation.event.IVertex;

import java.io.IOException;

/**
* @author engels
*/

public class SIOVertex extends IVertex{
	public SIOVertex(SIOInputStream in, SIOEvent owner, int major, int minor) throws IOException{
		this.primary = in.readInt();
		this.type = in.readInt();
		this.chi2 = in.readFloat();
		this.probability = in.readFloat();
		for (int i=0; i<3; i++) this.position[i] = in.readFloat();
		for (int i=0; i<6; i++) this.covMatrix[i] = in.readFloat();
		int n = in.readInt();
	    this.parameters = new float[n];
	    for (int i=0; i<n; i++) this.parameters[i] = in.readInt();
	    this.aRecP = (ReconstructedParticle) in.readPntr().getObject();
		in.readPTag(this);
	}
	static void write(Vertex v, SIOOutputStream out) throws IOException{
		if (v instanceof SIOVertex){
			((SIOVertex) v).write(out);
		}
	    else{
	    	out.writeInt(v.isPrimary()?1:0);
	    	out.writeInt(v.getAlgorithmType());
	    	out.writeFloat(v.getChi2());
	    	out.writeFloat(v.getProbability());
	    	float[] pos = v.getPosition();
	    	for (int i=0; i<3; i++) out.writeFloat(pos[i]);
	    	float[] matrix = v.getCovMatrix();
	    	for (int i=0; i<6; i++) out.writeFloat(matrix[i]);
	    	float[] pars = v.getParameters();
	    	int n = (pars == null ? 0 : pars.length);
	    	out.writeInt(n);
	    	for (int i=0; i<n; i++) out.writeFloat(pars[i]);
	    	out.writePntr(v.getAssociatedParticle());
	    	out.writePTag(v);
	      }
	   }
	private void write(SIOOutputStream out) throws IOException{
		out.writeInt(primary);
		out.writeInt(type);
		out.writeFloat(chi2);
		out.writeFloat(probability);
		for (int i=0; i<3; i++) out.writeFloat(position[i]);
		for (int i=0; i<6; i++) out.writeFloat(covMatrix[i]);
		int n = parameters == null ? 0 : parameters.length;
		out.writeInt(n);
		for (int i=0; i<n; i++) out.writeFloat(parameters[i]);
		out.writePntr(aRecP);
		out.writePTag(this);
	}
}
