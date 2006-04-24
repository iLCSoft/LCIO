package hep.lcio.util;

import java.io.File;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.Parser;
import org.apache.commons.cli.PosixParser;

/**
 * This is the CommandHandler for the merge utility.
 * It handles command-line options and passes them
 * to methods in MergeUtil.
 * 
 * @author jeremym
 * @version $Id: MergeCommandHandler.java,v 1.1 2006-04-24 22:08:34 jeremy Exp $
 */
public class MergeCommandHandler extends CommandHandler
{
	Parser parser = new PosixParser();
	Options options = new Options();
	File outfile;
	File[] infiles;
	float dt = 0;
	boolean breakOnSingleEOF = false;
	int maxevents = Integer.MAX_VALUE;
	int ntoread = 1;
	boolean incrTime = false;

	/** 
	 * MergeCommandHandler ctor. 
	 */
	MergeCommandHandler()
	{
		// Call CommandHandler ctor.
		super("merge", "Merge LCIO events together.");

		// Setup options for the merge command.
		options = createMergeOptions();
	}

	/**
	 * Creates the CLI options for the merge command.
	 * @return Options for the merge command.
	 */
	private Options createMergeOptions()
	{
		Options options = new Options();

		Option opt = new Option("o", false, "set the output file");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("i", false, "add an input file");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("t", true, "set delta time (NS)");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("d", false, "increment t after each overlay event");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("n", true, "set maximum number of events");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("c", true, "continue when one of the input files runs out of events (default is stop processing)");
		options.addOption(opt);

		opt = new Option("e", true, "set number of events at a time to merge in from each input file (default is 1)");

		return options;
	}

	/** 
	 * Parse the command line options for the merge command using apache CLI.
	 * @param argv The input arguments for the merge command. 
	 */
	public void parse(String[] argv) throws Exception
	{
		CommandLine cl = parser.parse(options, argv);

		// Make an input file list.
		if (cl.hasOption("i"))
		{	
			infiles = MergeUtil.createFiles(cl.getOptionValues("i"));
		}

		// DEBUG
		System.err.println("outfile=" + cl.getOptionValue("o"));

		// Output file.
		outfile = new File(cl.getOptionValue("o"));

		// The time delta.
		if (cl.hasOption("t"))
		{
			dt = Float.parseFloat(cl.getOptionValue("t"));
		}

		// Set whether to break on a single EOF from input files.
		if (cl.hasOption("c"))
		{
			breakOnSingleEOF = false;
		}

		// Set max events.
		if (cl.hasOption("n"))
		{
			maxevents = Integer.parseInt(cl.getOptionValue("n"));
		}

		// Set whether to increment the time delta after each event overlay.
		if (cl.hasOption("d"))
		{
			incrTime = true;
		}
	}

	/** 
	 * Execute the merge command with current arguments. 
	 */
	public void execute() throws Exception
	{
		MergeUtil.mergeFiles(outfile, infiles, ntoread, maxevents, dt, incrTime);
	}
}