package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcio.event.ParticleID;
import hep.lcio.implementation.event.ICluster;

import java.util.Iterator;
import java.util.List;
import java.io.IOException;
import java.util.Vector;

import sun.security.x509.IPAddressName;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOCluster.java,v 1.9 2004-09-01 16:42:57 gaede Exp $
 */
class SIOCluster extends ICluster
{
   SIOCluster(SIOInputStream in, SIOEvent owner, int flag, int major, int minor) throws IOException
   {
      type = in.readInt();
      energy = in.readFloat();
      position = new float[3];
      position[0] = in.readFloat();
      position[1] = in.readFloat();
      position[2] = in.readFloat();
      positionError = new float[6];
      positionError[0] = in.readFloat();
      positionError[1] = in.readFloat();
      positionError[2] = in.readFloat();
      positionError[3] = in.readFloat();
      positionError[4] = in.readFloat();
      positionError[5] = in.readFloat();
      theta = in.readFloat();
      phi = in.readFloat();
      directionError = new float[3] ;
      directionError[0] = in.readFloat();
      directionError[1] = in.readFloat();
      directionError[2] = in.readFloat();

	  int nShape ;
	  if( 1000.*major+minor > 1002.){
        nShape = in.readInt() ;
	  }else{
	    nShape = 6 ;
	  }
      shape = new float[nShape] ;
      for (int i = 0; i < nShape; i++) {
		shape[i] = in.readFloat();
	  }

	  if( 1000.*major+minor > 1002.){
         // TODO/Fixme: need IParticleID here
         int nPID = in.readInt() ;
         for (int i = 0; i < nPID; i++) {
			float dummy  = in.readFloat() ;
			dummy  = in.readInt() ;
			dummy  = in.readInt() ;
			dummy  = in.readFloat() ;
		 }

	  }else{
		// read 3 dummy floats
		float[] particleType = new float[3] ;
		particleType[0] = in.readFloat();
		particleType[1] = in.readFloat();
		particleType[2] = in.readFloat();
	  }


      int x = in.readInt(); // Fixme:
      for(int i =0 ; i<x ; i++ ){
		in.readPntr(); // Fixme:
      }
      if ((flag & (1<<31)) != 0)
      {
         int n = in.readInt();
         
         for (int i=0; i<n; i++)
         {
			in.readPntr(); // Fixme:
            in.readFloat(); // Fixme:
         }
      }
      int nEnergies = in.readInt() ;
      subdetectorEnergies = new float[ nEnergies] ;
      for (int i = 0; i < nEnergies; i++) {
		subdetectorEnergies[i] = in.readFloat() ;
	  }
      in.readPTag(this);
   }

   static void write(ICluster cluster, SIOOutputStream out, int flag) throws IOException
   {
      if (cluster instanceof SIOCluster)
         ((SIOCluster) cluster).write(out,flag);
      else
      {
         out.writeInt(cluster.getType());
         out.writeFloat(cluster.getEnergy());
         float[] p = cluster.getPosition();
         out.writeFloat(p[0]);
         out.writeFloat(p[1]);
         out.writeFloat(p[2]);
         p = cluster.getPositionError();
         out.writeFloat(p[0]);
         out.writeFloat(p[1]);
         out.writeFloat(p[2]);
         out.writeFloat(p[3]);
         out.writeFloat(p[4]);
         out.writeFloat(p[5]);
         out.writeFloat(cluster.getIPhi());
         out.writeFloat(cluster.getITheta());
         p = cluster.getDirectionError();
         out.writeFloat(p[0]);
         out.writeFloat(p[1]);
         out.writeFloat(p[2]);

         float shapes[] = cluster.getShape();
		 //out.writeInt( shapes.length );
		 out.writeFloatArray(shapes) ;	

         List pids = cluster.getParticleIDs() ;
         out.writeInt( pids.size()) ;
         for (Iterator iter = pids.iterator(); iter.hasNext();) {
			ParticleID pid = (ParticleID) iter.next();
			out.writeFloat( pid.getLoglikelihood() ) ;
			out.writeInt( pid.getType() ) ;
			out.writeInt( pid.getPDG() ) ;
			out.writeFloat( pid.getGoodnessOfPID() ) ;
		}
         
         
         out.writeInt(0); // Fixme:
         if ((flag & (1<<31)) != 0)
         {
            int n = 0; // Fixme:
            out.writeInt(n);
            for (int i=0; i<n; i++)
            {
               out.writePntr(null);
               out.writeFloat(0);
            }
         }
         //out.writeInt( cluster.getSubdetectorEnergies().length ) ;
        // for (int i = 0; i < cluster.getSubdetectorEnergies().length; i++){
			out.writeFloatArray( cluster.getSubdetectorEnergies() ) ;
 		// }
         
         out.writePTag(cluster);
      }
   }
   private void write(SIOOutputStream out, int flag) throws IOException
   {
      out.writeInt(getType());
      out.writeFloat(energy);
      out.writeFloat(position[0]);
      out.writeFloat(position[1]);
      out.writeFloat(position[2]);
      out.writeFloat(positionError[0]);
      out.writeFloat(positionError[1]);
      out.writeFloat(positionError[2]);
      out.writeFloat(positionError[3]);
      out.writeFloat(positionError[4]);
      out.writeFloat(positionError[5]);
      out.writeFloat(theta);
      out.writeFloat(phi);
      out.writeFloat(directionError[0]);
      out.writeFloat(directionError[1]);
      out.writeFloat(directionError[2]);

  	  //out.writeInt( shape.length );
	  out.writeFloatArray(shape) ;	
	  out.writeInt( particleIDs.size()) ;
	  for (Iterator iter = particleIDs.iterator(); iter.hasNext();) {
	    ParticleID pid = (ParticleID) iter.next();
	    out.writeFloat( pid.getLoglikelihood() ) ;
	    out.writeInt( pid.getType() ) ;
	    out.writeInt( pid.getPDG() ) ;
	    out.writeFloat( pid.getGoodnessOfPID() ) ;
      }

      out.writeInt(0); // Fixme:
      if ((flag & (1<<31)) != 0)
      {
         int n = 0; // Fixme:
         out.writeInt(n);
         for (int i=0; i<n; i++)
         {
            out.writePntr(null);
            out.writeFloat(0);
         }
      }
	  //out.writeInt( subdetectorEnergies.length ) ;
	  //for (int i = 0; i < subdetectorEnergies.length; i++){
	     out.writeFloatArray( subdetectorEnergies ) ;
	  //}
      out.writePTag(this);
   }
}
