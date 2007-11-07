package hep.lcio.util;

import hep.io.sio.SIOBlock;
import hep.io.sio.SIOReader;
import hep.io.sio.SIORecord;
import hep.lcio.event.LCEvent;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCReader;

import java.io.BufferedInputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;

/**
 * 
 * Several static utility methods for simple validation of LCIO/SIO files. 
 * 
 * @author jeremym
 * @version $Id: Validate.java,v 1.2 2007-11-07 20:46:24 jeremy Exp $
 */
public class Validate
{	
	/** 
	 * Confirm major and minor versions of an LCIO file.
	 * @param major The major version to match.
	 * @param minor The minor version to match.
	 * @throws If versions do not match, or if there is an IO error.
	 */
	public static void validateVersion(File f, int major, int minor) throws Exception
	{
		if (major == -1 && minor == -1)
		{
			return;
		}
		
		SIOReader rdr = 
			new SIOReader(new BufferedInputStream(new FileInputStream(f.getAbsolutePath())));
		
		for (;;)
		{
			SIORecord rec;
			try {
				rec = rdr.readRecord();
			}
			catch (EOFException e)
			{
				break;
			}
			
			SIOBlock block = rec.getBlock();
			
			if (block == null)
			{
				System.err.println("null block");
				break;
			}
			
			int bmajor = block.getMajorVersion();
			
			if (major != -1 && bmajor != major)
			{
				throw new Exception(block.getBlockName() + " : major version mismatch : " + bmajor + " != " + major);
			}
			
			int bminor = block.getMinorVersion();
			
			if (minor != -1 && bminor != minor)
			{
				throw new Exception(block.getBlockName() + " : minor version mismatch : " + bminor + " != " + minor);
			}
		}

		try { rdr.close(); } catch (Exception e) {}
	}
	
	/**
	 * Perform test read of an LCIO file.
	 * @param f The file to read.
	 * @param nevents The number of events to read.
	 * @throws Exception If file does not exist or does not have nevents.
	 */
	public static void testRead(File f, int nevents) throws Exception
	{
		LCReader rdr = LCFactory.getInstance().createLCReader();
		
		if (!f.exists())
		{
			throw new Exception(f.getAbsolutePath() + " does not exist.");
		}
		
		rdr.open(f.getAbsolutePath());
	
		int nread = 0;
				
		for (int i=0; i<nevents; i++)
		{
			LCEvent event = rdr.readNextEvent();
			if (event == null)
			{
				break;
			}
			++nread;
		}
		
		if (nread < nevents)
		{
			throw new Exception(f.getAbsolutePath() + " does not have " + nevents + " events.");
		}
		
		rdr.close();
	}
}