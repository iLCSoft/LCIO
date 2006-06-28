package hep.lcio.util;

import hep.lcio.event.LCEvent;
import java.beans.BeanInfo;
import java.beans.Introspector;
import java.beans.PropertyDescriptor;
import java.lang.reflect.Array;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

/**
 *
 * Object comparison code refactored out of tonyj's old hep.lcio.test.RandomEvent .
 *
 * @author jeremym
 */
public class ObjectComparator
{

	// comparison result
	public static final int NOT_EQUAL = 1;
	public static final int EQUAL = 0;
	public static final int NO_COMPARISON = -1;

	// verbosity
	public static final int SILENT = 0;
	public static final int INFO = 1;
	public static final int DEBUG = 2;
	public static final int ALL = 3;

	// current result
	int m_result;

	// current method
	Method m_method;

	// dup map
	Map m_alreadyChecked;

	int verbosity = 1;

	public ObjectComparator()
	{
		m_result = EQUAL;
		m_alreadyChecked = new HashMap();
	}

	public void setVerbosity(int verbosity)
	{
		this.verbosity = verbosity;
	}

	private void setResultCode(int cr) throws IllegalArgumentException
	{
		if (cr < -1 || cr > 1)
		{
			throw new IllegalArgumentException("Not a valid result: " + cr);
		}

		m_result = cr;
	}

	public String getResultString()
	{
		String rs = "";
		if (m_result == NOT_EQUAL)
		{
			rs = "Not Equal";
		}
		else if (m_result == EQUAL)
		{
			rs = "Equal";
		}
		else if (m_result == NO_COMPARISON)
		{
			rs = "No Comparison";
		}
		return rs;
	}

	public void reset()
	{
		m_result = EQUAL;
		m_alreadyChecked.clear();
	}

	public int getResultCode()
	{
		return m_result;
	}

	public void compare(Object o1, Object o2) throws IllegalArgumentException
	{
		//System.out.println("class: " + o1.getClass().getCanonicalName() );

		// did this object already?
		if (m_alreadyChecked.get(o1) == o2)
		{
			return;
		}

		// add to checked map
		m_alreadyChecked.put(o1, o2);

		try
		{
			//System.out.println("comparisons...");

			// basic object comparison
			try
			{
				if (o1 instanceof Comparable)
				{
					//System.out.println("object");
					compareObject(o1, o2);
				}
			}
			catch (Throwable t)
			{
				System.out.println("error comparing object");
			}

			// array
			try
			{
				if (o1.getClass().isArray())
				{
					//System.out.println("array");
					compareArray(o1, o2);
				}
			}
			catch (Throwable t)
			{
				System.out.println("error comparing array");
			}

			// collection
			try
			{
				if (o1 instanceof Collection)
				{
					//System.out.println("collection");
					compareCollection(o1, o2);
				}
			}
			catch (Throwable t)
			{
				System.out.println("error comparing collection");
			}

			// bean
			try
			{
				compareBeanProperties(o1, o2);
			}
			catch (Throwable t)
			{
				System.out.println("error comparing bean properties");
			}

			// LCEvent
			try
			{
				if (o1 instanceof LCEvent)
				{
					//System.out.println("LCEvent");
					compareLCEvent(o1, o2);
				}
			}
			catch (Throwable t)
			{
				System.out.println("error comparing LCEvent");
				t.printStackTrace();
			}

		}
		catch (Throwable t)
		{
			throw new IllegalArgumentException("FATAL ERROR: Could not compare " + o1 + " " + o2);
		}
	}

	public void compareLCEvent(Object o1, Object o2)
	{
		String[] names = ((LCEvent) o1).getCollectionNames();
		for (int i = 0; i < names.length; i++)
		{

			if (verbosity > 1)
			{
				System.out.println("object compare: " + names[i]);
			}

			Collection c1 = ((LCEvent) o1).getCollection(names[i]);
			Collection c2 = ((LCEvent) o2).getCollection(names[i]);

			// missing coll; don't bother comparing the rest of them
			if (c1 == null || c2 == null)
			{

				if (verbosity > 1)
				{
					System.out.println("one of the events is missing coll: " + names[i]);
				}
				setResultCode(NOT_EQUAL);
				return;
			}

			// different size colls treated like array with different # elements
			if (c1.size() != c2.size())
			{
				if (verbosity > 1)
				{
					System.out.println(names[i] + " size is different: " + c1.size() + " " + c2.size());
				}
				setResultCode(NOT_EQUAL);
				// same size coll
			}
			else
			{

				// iterate over coll, comparing the objects
				Iterator i1 = c1.iterator();
				Iterator i2 = c2.iterator();
				while (i1.hasNext())
				{
					Object v1 = i1.next();
					Object v2 = null;
					if (i2.hasNext())
					{
						v2 = i2.next();
						// fewer elements in 2nd event
					}
					else
					{

						if (verbosity > 1)
						{
							System.out.println("2nd event has fewer objects in coll: " + names[i]);
						}
						setResultCode(NOT_EQUAL);
						break;
					}

					compare(v1, v2);
				}

				// more elements left in 2nd event
				if (i2.hasNext())
				{
					if (verbosity > 1)
					{
						System.out.println("2nd event has more objects in coll: " + names[i]);
					}
					setResultCode(NOT_EQUAL);
				}

			}
		}
	}

	public void compareObject(Object o1, Object o2)
	{
		// both null
		if (o1 == null && o2 == null)
			return;

		// one null
		if (o1 == null || o2 == null)
		{
			System.out.println("one object is null");
			setResultCode(NOT_EQUAL);
		}

		// comparison
		if (o1 instanceof Comparable)
		{
			int rc = ((Comparable) o1).compareTo(o2);
			if (rc != 0)
			{
				System.out.println("objects not equal: " + o1.getClass().getName() + " ( last method: " + m_method + " )");
				System.out.println(o1 + " != " + o2);
				setResultCode(NOT_EQUAL);
			}
		}
	}

	private void compareArray(Object o1, Object o2)
	{
		if (Array.getLength(o1) != Array.getLength(o2))
		{
			System.out.println("arr len not equal");
			setResultCode(NOT_EQUAL);
		}
		else
		{
			for (int i = 0; i < Array.getLength(o1); i++)
			{
				Object v1 = Array.get(o1, i);
				Object v2 = Array.get(o2, i);
				compare(v1, v2);
			}
		}
	}

	private void compareCollection(Object o1, Object o2)
	{
		Collection c1 = (Collection) o1;
		Collection c2 = (Collection) o2;
		if (c1.size() != c2.size())
		{
			System.out.println("coll size !=");
			setResultCode(NOT_EQUAL);
		}
		else
		{
			Iterator i1 = c1.iterator();
			Iterator i2 = c2.iterator();
			while (i1.hasNext())
			{
				Object v1 = i1.next();
				Object v2 = i2.next();
				compare(v1, v2);
			}
		}
	}

	private void compareBeanProperties(Object o1, Object o2)
	{
		//System.out.println("bean");

		try
		{
			BeanInfo info = Introspector.getBeanInfo(o1.getClass(), Object.class);
			PropertyDescriptor[] desc = info.getPropertyDescriptors();

			//if ( desc.length == 0 ) {
			//    System.out.println("WARNING: no bean properties for " + o1.getClass().getCanonicalName() );
			//}

			if (desc.length != 0)
			{
				for (int i = 0; i < desc.length; i++)
				{
					Method m = desc[i].getReadMethod();
					m_method = m;
					if (m != null)
					{
						try
						{
							Object v1 = m.invoke(o1, (Object[]) null);
							Object v2 = m.invoke(o2, (Object[]) null);
							compare(v1, v2);
						}
						catch (Throwable t)
						{
							/* just eat it for now */
							//System.out.println("skipping bean compare w/ error: " + desc[i].getDisplayName() );
							//t.printStackTrace();
						}
					}
				}
			}
		}
		catch (Throwable t)
		{
			System.out.println("error comparing bean properties");
			t.printStackTrace();
		}
	}
}
