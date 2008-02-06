package hep.lcio.util;

import hep.io.sio.SIOBlock;
import hep.io.sio.SIOReader;
import hep.io.sio.SIORecord;
import hep.lcio.event.LCIO;

import java.io.BufferedInputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Options;

/**
 * A utility for printing the size in bytes of SIO collections.
 * @author jeremym
 */
public class SizeCommandHandler 
extends CommandHandler
{
	File file = null;
	int nskip = -1;
	int nread = -1;
	Map<String,Integer> totals = new HashMap<String,Integer>();
	
	public SizeCommandHandler()
	{
		super("size","Print the size in bytes of LCIO blocks.");
		
		Options options = new Options();
		
		options.addOption("n", true, "Maximum number of events to read.");
		options.addOption("s", true, "Number of events to skip.");	
	}
	
	public void parse(String[] argv) throws Exception 
	{			
		CommandLine cl = parser.parse(options, argv);
		
		if (cl.hasOption("n"))
		{
			nread = Integer.parseInt(cl.getOptionValue("n"));
			System.out.println("Set nread " + nread);
			if (nread < 1)
				throw new IllegalArgumentException("The -n argument must be greater than 0.");
		}
		
		if (cl.hasOption("s"))
		{
			nskip = Integer.parseInt(cl.getOptionValue("s"));
			if (nskip < 1)
				throw new IllegalArgumentException("The -s argument must be greater than 0.");
		}			
		
		if (cl.getArgList() != null)
		{
			file = new File(cl.getArgs()[0]);
			if (!file.exists())
				throw new IllegalArgumentException("The file " + file.getAbsolutePath() + " does not exist!");
		}
	}
	
	public void execute() throws Exception 	
	{		
		SIOReader reader = null;
		try {
			reader = new SIOReader(new BufferedInputStream(new FileInputStream(file.getAbsolutePath())));
		}
		catch (Exception e)
		{
			throw new RuntimeException(e);
		}
		
		System.out.println("Opened file " + file.getAbsolutePath());
		
		// Skip events.
		if (nskip != -1)
		{
			int skipped = 0;
			for (;;)			
			{				
				SIORecord record = reader.readRecord();
				if (record.getRecordName().equals(LCIO.LCEVENT))
				{
					++skipped;
				}
				if (skipped >= nskip)
				{
					break;
				}
			}
			System.out.println("Skipped " + skipped + " events");
		}
		
		int eventsread = 0;
		for (;;)
		{			
			SIORecord record;
						
			try {
				record = reader.readRecord();
			}
			catch (EOFException x)
			{
				break;
			}
			catch (IOException x)
			{
				throw new RuntimeException(x);
			}
									
			if (record.getRecordName().equals(LCIO.LCEVENT))
			{	
				System.out.println();
				System.out.println("EVENT : " + (eventsread + 1) );
				System.out.println();
 
				SIOBlock block = null;			
				boolean readBlocks = false;
				while ((block = record.getBlock()) != null)
				{
					if (!readBlocks) readBlocks = true;
					System.out.format("%-40s %d\n", block.getBlockName(), block.getBlockLength());
					if (!totals.containsKey(block.getBlockName()))
					{
						totals.put(block.getBlockName(), 0);
					}
					int total = totals.get(block.getBlockName());
					total += block.getBlockLength();
					totals.put(block.getBlockName(), total);
				}			
				
				if (readBlocks)
				{
					++eventsread;
			
					if (nread != -1 && eventsread >= nread)
					{
						break;
					}
				}
			}			
		}
		
		System.out.println();
		System.out.println("Read " + eventsread + " events.");
		
		System.out.println();
		System.out.println("TOTALS");
		System.out.println();
		System.out.format("%-40s %10s %10s\n","COLLECTION","TOT BYTES","AVG BYTES");
		for (Entry<String,Integer> total : totals.entrySet())
		{
			System.out.format("%-40s %10d %10d\n",total.getKey(),total.getValue(),total.getValue() / eventsread);
		}
	}
}