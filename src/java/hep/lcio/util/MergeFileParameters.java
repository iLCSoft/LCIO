package hep.lcio.util;

import hep.lcio.event.LCEvent;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCReader;
import org.apache.commons.math.distribution.DistributionFactory;
import org.apache.commons.math.distribution.PoissonDistribution;
import java.util.Random;

import java.io.File;
import java.io.IOException;

/**
 * Encapsulates parameters for events to be merged.
 * The settings are input file, number of reads per event,
 * start time, and delta time.
 * 
 * @author jeremym
 * @version $Id: MergeFileParameters.java,v 1.2 2009-05-22 20:18:59 cassell Exp $
 */
public final class MergeFileParameters
{		
	int nreads = 1;
        double mreads = 0.;
	float startt = 0;
	float dt = 0;
	File f;		
        PoissonDistribution _poisson;
	LCReader reader = LCFactory.getInstance().createLCReader();
	DistributionFactory df = DistributionFactory.newInstance();
        Random ran;
	
	MergeFileParameters(File f, int nreads, float startt, float dt) throws IOException
	{
		this.f = f;
		this.nreads = nreads;
		this.startt = startt;
		this.dt = dt;
		
		open();
	}
	
	MergeFileParameters(File f, double mreads, float startt, float dt) throws IOException
	{
		this.f = f;
		this.mreads = mreads;
		this.startt = startt;
		this.dt = dt;
	        _poisson = df.createPoissonDistribution( mreads );
                ran = new Random();
		
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
                if(mreads > 0.)
                {
                   int n = 0;
                   try
                   {
                      n = _poisson.inverseCumulativeProbability(ran.nextDouble()) + 1;
                   }
                   catch(Exception e)
                   {
                      System.out.println("Poisson error: "+e.toString());
                   }
                   return n;
                }
		else return nreads;	
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