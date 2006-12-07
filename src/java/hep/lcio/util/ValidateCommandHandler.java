package hep.lcio.util;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;

/**
 * Command-line handler for the validate utility.
 * 
 * @author Jeremy McCormick
 * @version $Id: ValidateCommandHandler.java,v 1.2 2006-12-07 00:47:33 jeremy Exp $
 */
public class ValidateCommandHandler extends CommandHandler
{
	List infiles = new ArrayList();
	int ntoread = 1;
	int major = -1;
	int minor = -1;
	
	public ValidateCommandHandler()
	{
		super("validate","Perform basic validation on an LCIO file.");
		options = createValidateOptions();
	}
	
	/**
	 * Creates commandline options for the validate command.
	 * @return Options object for validate command.
	 */
	public static Options createValidateOptions()
	{
		Options options = new Options();
		
		Option opt = new Option("h", false, "Print validate usage.");
		options.addOption(opt);
				
		opt = new Option("f", true, "Add an LCIO file to validate.");
		opt.setArgs(1);
		options.addOption(opt);
		
		opt = new Option("M", true, "Set major version.");
		opt.setArgs(1);
		options.addOption(opt);
		
		opt = new Option("m", true, "Set minor version.");
		opt.setArgs(1);
		options.addOption(opt);
		
		opt = new Option("n", true, "Set number of events to read.");
		opt.setArgs(1);
		options.addOption(opt);
		
		return options;
	}
	
	/**
	 * Parse the commandline options for the validate command.
	 */
	public void parse(String[] argv) throws Exception
	{
		CommandLine cl = parser.parse(options, argv);
		
		if ( cl.hasOption("h") )
		{
			printUsage(true);
		}
				
		if (cl.hasOption("f"))
		{
			String files[] = cl.getOptionValues("f");
			for (int i=0; i<files.length; i++)
			{
				infiles.add(new File(files[i]));
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
		
		if ( infiles.size() == 0 ) {
			printUsage(true);
		}
		
		if (cl.hasOption("M"))
		{
			major = Integer.parseInt(cl.getOptionValue("M"));
		}
		
		if (cl.hasOption("m"))
		{
			minor = Integer.parseInt(cl.getOptionValue("m"));
		}
		
		if (cl.hasOption("n"))
		{
			ntoread = Integer.parseInt(cl.getOptionValue("n"));
		}
	}

	/**
	 * Execute the validate command with current options.
	 */
	public void execute() throws Exception
	{
		for (int i=0; i<infiles.size(); i++)
		{
			File f = (File)infiles.get(i);
			
			Validate.testRead(f, ntoread);			
			Validate.validateVersion(f, major, minor);
		}
	}
}