package hep.lcio.util;

import hep.io.xdr.XDRInputStream;
import hep.io.xdr.XDROutputStream;

import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.Iterator;
import java.util.List;

/**
 * Concatenation utility to combined many LCIO files into one.
 * @author Jeremy McCormick
 * @version $Id: Concatenate.java,v 1.1 2006-04-28 23:38:49 jeremy Exp $
 */
public class Concatenate
{
	/**
	 * Method to concatenate list of LCIO files to outfile.
	 * @param files A list of input files.
	 * @param outfile The output file.
	 * @throws Exception
	 */
	public static void concatenate(List files, File outfile) throws Exception
	{
		if (files.size() == 0)
		{
			System.err.println("concatenate - Input file list is empty!");
			return;
		}
		
		// Create the output stream.
		XDROutputStream xo = 
			new XDROutputStream(new FileOutputStream(outfile.getAbsolutePath()));

		// Loop over input files.
		for (Iterator iter = files.iterator(); iter.hasNext();) 
		{
			// Get the next file.
			File f = (File)iter.next();

			// Create the XDR input stream for this file.
			XDRInputStream xi =
				new XDRInputStream(new FileInputStream(f.getAbsolutePath()));
			
			// Record loop.
			for (;;)
			{
				WritableSIORecord rec = new WritableSIORecord();
				
				// Read in the record.
				try {
					rec.read(xi);
				}
				catch (EOFException x)
				{
					break;
				}
				
				// Write out the record.
				rec.write(xo);
			}
			
			// Close this input stream.
			xi.close();
		}
		
		// Close the output stream.
		xo.close();
	}
}
