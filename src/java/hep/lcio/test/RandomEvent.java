package hep.lcio.test;

import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.LCObject;
import hep.lcio.event.TrackerData;
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
	private int maxobj = 100;

	public RandomEvent(int maxobj)
	{
		this.maxobj = maxobj;
		create();
	}
	
	public RandomEvent()
	{
		create();
	}

	private void create()
	{
		randomize(this);
		addCollection(LCIO.TRACK, ITrack.class, 1 << LCIO.TRBIT_HITS);
		addCollection(LCIO.CLUSTER, ICluster.class, 1 << LCIO.CLBIT_HITS);
		addCollection(LCIO.RECONSTRUCTEDPARTICLE, IReconstructedParticle.class);
		addCollection(LCIO.VERTEX, IVertex.class);
		addCollection(LCIO.CALORIMETERHIT, ICalorimeterHit.class, 1 << LCIO.RCHBIT_ID1 | 1 << LCIO.RCHBIT_LONG | 1 << LCIO.RCHBIT_TIME);
		addCollection(LCIO.LCFLOATVEC, ILCFloatVec.class);
		addCollection(LCIO.LCINTVEC, ILCIntVec.class);
		// FIXME: Put in ILCRelation.
		//addCollection(LCIO.LCRELATION,ILCRelation.class);
		addCollection(LCIO.LCGENERICOBJECT, ILCGenericObject.class);
		addCollection(LCIO.LCSTRVEC, ILCStrVec.class);
		addCollection(LCIO.MCPARTICLE, IMCParticle.class);
		addCollection(LCIO.RAWCALORIMETERHIT, IRawCalorimeterHit.class, 1 << LCIO.RCHBIT_ID1 | 1 << LCIO.RCHBIT_TIME);
		addCollection(LCIO.SIMCALORIMETERHIT, ISimCalorimeterHit.class, 1 << LCIO.CHBIT_ID1 | 1 << LCIO.CHBIT_LONG);
		addCollection(LCIO.SIMTRACKERHIT, ISimTrackerHit.class);
		addCollection(LCIO.TRACKERHIT, ITrackerHit.class);
		addCollection(LCIO.TPCHIT, ITPCHit.class);
		addCollection(LCIO.TRACKERRAWDATA, ITrackerRawData.class, 1 << LCIO.TRAWBIT_ID1);
		addCollection(LCIO.TRACKERDATA, ITrackerData.class, 1 << LCIO.TRAWBIT_ID1);
		addCollection(LCIO.TRACKERPULSE, ITrackerPulse.class, 1 << LCIO.TRAWBIT_ID1);

		// Put in some links between objects, has to be done by hand for now
		LCCollection collection = this.getCollection(LCIO.TRACKERHIT);
		LCCollection target = this.getCollection(LCIO.TPCHIT);
		for (Iterator i = collection.iterator(); i.hasNext();)
		{
			int size = r.nextInt(10);
			List l = new ArrayList(size);
			for (int ii = 0; ii < size; ii++)
			{
				l.add(target.get(r.nextInt(target.size())));
			}
			ITrackerHit hit = (ITrackerHit) i.next();
			hit.setRawHits(l);
		}

		collection = this.getCollection(LCIO.CALORIMETERHIT);
		target = this.getCollection(LCIO.RAWCALORIMETERHIT);
		for (Iterator i = collection.iterator(); i.hasNext();)
		{
			ICalorimeterHit hit = (ICalorimeterHit) i.next();
			hit.setRawHit((LCObject) target.get(r.nextInt(target.size())));
		}

		collection = this.getCollection(LCIO.TRACKERPULSE);
		target = this.getCollection(LCIO.TRACKERDATA);
		for (Iterator i = collection.iterator(); i.hasNext();)
		{
			ITrackerPulse hit = (ITrackerPulse) i.next();
			hit.setTrackerData((TrackerData) target.get(r.nextInt(target.size())));
		}

		collection = this.getCollection(LCIO.TRACK);
		target = this.getCollection(LCIO.TRACKERHIT);
		for (Iterator i = collection.iterator(); i.hasNext();)
		{
			int size = r.nextInt(10);
			List l = new ArrayList(size);
			for (int ii = 0; ii < size; ii++)
			{
				l.add(target.get(r.nextInt(target.size())));
			}
			ITrack track = (ITrack) i.next();
			track.setTrackerHits(l);
		}

		collection = this.getCollection(LCIO.CLUSTER);
		target = this.getCollection(LCIO.CALORIMETERHIT);
		for (Iterator i = collection.iterator(); i.hasNext();)
		{
			int size = r.nextInt(10);
			List l = new ArrayList(size);
			float[] weights = new float[size];
			for (int ii = 0; ii < size; ii++)
			{
				l.add(target.get(r.nextInt(target.size())));
				weights[ii] = r.nextFloat();
			}
			ICluster cluster = (ICluster) i.next();
			cluster.setCalorimeterHits(l, weights);
		}
		collection = this.getCollection(LCIO.RECONSTRUCTEDPARTICLE);
		target = this.getCollection(LCIO.CLUSTER);
		for (Iterator i = collection.iterator(); i.hasNext();)
		{
			int size = r.nextInt(10);
			List l = new ArrayList(size);
			for (int ii = 0; ii < size; ii++)
			{
				l.add(target.get(r.nextInt(target.size())));
			}
			IReconstructedParticle particle = (IReconstructedParticle) i.next();
			particle.setClusters(l);
		}
		collection = this.getCollection(LCIO.RECONSTRUCTEDPARTICLE);
		target = this.getCollection(LCIO.TRACK);
		for (Iterator i = collection.iterator(); i.hasNext();)
		{
			int size = r.nextInt(10);
			List l = new ArrayList(size);
			for (int ii = 0; ii < size; ii++)
			{
				l.add(target.get(r.nextInt(target.size())));
			}
			IReconstructedParticle particle = (IReconstructedParticle) i.next();
			particle.setTracks(l);
		}
		collection = this.getCollection(LCIO.RECONSTRUCTEDPARTICLE);
		for (Iterator i = collection.iterator(); i.hasNext();)
		{
			int size = r.nextInt(10);
			List l = new ArrayList(size);
			for (int ii = 0; ii < size; ii++)
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
		addCollection(type, klass, 0);
	}

	private void addCollection(String type, Class klass, int flag)
	{
		int n = r.nextInt(maxobj);
		if (n==0) n=1;
		LCCollection collection = new ILCCollection(type);
		collection.setFlag(flag);
		for (int i = 0; i < n; i++)
		{
			try
			{
				Object obj = klass.newInstance();
				randomize(obj);
				collection.add(obj);
			}
			catch (Throwable t)
			{
				throw new RuntimeException("Could not create new instance of " + klass, t);
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
			for (int i = 0; i < desc.length; i++)
			{
				Method m = desc[i].getWriteMethod();
				Class type = desc[i].getPropertyType();
				Object[] args = new Object[1];
				if (m != null && type != null)
				{
					if (type == int.class)
						args[0] = new Integer(r.nextInt());
					if (type == float.class)
						args[0] = new Float(r.nextFloat());
					if (type == double.class)
						args[0] = new Double(r.nextDouble());
					if (type == long.class)
						args[0] = new Long(r.nextLong());
					if (type == boolean.class)
						args[0] = r.nextDouble() >= .5 ? Boolean.TRUE : Boolean.FALSE;
					if (type == String.class)
						args[0] = gobbledygook();
				}
				if (args[0] != null)
					m.invoke(obj, args);
			}
		}
		catch (Throwable t)
		{
			throw new RuntimeException("Could not randomize " + obj.getClass(), t);
		}
	}

	private static String gobbledygook()
	{
		StringBuffer buffer = new StringBuffer(10);
		for (int i = 0; i < 10; i++)
			buffer.append((char) ('a' + r.nextInt(26)));
		return buffer.toString();
	}
}
