package hep.lcio.test;

import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.LCObject;
import hep.lcio.implementation.event.*;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.ILCFactory;
import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;
import java.beans.BeanInfo;
import java.beans.Introspector;
import java.beans.PropertyDescriptor;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.Array;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Random;

/**
 *
 * @author tonyj
 */
public class RandomEvent extends ILCEvent
{
   private static Random r = new Random();
   /** Creates a new instance of RandomEvent */
   public RandomEvent()
   {
      randomize(this);
      addCollection(LCIO.TRACK,ITrack.class,1<<LCIO.TRBIT_HITS);
      addCollection(LCIO.CLUSTER,ICluster.class,1<<LCIO.CLBIT_HITS);
      addCollection(LCIO.RECONSTRUCTEDPARTICLE,IReconstructedParticle.class);
      addCollection(LCIO.CALORIMETERHIT,ICalorimeterHit.class,1<<LCIO.RCHBIT_ID1 | 1<<LCIO.RCHBIT_LONG | 1<<LCIO.RCHBIT_TIME);
      addCollection(LCIO.LCFLOATVEC,ILCFloatVec.class);
      addCollection(LCIO.LCINTVEC,ILCIntVec.class);
      //addCollection(LCIO.LCRELATION,ILCRelation.class);
      //addColleciton(LCIO.LCGENERICOBJEC,ILCGenericObject.class);
      addCollection(LCIO.LCSTRVEC,ILCStrVec.class);
      addCollection(LCIO.MCPARTICLE,IMCParticle.class);
      addCollection(LCIO.RAWCALORIMETERHIT,IRawCalorimeterHit.class,1<<LCIO.RCHBIT_ID1 | 1<<LCIO.RCHBIT_TIME);
      addCollection(LCIO.SIMCALORIMETERHIT,ISimCalorimeterHit.class,1<<LCIO.CHBIT_ID1 | 1<<LCIO.CHBIT_LONG);
      addCollection(LCIO.SIMTRACKERHIT,ISimTrackerHit.class);
      addCollection(LCIO.TRACKERHIT,ITrackerHit.class);
      addCollection(LCIO.TPCHIT,ITPCHit.class);
      
      // Put in some links between objects, has to be done by hand for now
      LCCollection collection = this.getCollection(LCIO.TRACKERHIT);
      LCCollection target = this.getCollection(LCIO.TPCHIT);
      for (Iterator i = collection.iterator(); i.hasNext(); )
      {
         int size = r.nextInt(10);
         List l = new ArrayList(size);
         for (int ii=0; ii<size; ii++)
         {
            l.add(target.get(r.nextInt(target.size())));
         }
         ITrackerHit hit = (ITrackerHit) i.next();
         hit.setRawHits(l);
      }
      
      collection = this.getCollection(LCIO.CALORIMETERHIT);
      target = this.getCollection(LCIO.RAWCALORIMETERHIT);
      for (Iterator i = collection.iterator(); i.hasNext(); )
      {
         ICalorimeterHit hit = (ICalorimeterHit) i.next();
         hit.setRawHit((LCObject) target.get(r.nextInt(target.size())));
      }
      
      collection = this.getCollection(LCIO.TRACK);
      target = this.getCollection(LCIO.TRACKERHIT);
      for (Iterator i = collection.iterator(); i.hasNext(); )
      {
         int size = r.nextInt(10);
         List l = new ArrayList(size);
         for (int ii=0; ii<size; ii++)
         {
            l.add(target.get(r.nextInt(target.size())));
         }
         ITrack track = (ITrack) i.next();
         track.setTrackerHits(l);
      }
      
      collection = this.getCollection(LCIO.CLUSTER);
      target = this.getCollection(LCIO.CALORIMETERHIT);
      for (Iterator i = collection.iterator(); i.hasNext(); )
      {
         int size = r.nextInt(10);
         List l = new ArrayList(size);
         float[] weights = new float[size];
         for (int ii=0; ii<size; ii++)
         {
            l.add(target.get(r.nextInt(target.size())));
            weights[ii] = r.nextFloat();
         }
         ICluster cluster = (ICluster) i.next();
         cluster.setCalorimeterHits(l,weights);
      }
      collection = this.getCollection(LCIO.RECONSTRUCTEDPARTICLE);
      target = this.getCollection(LCIO.CLUSTER);
      for (Iterator i = collection.iterator(); i.hasNext(); )
      {
         int size = r.nextInt(10);
         List l = new ArrayList(size);
         for (int ii=0; ii<size; ii++)
         {
            l.add(target.get(r.nextInt(target.size())));
         }
         IReconstructedParticle particle = (IReconstructedParticle) i.next();
         particle.setClusters(l);
      }
      collection = this.getCollection(LCIO.RECONSTRUCTEDPARTICLE);
      target = this.getCollection(LCIO.TRACK);
      for (Iterator i = collection.iterator(); i.hasNext(); )
      {
         int size = r.nextInt(10);
         List l = new ArrayList(size);
         for (int ii=0; ii<size; ii++)
         {
            l.add(target.get(r.nextInt(target.size())));
         }
         IReconstructedParticle particle = (IReconstructedParticle) i.next();
         particle.setTracks(l);
      }
      collection = this.getCollection(LCIO.RECONSTRUCTEDPARTICLE);
      for (Iterator i = collection.iterator(); i.hasNext(); )
      {
         int size = r.nextInt(10);
         List l = new ArrayList(size);
         for (int ii=0; ii<size; ii++)
         {
            IParticleID pid = new IParticleID();
            randomize(pid);
            l.add(pid);
         }
         IReconstructedParticle particle = (IReconstructedParticle) i.next();
         particle.setParticleIDs(l);
      }
   }
   private void addCollection(String type, Class klass)
   {
      addCollection(type,klass,0);
   }
   private void addCollection(String type, Class klass, int flag)
   {
      int n = r.nextInt(100);
      LCCollection collection = new ILCCollection(type);
      collection.setFlag(flag);
      for (int i=0; i<n; i++)
      {
         try
         {
            Object obj = klass.newInstance();
            randomize(obj);
            collection.add(obj);
         }
         catch (Throwable t)
         {
            throw new RuntimeException("Could not create new instance of "+klass,t);
         }
      }
      this.addCollection(collection, type);
   }
   private static void randomize(Object obj)
   {
      try
      {
         BeanInfo info = Introspector.getBeanInfo(obj.getClass());
         PropertyDescriptor[] desc = info.getPropertyDescriptors();
         for (int i=0; i<desc.length; i++)
         {
            Method m = desc[i].getWriteMethod();
            Class type = desc[i].getPropertyType();
            Object[] args = new Object[1];
            if (m != null && type != null)
            {
               if (type == int.class)     args[0] = new Integer(r.nextInt());
               if (type == float.class)   args[0] = new Float(r.nextFloat());
               if (type == double.class)  args[0] = new Double(r.nextDouble());
               if (type == long.class)    args[0] = new Long(r.nextLong());
               if (type == boolean.class) args[0] = r.nextDouble() >= .5 ? Boolean.TRUE : Boolean.FALSE;
               if (type == String.class)  args[0] = gobbledygook();
            }
            if (args[0] != null) m.invoke(obj,args);
         }
      }
      catch (Throwable t)
      {
         throw new RuntimeException("Could not randomize "+obj.getClass(),t);
      }
   }
   private static void checkEqual(Object o1, Object o2)
   {
      checkEqual(o1,o2,new HashMap());
   }
   private static void checkEqual(Object o1,Object o2,Map alreadyChecked)
   {
      if (alreadyChecked.get(o1) == o2) return;
      alreadyChecked.put(o1,o2);
      try
      {
         if (o1 == null && o2 == null) return;
         if (o1 == null || o2 == null) throw new RuntimeException(o1+" != "+o2);
         if (o1 instanceof Comparable) 
         {
            int rc = ((Comparable) o1).compareTo(o2);
            if (rc == 0) return;
            else throw new RuntimeException(o1+" != "+o2);
         }
         if (o1.getClass().isArray())
         {
            if (Array.getLength(o1) != Array.getLength(o2)) throw new RuntimeException(o1+" != "+o2);
            for (int i=0; i<Array.getLength(o1);i++)
            {
               Object v1 = Array.get(o1, i);
               Object v2 = Array.get(o2, i);
               checkEqual(v1,v2,alreadyChecked);
            }
            return;
         }
         if (o1 instanceof Collection)
         {
            Collection c1 = (Collection) o1;
            Collection c2 = (Collection) o2;
            if (c1.size() != c2.size()) throw new RuntimeException(c1+" != "+c2);
            Iterator i1 = c1.iterator();
            Iterator i2 = c2.iterator();
            while ( i1.hasNext() )
            {
               Object v1 = i1.next();
               Object v2 = i2.next();
               checkEqual(v1,v2,alreadyChecked);
            }
            return;
         }

         BeanInfo info = Introspector.getBeanInfo(o1.getClass(),Object.class);
         PropertyDescriptor[] desc = info.getPropertyDescriptors();
         for (int i=0; i<desc.length; i++)
         {
            Method m = desc[i].getReadMethod();
            if (m != null)
            {
               Object v1 = m.invoke(o1,null);
               Object v2 = m.invoke(o2,null);
               checkEqual(v1,v2,alreadyChecked);
            }
         }
         
         if (o1 instanceof LCEvent)
         {
            String[] names = ((LCEvent) o1).getCollectionNames();
            for (int i=0; i<names.length; i++)
            {
               Collection c1 = ((LCEvent) o1).getCollection(names[i]);
               Collection c2 = ((LCEvent) o2).getCollection(names[i]);
               Iterator i1 = c1.iterator();
               Iterator i2 = c2.iterator();
               while ( i1.hasNext() )
               {
                  Object v1 = i1.next();
                  Object v2 = i2.next();
                  checkEqual(v1,v2,alreadyChecked);
               }
            }
         }
         
         return;
      }
      catch (Throwable t)
      {
         throw new RuntimeException("Could not compare "+o1+" "+o2,t);
      }
   }
   private static String gobbledygook()
   {
      StringBuffer buffer = new StringBuffer(10);
      for (int i=0; i<10; i++) buffer.append((char) ('a'+r.nextInt(26)));
      return buffer.toString();
   }
   public static void main(String[] args) throws IOException
   {
      RandomEvent e = new RandomEvent();
      checkEqual(e,e);
      
      ILCFactory factory = LCFactory.getInstance();
      LCWriter lcWrt = factory.createLCWriter();
      File file = new File("temp.slcio");
      file.deleteOnExit();
      lcWrt.open(file.getAbsolutePath());
      lcWrt.writeEvent(e);
      lcWrt.close();
      
      
//      File inFile = new File("temp.slcio"); 
      LCReader lcRead = factory.createLCReader();
      lcRead.open(file.getAbsolutePath());
      LCEvent e2 = lcRead.readNextEvent();
      lcRead.close();
      
      checkEqual(e,e2);
   }
}
