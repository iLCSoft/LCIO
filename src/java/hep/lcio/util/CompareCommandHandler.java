package hep.lcio.util;

import hep.lcio.event.LCEvent;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.ILCFactory;
import hep.lcio.io.LCReader;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;

/**
 * CommandHandler for the compare command.
 * 
 * @author jeremym
 * @version $Id: CompareCommandHandler.java,v 1.2 2006-12-07 00:47:32 jeremy Exp $
 */
public class CompareCommandHandler extends CommandHandler
{
	List files = new ArrayList();
	List readers = new ArrayList();
	int verbosity = 1;
	int ntoskip = 0;
	int ntocompare = 1;

	public CompareCommandHandler()
	{
		super("compare", "Compare events from two LCIO files.");

		options = createCompareOptions();
	}

	/**
	 * Creates the CLI options for the compare command.
	 * @return Options for the compare command.
	 */
	private static Options createCompareOptions()
	{
		Options options = new Options();
		
		Option opt = new Option("h", false, "Print compare usage.");
		options.addOption(opt);

		opt = new Option("f", false, "Add a file to compare (Must have at least 2).");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("n", true, "Set number of events to compare.");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("s", true, "Set number of events to skip.");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("v", true, "Set verbosity level (must be between 1 and 4).");
		opt.setArgs(1);
		options.addOption(opt);

		return options;
	}

	/** 
	 * Parse the command line options for the compare command using apache CLI.
	 * @param argv The input arguments for the compare command. 
	 */
	public void parse(String[] argv) throws Exception
	{
		CommandLine cl = parser.parse(options, argv);
			
		if ( cl.hasOption("h") )
		{
			printUsage(true);
		}		

		String f[] = cl.getOptionValues("f");
		
		if (f != null && f.length >= 2)
		{
			for (int i = 0; i < f.length; i++)
			{
				files.add(new File(f[i]));
			}
		}
		
		if (cl.getArgList() != null)
		{
			for (Object o : cl.getArgList())
			{
				String s = (String)o;
				files.add(new File(s));
			}			
		}
		
		if ( files.size() == 0 )
		{
			printUsage(true);
		}

		if (cl.hasOption("n"))
		{
			ntocompare = Integer.parseInt(cl.getOptionValue("n"));
		}

		if (cl.hasOption("s"))
		{
			ntoskip = Integer.parseInt(cl.getOptionValue("s"));
		}

		if (cl.hasOption("v"))
		{
			verbosity = Integer.parseInt(cl.getOptionValue("v"));
		}
	}

	/** 
	 * Execute the merge compare with current arguments. 
	 */
	public void execute() throws Exception
	{
		ILCFactory factory = LCFactory.getInstance();

		// Setup all the LCReaders.
		for (int i = 0; i < files.size(); i++)
		{
			LCReader rdr = factory.createLCReader();
			rdr.open(((File) files.get(i)).getAbsolutePath());

			if (ntoskip > 0)
			{
				System.out.println("Skipping " + ntoskip + " events in file " + ((File)files.get(i)).getAbsolutePath());
				rdr.skipNEvents(ntoskip);
			}

			readers.add(rdr);
		}

		// Compare each event to base event.
		for (int i = 0; i < ntocompare; i++)
		{
			LCEvent baseevent = ((LCReader) readers.get(0)).readNextEvent();
			
			if (baseevent == null) break;
			
			LCEvent nextevent = null;
			for (int j = 1; j < readers.size(); j++)
			{
				ObjectComparator cmp = new ObjectComparator();
				cmp.setVerbosity(verbosity);
				
				nextevent = ((LCReader) readers.get(j)).readNextEvent();
				
				if (nextevent == null) break;
				
				cmp.compare(baseevent, nextevent);
	
				// Print result.
				System.out.println("Compare event " + baseevent.getEventNumber() + " from " + ((File)files.get(0)).getName() + " with event " + nextevent.getEventNumber() + " from " + ((File)files.get(j)).getName() + " : " + cmp.getResultString());
			}
			
			if (nextevent == null) break;
		}

		// Close the LCReaders.
		for (int i = 0; i < readers.size(); i++)
		{
			((LCReader) readers.get(i)).close();
		}
	}
}