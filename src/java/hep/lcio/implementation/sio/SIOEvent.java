package hep.lcio.implementation.sio;

import hep.lcd.io.sio.*;
import hep.lcio.event.*;

import hep.lcio.implementation.event.ILCEvent;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOEvent.java,v 1.28 2004-09-22 16:12:39 tonyj Exp $
 */
class SIOEvent extends ILCEvent
{
   private Map blockMap;
   
   SIOEvent(SIORecord record, int accessMode) throws IOException
   {
      setAccess(accessMode);
      SIOBlock block = record.getBlock();
      int major = block.getMajorVersion() ;
      int minor = block.getMinorVersion() ;
      if (( major < 1) && ( minor < 8))
         throw new IOException("Sorry: files created with versions older than v00-08" + " are no longer supported !");
      
      SIOInputStream in = block.getData();
      runNumber = in.readInt();
      eventNumber = in.readInt();
      
      timeStamp = in.readLong();
      detectorName = in.readString();
      
      int nBlockNames = in.readInt();
      blockMap = new HashMap();
      for (int i = 0; i < nBlockNames; i++)
      {
         String blockName = in.readString();
         String blockType = in.readString();
         
         blockMap.put(blockName, blockType);
      }
      
      if( (major<<16 | minor ) > (1<<16|1)  )
      {
         parameters = new SIOLCParameters(in) ;
      }
      
   }
   void setReadOnly(boolean mode)
   {
      super.setAccess(mode ? LCIO.READ_ONLY : LCIO.UPDATE);
   }
   void checkSIOAccess()
   {
      super.checkAccess();
   }
   void readData(SIORecord record) throws IOException
   {
      for (;;)
      {
         SIOBlock block = record.getBlock();
         if (block == null) break;
         int major = block.getMajorVersion();
         int minor = block.getMinorVersion();
         
         if ((major < 1) && (minor < 8))
            throw new IOException("Sorry: files created with versions older than v00-08" + " are no longer supported !");
         
         SIOInputStream in = block.getData();
         String name = block.getBlockName();
         String type = (String) blockMap.get(name);
         if (type == null) continue;
                 
         int flags = in.readInt();
         LCParameters colParameters = null ;
         if( (major<<16 | minor ) > (1<<16|1)  )
         {
            colParameters = new SIOLCParameters(in) ;
         }
         
         if (type.equals(LCIO.MCPARTICLE))
         {
            int nMC = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, nMC);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < nMC; i++)
               ilc.add(new SIOMCParticle(in, this, major, minor));
            for (int i = 0; i < nMC; i++)
               ((SIOMCParticle) ilc.get(i)).resolve(major, minor);
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.SIMTRACKERHIT))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOSimTrackerHit(in, this,major,minor));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.TRACKERHIT))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOTrackerHit(in, this,major,minor));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.TPCHIT))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOTPCHit(in,flags, this,major,minor) );
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.SIMCALORIMETERHIT))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOSimCalorimeterHit(in, flags, this, major, minor));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.CALORIMETERHIT))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOCalorimeterHit(in, flags, this, major, minor));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.LCSTRVEC))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOStrVec(in, this));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.LCFLOATVEC))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOFloatVec(in, this));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.LCINTVEC))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOIntVec(in, this));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.CLUSTER))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOCluster(in, this, flags, major, minor));
            ilc.setOwner(this);
            addCollection(ilc,name);
         }
         else if (type.equals(LCIO.TRACK))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOTrack(in, this, flags, major, minor));
            ilc.setOwner(this);
            addCollection(ilc,name);
         }
         else if (type.equals(LCIO.RECONSTRUCTEDPARTICLE))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOReconstructedParticle(in, this, flags, major, minor));
            ilc.setOwner(this);
            addCollection(ilc,name);
         }
         else if (type.equals(LCIO.LCRELATION))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIORelation(in,this, flags, major, minor));
            ilc.setOwner(this);
         }
         else 
         {
           System.err.println("Warning: UNKNOWN collection type: " + type) ;
         }
         
      }
   }
   
   static void writeData(LCEvent event, SIOWriter writer, boolean headerOnly) throws IOException
   {
      if (headerOnly)
      {
         SIOOutputStream out = writer.createBlock(SIOFactory.eventHeaderBlockName, LCIO.MAJORVERSION, LCIO.MINORVERSION);
         out.writeInt(event.getRunNumber());
         out.writeInt(event.getEventNumber());
         
         out.writeLong(event.getTimeStamp());
         out.writeString(event.getDetectorName());
         
         String[] blockNames = event.getCollectionNames();
         
         int nBlocks = blockNames.length ;
         for (int i = 0; i < blockNames.length; i++)
         {
            if( event.getCollection(blockNames[i]).isTransient() )
               nBlocks-- ;
         }
         out.writeInt( nBlocks );
         
         for (int i = 0; i < blockNames.length; i++)
         {
            if( ! event.getCollection(blockNames[i]).isTransient() )
            {
               
               String blockName = blockNames[i];
               out.writeString(blockName);
               out.writeString(event.getCollection(blockName).getTypeName());
            }
         }
         //if( (LCIO.MAJORVERSION<<16 | LCIO.MINORVERSION ) > (1<<16|1)  ){
         SIOLCParameters.write( event.getParameters() , out ) ;
         //}
         
      }
      else
      {
         String[] blockNames = event.getCollectionNames();
         for (int j = 0; j < blockNames.length; j++)
         {
            String blockName = blockNames[j];
            SIOOutputStream out = writer.createBlock(blockName, LCIO.MAJORVERSION, LCIO.MINORVERSION);
            LCCollection col = event.getCollection(blockName);
            String type = col.getTypeName();
            int flags = col.getFlag();
            out.writeInt(flags);
            
            //if( (LCIO.MAJORVERSION<<16 | LCIO.MINORVERSION ) > (1<<16|1)  ){
            SIOLCParameters.write( col.getParameters() , out ) ;
            //}
              
            int n = col.getNumberOfElements();
            out.writeInt(n);
            if (type.equals(LCIO.MCPARTICLE))
            {
               for (int i = 0; i < n; i++)
                  SIOMCParticle.write((MCParticle) col.getElementAt(i), out);
            }
            else if (type.equals(LCIO.SIMTRACKERHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOSimTrackerHit.write((SimTrackerHit) col.getElementAt(i), out);
            }
            else if (type.equals(LCIO.TRACKERHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOTrackerHit.write((TrackerHit) col.getElementAt(i), out);
            }            
            else if (type.equals(LCIO.TPCHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOTPCHit.write((TPCHit) col.getElementAt(i), out, flags );
            }
            else if (type.equals(LCIO.SIMCALORIMETERHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOSimCalorimeterHit.write((SimCalorimeterHit) col.getElementAt(i), out, flags);
            }
            else if (type.equals(LCIO.CALORIMETERHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOCalorimeterHit.write((CalorimeterHit) col.getElementAt(i), out, flags);
            }
            else if (type.equals(LCIO.LCSTRVEC))
            {
               for (int i = 0; i < n; i++)
                  SIOStrVec.write((LCStrVec) col.getElementAt(i), out);
            }
            else if (type.equals(LCIO.LCFLOATVEC))
            {
               for (int i = 0; i < n; i++)
                  SIOFloatVec.write((LCFloatVec) col.getElementAt(i), out);
            }
            else if (type.equals(LCIO.LCINTVEC))
            {
               for (int i = 0; i < n; i++)
                  SIOIntVec.write((LCIntVec) col.getElementAt(i), out);
            }
            else if (type.equals(LCIO.CLUSTER))
            {
               for (int i=0; i < n; i++)
                  SIOCluster.write((Cluster) col.getElementAt(i), out, flags);
            }
            else if (type.equals(LCIO.TRACK))
            {
               for (int i=0; i < n; i++)
                  SIOTrack.write((Track) col.getElementAt(i), out, flags);
            }
            else if (type.equals(LCIO.RECONSTRUCTEDPARTICLE))
            {
               for (int i=0; i < n; i++)
                  SIOReconstructedParticle.write((ReconstructedParticle) col.getElementAt(i), out, flags);
            }
            else if (type.equals(LCIO.LCRELATION))
            {
               for (int i=0; i < n; i++)
                  SIORelation.write((LCRelation) col.getElementAt(i), out, flags);
            }
         }
      }
   }
   
   static void write(LCEvent event, SIOWriter writer) throws IOException
   {
      writer.createRecord(SIOFactory.eventHeaderRecordName, SIOFactory.compressionMode);
      SIOEvent.writeData(event, writer, true);
      writer.createRecord(SIOFactory.eventRecordName, SIOFactory.compressionMode);
      SIOEvent.writeData(event, writer, false);
   }
}
