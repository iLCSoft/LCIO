package hep.lcio.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Miscellaneous java file utilities.
 * @author jeremym
 * @version $Id: FileUtil.java,v 1.4 2007-07-24 18:28:42 jeremy Exp $
 */
public abstract class FileUtil
{
	/**
	 * Reads the lines of file at path fileName into 
	 * an ArrayList.
	 * @param fileName Name of the input file.
	 * @return A List with one String file line per entry.
	 */
	public static List loadFile(String fileName)
	{
		if ((fileName == null) || (fileName == ""))
			throw new IllegalArgumentException("invalid fileName: "+fileName);

		String line;
		ArrayList file = new ArrayList();

		try
		{
			BufferedReader in = new BufferedReader(new FileReader(fileName));

			if (!in.ready())
				throw new IOException();

			while ((line = in.readLine()) != null)
				file.add(line);

			in.close();
		}
		catch (IOException e)
		{
			System.out.println(e);
			return null;
		}

		return file;
	}
	
	/**
	 * Return an array of @see File objects from an array of paths.
	 * @param fstr Array of file paths.
	 * @return Array of File objects.
	 */
	public static File[] createFiles(String[] fstr)
	{
		File[] infiles = new File[fstr.length];
		for (int i = 0; i < fstr.length; i++)
		{
			String ifile = (String) fstr[i];
			infiles[i] = new File(ifile);
			if (!infiles[i].exists())
				throw new RuntimeException("File " + infiles[i] + " does not exist!");
		}
		return infiles;
	}
}
