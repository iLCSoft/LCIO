package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcio.event.ParticleID;
import hep.lcio.implementation.event.ICluster;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOCluster.java,v 1.10 2004-09-14 05:38:38 tonyj Exp $
 */
class SIOCluster extends ICluster
{
   SIOCluster(SIOInputStream in, SIOEvent owner, int flag, int major, int minor) throws IOException
   {
      type = in.readInt();
      energy = in.readFloat();
      position = new float[3];
      for (int i=0; i<3; i++) position[i] = in.readFloat();
      positionError = new float[6];
      for (int i=0; i<3; i++) positionError[i] = in.readFloat();
      theta = in.readFloat();
      phi = in.readFloat();
      directionError = new float[3] ;
      for (int i=0; i<3; i++) directionError[i] = in.readFloat();
      
      int nShape ;
      if( 1000*major+minor > 1002)
      {
         nShape = in.readInt() ;
      }
      else
      {
         nShape = 6 ;
      }
      shape = new float[nShape] ;
      for (int i = 0; i < nShape; i++)
      {
         shape[i] = in.readFloat();
      }
      
      if( 1000*major+minor > 1002)
      {
         int nPid = in.readInt() ;
         this.particleIDs = new ArrayList(nPid);
         for (int i=0; i<nPid; i++)
         {
            ParticleID id = new SIOParticleID(in,owner,flag,major,minor);
            particleIDs.add(id);
         }         
      }
      else
      {
         // read 3 dummy floats
         float[] particleType = new float[3] ;
         particleType[0] = in.readFloat();
         particleType[1] = in.readFloat();
         particleType[2] = in.readFloat();
      }
         
      int nClust = in.readInt();
      this.clusters = new ArrayList(nClust);
      for (int i =0 ; i<nClust ; i++ )
      {
         clusters.add(in.readPntr());
      }
      if ((flag & (1<<31)) != 0)
      {
         int n = in.readInt();
         this.calorimeterHits = new ArrayList(n);
         this.hitContributions = new float[n];
         for (int i=0; i<n; i++)
         {
            calorimeterHits.add(in.readPntr());
            hitContributions[i] = in.readFloat();
         }
      }
      int nEnergies = in.readInt();
      subdetectorEnergies = new float[nEnergies];
      for (int i = 0; i < nEnergies; i++)
      {
         subdetectorEnergies[i] = in.readFloat();
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
         for (int i=0; i<3; i++) out.writeFloat(p[i]);
         p = cluster.getPositionError();
         for (int i=0; i<6; i++) out.writeFloat(p[i]);
         out.writeFloat(cluster.getIPhi());
         out.writeFloat(cluster.getITheta());
         p = cluster.getDirectionError();
         for (int i=0; i<3; i++) out.writeFloat(p[i]);
         
         out.writeFloatArray(cluster.getShape()) ;
         
         List pids = cluster.getParticleIDs() ;
         out.writeInt( pids.size());
         for (Iterator iter = pids.iterator(); iter.hasNext();)
         {
            ParticleID pid = (ParticleID) iter.next();
            SIOParticleID.write(pid,out);
         }
            
         List clusters = cluster.getClusters();
         out.writeInt(clusters.size());
         for (Iterator i = clusters.iterator(); i.hasNext(); )
         {
            out.writePntr(i.next());
         }

         if ((flag & (1<<31)) != 0)
         {
            List calorimeterHits = cluster.getCalorimeterHits();
            float[] hitContributions = cluster.getHitContributions();
            out.writeInt(calorimeterHits.size());
            int ii = 0;
            for (Iterator i = calorimeterHits.iterator(); i.hasNext(); )
            {
               out.writePntr(i.next());
               out.writeFloat(hitContributions[ii++]);
            }
         }
         out.writeFloatArray(cluster.getSubdetectorEnergies());
         out.writePTag(cluster);
      }
   }
   private void write(SIOOutputStream out, int flag) throws IOException
   {
      out.writeInt(type);
      out.writeFloat(energy);
      for (int i=0; i<3; i++) out.writeFloat(position[i]);
      for (int i=0; i<3; i++) out.writeFloat(positionError[i]);
      out.writeFloat(theta);
      out.writeFloat(phi);
      for (int i=0; i<3; i++) out.writeFloat(directionError[i]);
    
      out.writeFloatArray(shape) ;
      out.writeInt(particleIDs.size()) ;
      for (Iterator iter = particleIDs.iterator(); iter.hasNext();)
      {
         ParticleID pid = (ParticleID) iter.next();
         SIOParticleID.write(pid,out);
      }
      
      out.writeInt(clusters.size());
      for (Iterator i = clusters.iterator(); i.hasNext(); )
      {
         out.writePntr(i.next());
      }
      
      if ((flag & (1<<31)) != 0)
      {
         out.writeInt(calorimeterHits.size());
         int ii = 0;
         for (Iterator i = calorimeterHits.iterator(); i.hasNext(); )
         {
            out.writePntr(i.next());
            out.writeFloat(hitContributions[ii++]);
         }
      }
      out.writeFloatArray( subdetectorEnergies ) ;
      out.writePTag(this);
   }
}
