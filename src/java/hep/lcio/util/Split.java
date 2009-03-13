package hep.lcio.util;

import hep.io.xdr.XDRInputStream;
import hep.io.xdr.XDROutputStream;
import hep.lcio.event.LCIO;

import java.io.BufferedInputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * Split utility that takes an LCIO file and breaks it into
 * chunks of given size.
 * 
 * @author Jeremy McCormick
 * @version $Id: Split.java,v 1.3 2009-03-13 18:40:04 jeremy Exp $
 */
public class Split
{
	/**
	 * Static utility function that splits infile into chunks with nevents.
	 * @param infile A File object pointing to an input LCIO file.
	 * @param nevents The number of events in each new output file.
	 * @throws Exception
	 */
	public static List split(File infile, File outdir, int nevents, int maxevents) throws Exception
	{			
		if (infile == null)
		{
			throw new IllegalArgumentException("The infile argument points to null.");
		}
		
		if (!infile.exists())
		{
			throw new IllegalArgumentException("Input file " + infile.toString() + " does not exist.");
		}
		                		
		// Decide where the files will go.  Uses either the -d argument from
		// the command line, if it was set.  Or by default the files will
		// be created in the current working directory.
		String outpath = null;
		if (outdir != null)
		{
			if (outdir.exists() && outdir.isDirectory() && outdir.canWrite())
			{
				outpath = outdir.getCanonicalPath();
			}
			else
			{
				throw new IllegalArgumentException("Directory " + outdir.toString() + " is not valid."); 
			}
		}
		else
		{
			outpath = System.getProperty("user.dir");
		}
		
		// List of files created.
		List<File> outfilelist = new ArrayList<File>();

		// Base name from input file name.
		String basename = infile.getName().replace(".slcio", "");
				
		// XDR input stream from file that is being split up.
		XDRInputStream xi = null;
		try
		{
			// Create the XDR input stream.
			xi = new XDRInputStream(new FileInputStream(infile.getAbsolutePath()));			
		}
		catch (FileNotFoundException x)
		{
			// The input file does not exist.
			throw new RuntimeException("File " + infile.getAbsolutePath() + " does not exist.", x);
		}
						
		// Output file sequence number.
		int filenum = 0;

		// Number of records processed.
		int nrecs = 0;

		// Number of records read with name 'LCEvent'.  This flag is reset when the next file is started.
		int neventsread = 0;

		// Flag to indicate processing should end.
		boolean done = false;

		// file loop
		String outfile;
        int totalevents=0;
		for (;;)
		{
			// Next output file name.
			outfile = basename + "-" + String.valueOf(filenum) + "-" + nevents + ".slcio";
			
			// Add base directory for output if specified.
			if (outpath != null)
			{
				outfile = outpath + File.separator + outfile;
			}
									
			// The output stream for this file.
			XDROutputStream xo;

			try
			{
				// FIXME: Using BufferedOutputStream seems to corrupt the output files.
				//xo = new XDROutputStream(new BufferedOutputStream(new FileOutputStream(outfile)));
				xo = new XDROutputStream(new FileOutputStream(outfile));
			}
			catch (FileNotFoundException x)
			{
				throw new RuntimeException("Error creating output file " + outfile);
			}

			// The record read loop.
			for (;;)
			{
				// Create a new WritableSIORecord that can do stream read, write.
				WritableSIORecord myrec = new WritableSIORecord();

				// Read in a record.
				try
				{
					myrec.read(xi);
				}
				catch (EOFException x)
				{
					done = true;
					break;
				}
				catch (IOException x)
				{
					throw new RuntimeException(x);
				}

				// Write out the record.
				if (!done)
				{
					try
					{
						myrec.write(xo);
						xo.flush();

						String recname = myrec.getName();
						if (recname.compareTo(LCIO.LCEVENT) == 0)
						{
							// Increment number of events.
							++neventsread;
                            ++totalevents;
						}

						// Increment number of records.
						++nrecs;
					}
					catch (IOException x)
					{
						throw new RuntimeException("Error writing record " + (nevents - 1), x);
					}
				}
                
                if (maxevents != -1 && totalevents == maxevents)
                {
                    done = true;
                }

				if (neventsread == nevents || done)
				{
					break;
				}
			}

			// Add this file to the output file list.
			outfilelist.add(new File(outfile));

			// Close the output stream.
			try
			{
				xo.flush();
				xo.close();
				xo = null;
			}
			catch (IOException x)
			{
				System.err.println("Error closing output stream.\n" + x.getMessage());
			}

			// If done flag is set, then stop processing.
			if (done)
				break;

			// Increment file sequence number.
			filenum += 1;

			// Reset event counter.
			neventsread = 0;

			// Reset record counter.
			nrecs = 0;
		}

		// Close the input stream.
		try
		{
			xi.close();
			xi = null;
		}
		catch (IOException x)
		{
			System.err.println("Error closing input stream.\n" + x.getMessage());
		}

		// Delete the last file as it doesn't have any events.
        if (neventsread == 0)
        {
            outfilelist.remove(outfilelist.size() - 1);
            (new File(outfile)).delete();
        }

		// Return a list of files created.
		return outfilelist;
	}

	public static void printSplitSummary(List outfilelist, PrintStream ps) throws Exception
	{
		ps.println();
		ps.println("--- Split Summary ---");
		ps.println();
		
		// Read back events.
		for (Iterator iter = outfilelist.iterator(); iter.hasNext();)
		{
			File f = (File) iter.next();

			ps.println(f.getAbsoluteFile());

			XDRInputStream xi = new XDRInputStream(new BufferedInputStream(new FileInputStream(f.getAbsolutePath())));

			int nrec = 0;
			int nevt = 0;

			for (;;)
			{
				WritableSIORecord rec = new WritableSIORecord();

				try
				{
					rec.read(xi);
					++nrec;
					if (rec.getName().compareTo("LCEvent") == 0)
					{
						++nevt;
					}
				}
				catch (EOFException x)
				{
					ps.println(nrec + " records");
					ps.println(nevt + " events");
					xi.close();
					break;
				}
			}

			ps.println();
		}
	}
}
