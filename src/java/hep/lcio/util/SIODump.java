package hep.lcio.util;

import gnu.jel.*;
import hep.io.sio.*;
import hep.lcio.event.LCIO;

import java.io.BufferedInputStream;
import java.io.EOFException;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.JDOMException;
import org.jdom.input.SAXBuilder;
import org.jdom.xpath.XPath;

/**
 * A utility for making ASCII dumps of LCIO files.
 * This utility uses an XML file to describe the format of the LCIO file,
 * and produces an ASCII dump of the LCIO file based on the information from
 * the XML file. The level of detail can be set using the verbosity flag
 * <ul>
 * <li>0 - No output
 * <li>1 - One line per record (default)
 * <li>2 - Medium verbosity
 * <li>3 - Complete dump of all contents
 * </ul>
 * This utility can also be used to check the consistency of an XML file with
 * a given LCIO file.
 * <p>
 * Usage:
 * <pre>
 *   java hep.lcio.util.SIODump [-v n] <xml-file> <lcio-file>
 * </pre>
 * @author tonyj
 */
public class SIODump
{
	private static final Pattern pattern1 = Pattern.compile("(\\w+)(?:\\[(.*)\\])?");
	private String recordName;
	private String blockName;
	private Map blockMap = new HashMap();
	private Map compilers = new HashMap();
	private Map dimensions = new HashMap();
	private static int verbosity = 1;
	private static int nevttoread = -1;
	private int nWarnings = 0;
	private List output = new LinkedList();
	private int global = 0;
	private Set versionAlreadyWarned = new HashSet();
	private Map subroutines = new HashMap();

	/**
	 * @param args the command line arguments
	 */
	public static void main(String[] args) throws IOException, JDOMException
	{
		String[] params = new String[2];
		int nParams = 0;
		for (int i = 0; i < args.length; i++)
		{
			if (args[i].equals("-v"))
				verbosity = Integer.parseInt(args[++i]);
			else
			{
				if (nParams >= 2)
					help();
				params[nParams++] = args[i];
			}
		}

		if (nParams < 2)
			help();

		SAXBuilder builder = new SAXBuilder();
		Document doc = builder.build(params[0]);
		Element root = doc.getRootElement();

		SIOReader reader = new SIOReader(new BufferedInputStream(new FileInputStream(params[1])));
		SIODump dumper = new SIODump();
		dumper.dump(root, reader);
	}

	public SIODump()
	{
	}

	public static void setVerbosity(int v)
	{
		verbosity = v;
	}

	public static void setMaxEvents(int e)
	{
		nevttoread = e;
	}

	public void dump(Element root, SIOReader reader) throws IOException, JDOMException
	{
		try
		{
			// Build subroutine list
			for (Iterator i = root.getChildren("subroutine").iterator(); i.hasNext();)
			{
				Element node = (Element) i.next();
				subroutines.put(node.getAttributeValue("name"), node);
			}
			int n = 0;
			int nevt = 0;
			for (;;)
			{
				SIORecord record;
				try
				{
					record = reader.readRecord();
				}
				catch (EOFException x)
				{
					break;
				}
				String recName = record.getRecordName();

				if (recName.compareTo(LCIO.LCEVENT) == 0)
				{
					++nevt;
				}
				Element recordElement = (Element) XPath.selectSingleNode(root, "record[@name=\"" + recName + "\"]");
				if (recordElement == null)
				{
					warn("Skipping unrecognized record: " + recName);
				}
				else
				{
					global = 0;
					dumpRecord(record, recordElement);
					flush();
				}

				n++;

				if (nevttoread > 0)
				{
					if (nevt >= nevttoread)
					{
						break;
					}
				}
			}
			dump(0, "Read " + n + " records with " + nWarnings + " warnings");
		}
		finally
		{
			reader.close();
			flush();
		}
	}

	private void dumpRecord(SIORecord record, Element root) throws IOException, JDOMException
	{
		recordName = record.getRecordName();
		dump(1, "Record: " + recordName);
		for (;;)
		{
			SIOBlock block = record.getBlock();
			if (block == null)
				return;
			String blockName = block.getBlockName();
			// LCIO Special
			Object blockType = blockMap.get(blockName);
			if (blockType == null)
				blockType = blockName;
			//fg -- allow one generic bloc for references         
			if (blockType.toString().endsWith("_References"))
				blockType = "References";

			Element blockElement = (Element) XPath.selectSingleNode(root, "block[@name=\"" + blockType + "\"]");
			if (blockElement == null)
			{
				warn("Skipping unrecognized block type: " + blockType);
			}
			else
			{
				dumpBlock(block, blockElement);
			}
		}
	}

	private void dumpBlock(SIOBlock block, Element root) throws IOException, JDOMException
	{
		blockName = block.getBlockName();
		String version = block.getMajorVersion() + "." + block.getMinorVersion();
		dump(2, "   Block: " + blockName + " (v" + version + ")");
		String expectedVersion = root.getAttributeValue("major") + "." + root.getAttributeValue("minor");
		String key = blockName + ":" + version;
		if (!version.equals(expectedVersion) && !versionAlreadyWarned.contains(key))
		{
			warn("For block " + blockName + " expected v" + expectedVersion + " got v" + version);
			versionAlreadyWarned.add(key);
		}
		Map values = new HashMap();
		values.put("major", new Integer(block.getMajorVersion()));
		values.put("minor", new Integer(block.getMinorVersion()));
		SIOInputStream data = block.getData();
		dumpValues(data, root, "      ", values);
		if (data.available() != 0)
			warn(data.available() + " unread bytes in block " + blockName);
	}

	private void dumpValues(SIOInputStream data, Element root, String indent, Map values) throws IOException, JDOMException
	{
		dumpValues(data, root, indent, values, false);
	}

	private void dumpValues(SIOInputStream data, Element root, String indent, Map values, boolean skip) throws IOException, JDOMException
	{
		Iterator it = root.getChildren().iterator();
		while (it.hasNext())
		{
			Element node = (Element) it.next();
			String nodeName = node.getName();
			if (nodeName.equals("else"))
			{
				skip = !skip;
			}
			else if (skip)
			{
			}
			else if (nodeName.equals("data"))
			{
				String type = node.getAttributeValue("type");
				String name = node.getAttributeValue("name");
				Object value = readValue(data, type, values);
				dump(3, indent + name + ": ", value);
				values.put(name, value);
			}
			else if (nodeName.equals("include"))
			{
				String subroutine = node.getAttributeValue("subroutine");
				Element subNode = (Element) subroutines.get(subroutine);
				if (subNode == null)
					throw new JDOMException("Unknown subroutine " + subroutine + " included");
				dumpValues(data, subNode, indent, values);
			}
			else if (nodeName.equals("repeat"))
			{
				String countName = node.getAttributeValue("count");
				int icount = evaluateInteger(countName, values, node);
				for (int i = 0; i < icount; i++)
				{
					dumpValues(data, node, indent + "[" + i + "]", values);
					// LCIO Special
					if (recordName.equals("LCEventHeader") && blockName.equals("EventHeader"))
					{
						blockMap.put(values.get("blockName"), values.get("blockType"));
					}
				}
                                // ToDo: Check what SIO rules are for padding
                                data.pad();
			}
			else if (nodeName.equals("if"))
			{
				String condition = node.getAttributeValue("condition");
				boolean booleanCondition = evaluateBoolean(condition, values, node);
				dumpValues(data, node, indent, values, !booleanCondition);
			}
			else
				warn("Ignoring element: " + node.getName());
		}
	}

	private Object readValue(SIOInputStream data, String declaration, Map values) throws IOException, JDOMException
	{
		String[] dims = (String[]) dimensions.get(declaration);
		if (dims == null)
		{
			//TODO: Currently only support a single dimension
			Matcher matcher = pattern1.matcher(declaration);
			if (!matcher.matches())
				throw new JDOMException("Invalid type: " + declaration);
			dims = new String[2];
			dims[0] = matcher.group(1).intern();
			dims[1] = matcher.group(2);
			dimensions.put(declaration, dims);
		}
		String type = dims[0];
		if (dims[1] != null)
		{
			int iDim = evaluateInteger(dims[1], values, declaration);
			StringBuffer result = new StringBuffer("[");
			for (int i = 0;;)
			{
				result.append(readValue(data, type, values));
				if (++i == iDim)
					break;
				result.append(",");
			}
			result.append("]");
			return result.toString();
		}
		else
		{
			if (type == "int")
				return new Integer(data.readInt());
			else if (type == "long")
				return new Long(data.readLong());
			else if (type == "short")
				return new Short(data.readShort());
			else if (type == "byte")
				return new Byte(data.readByte());
			else if (type == "double")
				return new Double(data.readDouble());
			else if (type == "float")
				return new Float(data.readFloat());
			else if (type == "boolean")
				return data.readBoolean() ? Boolean.TRUE : Boolean.FALSE;
			else if (type == "string")
				return data.readString();
			else if (type == "ptag")
			{
				String objectName = "Object[" + (global++) + "]";
				data.readPTag(objectName);
				return objectName;
			}
			else if (type == "pntr")
				return data.readPntr();
			else
				throw new JDOMException("Unrecognized type: " + type);
		}
	}

	private int evaluateInteger(String expression, Map values, Object key) throws IOException, JDOMException
	{
		try
		{
			Resolver resolver = new Resolver(values);
			CompiledExpression expr_c = (CompiledExpression) compilers.get(key);
			if (expr_c == null)
			{
				expr_c = Evaluator.compile(expression, createJELLibrary(resolver), Integer.TYPE);
				compilers.put(key, expr_c);
			}
			return expr_c.evaluate_int(new Object[]
			{ resolver });
		}
		catch (CompilationException x)
		{
			JDOMException xx = new JDOMException("Invalid expression: " + expression);
			xx.initCause(x);
			throw xx;
		}
		catch (Throwable x)
		{
			JDOMException xx = new JDOMException("Error evaluating expression: " + expression);
			xx.initCause(x);
			throw xx;
		}
	}

	private boolean evaluateBoolean(String expression, Map values, Object key) throws IOException, JDOMException
	{
		try
		{
			Resolver resolver = new Resolver(values);
			CompiledExpression expr_c = (CompiledExpression) compilers.get(key);
			if (expr_c == null)
			{
				expr_c = Evaluator.compile(expression, createJELLibrary(resolver), Boolean.TYPE);
				compilers.put(key, expr_c);
			}
			return expr_c.evaluate_boolean(new Object[]
			{ resolver });
		}
		catch (CompilationException x)
		{
			JDOMException xx = new JDOMException("Invalid expression: " + expression);
			xx.initCause(x);
			throw xx;
		}
		catch (Throwable x)
		{
			JDOMException xx = new JDOMException("Error evaluating expression: " + expression);
			xx.initCause(x);
			throw xx;
		}
	}

	private Library createJELLibrary(Resolver resolver)
	{
		Class[] dynamicLib =
		{ Resolver.class };
		Class[] staticLib = new Class[1];
		try
		{
			staticLib[0] = Class.forName("java.lang.Math");
		}
		catch (ClassNotFoundException e)
		{
			// Can't be ;)) ...... in java ... ;)
		}
		;
		return new Library(staticLib, dynamicLib, null, resolver, null);
	}
	public class Resolver implements DVResolver
	{
		private Map values;

		Resolver(Map values)
		{
			this.values = values;
		}

		public String getTypeName(String str)
		{
			Object value = values.get(str);
			if (value == null)
				return null;
			else
			{
				if (value instanceof String)
					return "String";
				else if (value instanceof Integer)
					return "Integer";
				else if (value instanceof Long)
					return "Long";
				else if (value instanceof Short)
					return "Short";
				else if (value instanceof Byte)
					return "Byte";
				else if (value instanceof Float)
					return "Float";
				else if (value instanceof Double)
					return "Double";
				else if (value instanceof Boolean)
					return "Boolean";
				else
					return "Object";
			}
		}

		public double getDoubleProperty(String name)
		{
			return ((Number) values.get(name)).doubleValue();
		}

		public int getIntegerProperty(String name)
		{
			return ((Number) values.get(name)).intValue();
		}

		public long getLongProperty(String name)
		{
			return ((Number) values.get(name)).longValue();
		}

		public short getShortProperty(String name)
		{
			return ((Number) values.get(name)).shortValue();
		}

		public byte getByteProperty(String name)
		{
			return ((Number) values.get(name)).byteValue();
		}

		public float getFloatProperty(String name)
		{
			return ((Number) values.get(name)).floatValue();
		}

		public boolean getBooleanProperty(String name)
		{
			return ((Boolean) values.get(name)).booleanValue();
		}

		public String getStringProperty(String name)
		{
			return values.get(name).toString();
		}

		public Object getObjectProperty(String name)
		{
			return values.get(name);
		}
	}

	private void warn(String message)
	{
		nWarnings++;
		System.out.println("Warning: " + message);
	}

	private void dump(int level, String message)
	{
		if (verbosity >= level)
			output.add(message);
	}

	private void dump(int level, String message, Object value)
	{
		if (verbosity >= level)
		{
			if (value instanceof SIORef)
			{
				output.add(value);
				output.add(message);
			}
			else
			{
				output.add(message + value);
			}
		}
	}

	private void flush()
	{
		Iterator i = output.iterator();
		while (i.hasNext())
		{
			Object line = i.next();
			if (line instanceof SIORef)
			{
				SIORef ref = (SIORef) line;
				line = i.next();
				Object obj = ref.getObject();
				System.out.println(line.toString() + obj);
			}
			else
				System.out.println(line);
		}
		output.clear();
	}

	private static void help()
	{
		System.out.println("java " + SIODump.class.getName() + " [options] <xml-file> <input-file>");
		System.out.println("   options:   -v n  sets the verbosity (default 1)");
		System.exit(1);
	}
}
