package hep.lcio.util;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.Parser;
import org.apache.commons.cli.PosixParser;

/**
 * Command-line handling for concat command.
 * 
 * @author jeremym
 * @version $Id: ConcatenateCommandHandler.java,v 1.3 2006-12-07 00:47:32 jeremy Exp $
 */
public class ConcatenateCommandHandler extends CommandHandler
{
	List infiles = new ArrayList();
	File outfile;
	String fname = "concat_events.slcio";
	
	ConcatenateCommandHandler()
	{
		super("concat","Concatenate LCIO files together.");
		
		// Create concatenate options.
		options = createConcatenateOptions();
	}
	
	/**
	 * Create Options for the concat command.
	 * @return An apache CLI Options object with concat options.
	 */
	private static Options createConcatenateOptions()
	{
		Options options = new Options();
		
		Option opt = new Option("h", false, "Print concat usage.");
		options.addOption(opt);
		
		opt = new Option("f",false,"Add an input file.");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("o",false,"Set the name of the output file.");
		opt.setArgs(1);
		options.addOption(opt);
		
		opt = new Option("i", false,"List of input files, 1 per line.");
		opt.setArgs(1);
		options.addOption(opt);
		
		return options;
	}
	
	/**
	 * Execute the concat command with current options.
	 */
	public void execute() throws Exception
	{
		Concatenate.concatenate(infiles, outfile);
		System.out.println("Concatenated " + infiles.size() + " files to " + outfile.getAbsolutePath());
	}

	/**
	 * Parse the arguments for concat options.
	 * @param argv The raw command-line options.
	 */
	public void parse(String[] argv) throws Exception
	{
		CommandLine cl = parser.parse(options, argv);
		
		if ( cl.hasOption("h") )
		{
			printUsage(true);
		}		
						
		// Add individual files.
		if (cl.hasOption("f"))
		{
			String[] infilepaths = cl.getOptionValues("f");
			for (int i=0; i<infilepaths.length; i++)
			{
				infiles.add(new File(infilepaths[i]));
			}
		}
		
		// Add list of files.
		if (cl.hasOption("i"))
		{
			String[] infilelists = cl.getOptionValues("i");
			for (int i=0; i<infilelists.length; i++)
			{
				List lines = FileUtil.loadFile(infilelists[i]);
				for (Iterator iter = lines.iterator(); iter.hasNext();)
				{
					String line = (String)iter.next();
					infiles.add(new File(line));
				}
			}
		}
		
		// Interpret extra arguments as input files.
		if (cl.getArgList() != null)
		{
			for (Object o : cl.getArgList())
			{
				String s = (String)o;
				infiles.add(new File(s));
			}			
		}
		
		if ( infiles.size() == 0 )
		{
			printUsage(true);
		}
				
		// Set output file name.
		if (cl.hasOption("o"))
		{
			fname = cl.getOptionValue("o");
		}
		
		// Create new File for output.
		outfile = new File(fname);
	}
}