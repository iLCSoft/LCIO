package hep.lcio.util;

import hep.lcio.event.LCEvent;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCReader;

import java.io.File;
import java.io.IOException;

/**
 * Encapsulates parameters for events to be merged.
 * The settings are input file, number of reads per event,
 * start time, and delta time.
 * 
 * @author jeremym
 * @version $Id: MergeFileParameters.java,v 1.1 2006-04-26 19:37:05 jeremy Exp $
 */
public final class MergeFileParameters
{		
	int nreads = 1;
	float startt = 0;
	float dt = 0;
	File f;		
	LCReader reader = LCFactory.getInstance().createLCReader();
	
	MergeFileParameters(File f, int nreads, float startt, float dt) throws IOException
	{
		this.f = f;
		this.nreads = nreads;
		this.startt = startt;
		this.dt = dt;
		
		open();
	}
	
	MergeFileParameters(File f) throws IOException
	{
		this.f = f;
		
		open();
	}
	
	/**
	 * Get the number of reads per event.
	 * @return The number of reads to do per output event.
	 */
	public int nreads()
	{
		return nreads;	
	}
	
	/**
	 * Get the starting time in ns.
	 * @return Start time in ns.
	 */
	public float startt()
	{
		return startt;
	}
	
	/**
	 * Get the delta time in ns.
	 * @return Delta time in ns.
	 */
	public float dt()
	{
		return dt;
	}
	
	/**
	 * Get a file object for these input events.
	 * @return The file corresponding to these input events.
	 */
	public File file()
	{
		return f;
	}

	/**
	 * Get the LCReader associated with these input events.
	 * @return The LCReader from this input file.
	 */
	public LCReader reader()
	{
		return reader;
	}
	
	/**
	 * Open the LCReader.
	 * @throws IOException
	 */
	public void open() throws IOException
	{
		reader.open(file().getAbsolutePath());
	}
	
	/**
	 * Close the LCReader.
	 * @throws IOException
	 */
	public void close() throws IOException
	{
		reader.close();
	}
	
	/**
	 * Get the next event.
	 * @return @see LCReader.readNextEvent
	 * @throws IOException
	 */
	public LCEvent nextEvent() throws IOException
	{
		return reader.readNextEvent();
	}
}