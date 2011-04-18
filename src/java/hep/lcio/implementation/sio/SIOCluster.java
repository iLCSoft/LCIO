package hep.lcio.implementation.sio;

import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;
import hep.io.sio.SIORef;
import hep.lcio.event.Cluster;
import hep.lcio.event.LCIO;
import hep.lcio.event.ParticleID;
import hep.lcio.implementation.event.ICluster;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOCluster.java,v 1.17 2007-11-07 20:46:22 jeremy Exp $
 */
class SIOCluster extends ICluster
{
   private List tempHits;
   private List tempClusters;
   
   SIOCluster(SIOInputStream in, SIOEvent owner, int flag, int major, int minor) throws IOException
   {
      type = in.readInt();
      energy = in.readFloat();
      //if( 1000*major+minor > 1051)
      if( SIOVersion.encode(major,minor) > SIOVersion.encode(1,51)){
        energyError = in.readFloat();
      }
      position = new float[3];
      for (int i=0; i<3; i++) position[i] = in.readFloat();
      positionError = new float[6];
      for (int i=0; i<6; i++) positionError[i] = in.readFloat();
      theta = in.readFloat();
      phi = in.readFloat();
      directionError = new float[3];
      for (int i=0; i<3; i++) directionError[i] = in.readFloat();
      
      int nShape;
      if( 1000*major+minor > 1002)
      {
         nShape = in.readInt();
      }
      else
      {
         nShape = 6;
      }
      shape = new float[nShape];
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
      tempClusters = new ArrayList(nClust);
      clusters = null;
      for (int i =0 ; i<nClust ; i++ )
      {
         tempClusters.add(in.readPntr());
      }
      if ((flag & (1<<31)) != 0)
      {
         int n = in.readInt();
         tempHits = new ArrayList(n);
         calorimeterHits = null;
         this.hitContributions = new float[n];
         for (int i=0; i<n; i++)
         {
            tempHits.add(in.readPntr());
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
   
   static void write(Cluster cluster, SIOOutputStream out, int flag) throws IOException
   {
      if (cluster instanceof SIOCluster)
         ((SIOCluster) cluster).write(out,flag);
      else
      {
         out.writeInt(cluster.getType());
         out.writeFloat(cluster.getEnergy());
         out.writeFloat(cluster.getEnergyError());
         float[] p = cluster.getPosition();
         for (int i=0; i<3; i++) out.writeFloat(p[i]);
         p = cluster.getPositionError();
         for (int i=0; i<6; i++) out.writeFloat(p[i]);
         out.writeFloat(cluster.getITheta());
         out.writeFloat(cluster.getIPhi());
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
      out.writeFloat(energyError);
      for (int i=0; i<3; i++) out.writeFloat(position[i]);
      for (int i=0; i<6; i++) out.writeFloat(positionError[i]);
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
      
      List clusters = getClusters();
      out.writeInt(clusters.size());
      for (Iterator i = clusters.iterator(); i.hasNext(); )
      {
         out.writePntr(i.next());
      }
      
      if (bitTest(flag,LCIO.CLBIT_HITS))
      {
         List calorimeterHits = getCalorimeterHits();
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
   
   public List getCalorimeterHits()
   {
      if (calorimeterHits == null && tempHits != null)
      {
         calorimeterHits = new ArrayList(tempHits.size());
         for (Iterator i = tempHits.iterator(); i.hasNext();)
         {
            calorimeterHits.add(((SIORef) i.next()).getObject());
         }
         tempHits = null;         
      }
      return super.getCalorimeterHits();
   }
   
}
