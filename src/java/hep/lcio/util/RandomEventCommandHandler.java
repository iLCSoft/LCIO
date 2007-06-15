package hep.lcio.util;

import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCWriter;
import hep.lcio.test.RandomEvent;

import java.io.File;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;

/**
 * CommandHandler for the <i>random</i> command.
 * 
 * Generates an {@link hep.lcio.event.LCEvent} containing random data.
 * 
 * @author jeremym
 * @version $Id: RandomEventCommandHandler.java,v 1.3 2007-06-15 23:15:28 jeremy Exp $
 */
public class RandomEventCommandHandler extends CommandHandler
{
	File outfile;
	int nevt = 1;
	int maxobj = 100;
	
	public RandomEventCommandHandler()
	{
		super("random","Create LCEvents filled with randomly generated data.");
		
		options = createRandomEventOptions();
	}
	
	/** 
	 * Create options for the random command.
	 */
	private static Options createRandomEventOptions()
	{
		Options options = new Options();
		
		Option opt = new Option("h", false, "Print random usage.");
		options.addOption(opt);
		
		opt = new Option("o", true, "Set the LCIO output file name.");
		opt.setArgs(1);
		options.addOption(opt);
		
		opt = new Option("n", true, "Set the number of random events to generate.");
		opt.setArgs(1);
		options.addOption(opt);
		
		opt = new Option("m", true, "Set the maximum number of objects in a collection.");
		opt.setArgs(1);
		options.addOption(opt);
		
		return options;
	}
	
	/**
	 * Execute the random command with current options.
	 */
	public void execute() throws Exception
	{
		LCWriter writer = LCFactory.getInstance().createLCWriter();
		writer.open(outfile.getAbsolutePath());
		
		for (int i=0; i<nevt; i++)
		{
			writer.writeEvent(new RandomEvent(maxobj));
		}
		
		writer.close();
	}

	/**
	 * Parse the raw options for the random command.
	 */
	public void parse(String[] argv) throws Exception
	{
		CommandLine cl = parser.parse(options, argv);
		
		if ( cl.hasOption("h") )
		{
			printUsage(true);
		}		
		
		String fname = "random_events";
		if (cl.hasOption("o"))
		{
			fname = cl.getOptionValue("o");
		}
		outfile = new File(fname);
		
		if (cl.hasOption("n"))
		{
			nevt = Integer.parseInt(cl.getOptionValue("n"));
			
			if (nevt < 1)
			{
				System.err.println("WARNING: Ignoring invalid value " + nevt + " for the -m switch.");
				nevt = 1;
			}
		}
		
		if (cl.hasOption("m"))
		{
			maxobj = Integer.parseInt(cl.getOptionValue("m"));
			
			if (maxobj < 1)
			{
				System.err.println("WARNING: Ignoring invalid value " + maxobj + " for the -m switch.");
				maxobj = 1;
			}
		}
	}
}
