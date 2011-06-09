package hep.lcio.implementation.sio;

import hep.io.sio.*;
import hep.lcio.event.*;

import hep.lcio.implementation.event.ILCEvent;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.TreeSet;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOEvent.java,v 1.46 2010-08-17 22:26:56 jeremy Exp $
 */
class SIOEvent extends ILCEvent
{
   private Map blockMap;
   
   static final String SUBSETPOSTFIX = "_References" ;
   
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
      
      List subsets = new ArrayList( ) ;
      
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
         //fg20050304 remove postfix _References for subset collections
         if( type.endsWith( SUBSETPOSTFIX ) )
         {
            type = type.substring( 0 , type.indexOf( SUBSETPOSTFIX ) ) ;
         }
         
         
         int flags = in.readInt();
         LCParameters colParameters = null ;
         if( (major<<16 | minor ) > (1<<16|1)  )
         {
            colParameters = new SIOLCParameters(in) ;
         }
//fg20050302 add suport for subset collections
         if( (flags & ( 1<< LCCollection.BITSubset )) != 0 )
         {
            int nObj = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, nObj );
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < nObj; i++)
            {
               ilc.addPointer( in.readPntr() );
            }
            ilc.setOwner(this);
            addCollection(ilc, name);
            subsets.add( ilc ) ;
         }
         
         else if (type.equals(LCIO.MCPARTICLE))
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
         else if (type.equals(LCIO.LCGENERICOBJECT))
         {
            boolean isFixedSize = (flags & (1 << LCIO.GOBIT_FIXED)) != 0 ;
            int nInt=0, nFloat=0, nDouble=0 ;
            if( isFixedSize)
            {
               nInt = in.readInt();
               nFloat = in.readInt();
               nDouble = in.readInt();
            }
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            if( isFixedSize)
            {
               for (int i = 0; i < n; i++)
                  ilc.add(new SIOLCGenericObject(in, this, major,minor,nInt,nFloat,nDouble)) ;
            }
            else
            {
               for (int i = 0; i < n; i++)
                  ilc.add(new SIOLCGenericObject(in, this,major,minor)) ;
            }
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.SIMTRACKERHIT))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOSimTrackerHit(in, flags, this,major,minor));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.TRACKERHIT))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOTrackerHit(in, flags, this,major,minor));
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
         else if (type.equals(LCIO.TRACKERRAWDATA))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOTrackerRawData(in,flags, this,major,minor) );
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.TRACKERDATA))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOTrackerData(in,flags, this,major,minor) );
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.TRACKERPULSE))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOTrackerPulse(in,flags, this,major,minor) );
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
         else if (type.equals(LCIO.RAWCALORIMETERHIT))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIORawCalorimeterHit(in, flags, this, major, minor));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.LCSTRVEC))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOStrVec(in, this, major, minor));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.LCFLOATVEC))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOFloatVec(in, this, major, minor));
            ilc.setOwner(this);
            addCollection(ilc, name);
         }
         else if (type.equals(LCIO.LCINTVEC))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            for (int i = 0; i < n; i++)
               ilc.add(new SIOIntVec(in, this, major, minor));
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
         else if (type.equals(LCIO.VERTEX))
         {
            int n = in.readInt();
            SIOLCCollection ilc = new SIOLCCollection(type, flags, n);
            ilc.setParameters( colParameters ) ;
            
            //index integers to strings (reading)
            String[] keys=colParameters.getStringVals("_lcio.VertexAlgorithmTypes");
            
            for (int i = 0; i < n; i++)
               ilc.add(new SIOVertex(in, this, keys, major, minor));
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
            addCollection(ilc,name);
         }
         else
         {
            System.err.println("Warning: UNKNOWN collection type: " + type) ;
         }
         
      }
      
      // restore the objects in subsets
      for (Iterator iter = subsets.iterator(); iter.hasNext();)
      {
         ( (SIOLCCollection) iter.next() ).resolve() ;
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
               
               // fg20050304 append _References to subset collection's type names
               LCCollection col  = event.getCollection(blockName) ;
               boolean isSubset = ( (col.getFlag() & ( 1<< LCCollection.BITSubset )) != 0 ) ;
               String typeName = col.getTypeName() ;
               if( isSubset )
               {
                  typeName += SUBSETPOSTFIX ;
               }
               
               out.writeString(  typeName );
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
            
            boolean isSubset = ( (flags & ( 1<< LCCollection.BITSubset )) != 0 ) ;
            
            int n = col.getNumberOfElements();
            
            boolean isFixedSize = true ;
            if ( type.equals(LCIO.LCGENERICOBJECT ))
            {
               // check if all objects are fixed size
               for (int i = 0; i < n; i++)
               {
                  if( ! ((LCGenericObject) col.getElementAt(i) ).isFixedSize() )
                  {
                     isFixedSize = false ;
                  }
               }
               // set the proper flag bit for isFixedSize
               if( isFixedSize)
                  flags |= (1 << LCIO.GOBIT_FIXED) ;
               else
                  flags &= ~(1 << LCIO.GOBIT_FIXED) ;
               
               // if the collection doesn't have the TypeName/DataDescription parameters set,
               //  we use the ones from the first object
               
               if( col.getNumberOfElements() > 0 )
               {
                  LCGenericObject gObj = (LCGenericObject) col.getElementAt(0) ;
                  
                  if(  col.getParameters().getStringVal("TypeName").length() == 0 )
                     col.getParameters().setValue( "TypeName", gObj.getTypeName() ) ;
                  if(isFixedSize )
                  {
                     if(  col.getParameters().getStringVal( "DataDescription" ).length() ==  0 )
                        col.getParameters().setValue( "DataDescription", gObj.getDataDescription() ) ;
                  }
               }
            }
            out.writeInt(flags);
            
            //vertices need to add indexing parameters to collection
            if ( type.equals(LCIO.VERTEX )){
                TreeSet _set = new TreeSet();
                
                for (int i=0; i < col.getNumberOfElements(); i++){
                	Vertex v = (Vertex) col.getElementAt(i);
                	_set.add(v.getAlgorithmType());
                }
                Object[] ob = _set.toArray() ;
        		String[] keys = new String[ ob.length ] ;
        	    for (int i = 0; i < keys.length; i++){
        	    	keys[i] = (String) ob[i] ;
        	    }
        	    //add parameters to collection
        	    col.getParameters().setValues( "_lcio.VertexAlgorithmTypes", keys );
            }
             
            SIOLCParameters.write( col.getParameters() , out ) ;
            
//          fg20050302 add suport for subset collections
            
            if (!isSubset && type.equals(LCIO.LCGENERICOBJECT))
            {
               if( isFixedSize)
               { // write the array length once for the collection
                  
                  if( col.getNumberOfElements() > 0 )
                  {
                     LCGenericObject obj =  (LCGenericObject) col.getElementAt(0) ;
                     
                     out.writeInt( obj.getNInt() ) ;
                     out.writeInt( obj.getNFloat() ) ;
                     out.writeInt( obj.getNDouble() ) ;
                  }
                  else
                  {
                     out.writeInt(0) ;
                     out.writeInt(0) ;
                     out.writeInt(0) ;
                  }
                  
               }
               out.writeInt(n);
               for (int i = 0; i < n; i++)
               {
                  SIOLCGenericObject.write((LCGenericObject) col.getElementAt(i), out, flags);
               }
            }
            else
               out.writeInt(n);
            
            
            if( isSubset ) // only write pointers
            {
               for (int i = 0; i < n; i++)
                  out.writePntr( col.getElementAt(i) ) ;
               
            }
            
            else if (type.equals(LCIO.MCPARTICLE))
            {
               for (int i = 0; i < n; i++)
                  SIOMCParticle.write((MCParticle) col.getElementAt(i), out);
            }
            else if (type.equals(LCIO.SIMTRACKERHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOSimTrackerHit.write((SimTrackerHit) col.getElementAt(i), out, flags);
            }
            else if (type.equals(LCIO.TRACKERHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOTrackerHit.write((TrackerHit) col.getElementAt(i), out, flags);
            }
            else if (type.equals(LCIO.TPCHIT))
            {
               for (int i = 0; i < n; i++)
                  SIOTPCHit.write((TPCHit) col.getElementAt(i), out, flags );
            }
            else if (type.equals(LCIO.TRACKERRAWDATA))
            {
               for (int i = 0; i < n; i++)
                  SIOTrackerRawData.write((TrackerRawData) col.getElementAt(i), out, flags );
            }
            else if (type.equals(LCIO.TRACKERDATA))
            {
               for (int i = 0; i < n; i++)
                  SIOTrackerData.write((TrackerData) col.getElementAt(i), out, flags );
            }
            else if (type.equals(LCIO.TRACKERPULSE))
            {
               for (int i = 0; i < n; i++)
                  SIOTrackerPulse.write((TrackerPulse) col.getElementAt(i), out, flags );
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
            else if (type.equals(LCIO.RAWCALORIMETERHIT))
            {
               for (int i = 0; i < n; i++)
                  SIORawCalorimeterHit.write((RawCalorimeterHit) col.getElementAt(i), out, flags);
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
            else if (type.equals(LCIO.VERTEX))
            {
            	//index strings to integers (writing)
 
            	TreeMap _map = new TreeMap();
            	String[] keys = new String[ col.getParameters().getStringVals("_lcio.VertexAlgorithmTypes").length ] ;
            	keys=col.getParameters().getStringVals("_lcio.VertexAlgorithmTypes");
            	for (int i=0; i < keys.length; i++){
            		_map.put(keys[i],new Integer(i));
            	}
                
               for (int i=0; i < n; i++)
                  SIOVertex.write((Vertex) col.getElementAt(i), out, _map);
            }
            else if (type.equals(LCIO.LCRELATION))
            {
               for (int i=0; i < n; i++)
                  SIORelation.write((LCRelation) col.getElementAt(i), out, flags);
            }
         }
      }
   }
   
   static long write(LCEvent event, SIOWriter writer) throws IOException
   {
      long recordLocation = writer.createRecord(SIOFactory.eventHeaderRecordName, SIOFactory.compressionMode);
      SIOEvent.writeData(event, writer, true);
      writer.createRecord(SIOFactory.eventRecordName, SIOFactory.compressionMode);
      SIOEvent.writeData(event, writer, false);
      return recordLocation;
   }
}
