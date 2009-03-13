package hep.lcio.util;

import java.io.File;
import java.util.List;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;

/**
 * Command-line handler for the split utility.
 * 
 * @author Jeremy McCormick
 * @version $Id: SplitCommandHandler.java,v 1.6 2009-03-13 18:40:04 jeremy Exp $
 */
public class SplitCommandHandler extends CommandHandler
{
	File infile;
	int nevents;
    int maxevents=-1;
    File outdir=null;
	
	SplitCommandHandler()
	{
		super("split","Split a single LCIO file into multiple files.");
		
		// Setup split command options.
		options = createSplitOptions();
	}
	
	/**
	 * Create the options for the split command.
	 * @return An Apache CLI Options object with split options.
	 */
	private static Options createSplitOptions()
	{
		Options options = new Options();
	
		Option opt = new Option("h",false,"Print split usage.");
		options.addOption(opt);
		
		opt = new Option("i", true,"The input LCIO file.");
		opt.setArgs(1);
		options.addOption(opt);
		
		opt = new Option("n", true,"The number of events to split.");
		opt.setArgs(1);
		options.addOption(opt);
        
        opt = new Option("m", false,"The maximum number of events to read.");
        opt.setArgs(1);
        options.addOption(opt);
                
        opt = new Option("d",false,"The output directory for split files; defaults to input file's directory.");
        opt.setArgs(1);
        options.addOption(opt);
		
		return options;
	}

	/**
	 * Parse the argv for split command options.
	 * @param argv The raw command array from command line. 
	 * @see hep.lcio.util.CommandHandler#parse(java.lang.String[])
	 */
	public void parse(String[] argv) throws Exception
	{
		CommandLine cl = parser.parse(options, argv);
		
		if ( cl.hasOption("h") )
		{
			printUsage(true);
		}
		
		if (!cl.hasOption("i") || !cl.hasOption("n"))
		{
			printUsage(true);
		}
		
		String inputpath = null;
		if (cl.hasOption("i"))
		{
			inputpath = cl.getOptionValue("i");
			infile = new File(inputpath);
		}
		
		if (cl.hasOption("n"))
		{
			nevents = Integer.parseInt(cl.getOptionValue("n"));
		}
        
        if (cl.hasOption("m"))
        {
            maxevents = Integer.parseInt(cl.getOptionValue("m"));
        }
        
        if (cl.hasOption("d"))
        {
        	outdir = new File(cl.getOptionValue("d"));
        	if (!outdir.exists())
        	{
        		throw new IllegalArgumentException("The directory " + outdir.toString() + " does not exist.");
        	}
        }
	}
	
	/**
	 * Execute the split command with current options.
	 */
	public void execute() throws Exception
	{
		List flist = Split.split(infile, outdir, nevents, maxevents);
		Split.printSplitSummary(flist, System.out);
	}
}
