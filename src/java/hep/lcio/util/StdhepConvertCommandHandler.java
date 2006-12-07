package hep.lcio.util;

import java.io.File;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;

public class StdhepConvertCommandHandler extends CommandHandler
{
	File infile;
	File outfile;
	
	StdhepConvertCommandHandler()
	{
		super("stdhep","Convert Stdhep file to LCIO.");
		
		options = createStdhepConvertOptions();
	}
	
	private static Options createStdhepConvertOptions()
	{
		Options options = new Options();
		
		Option opt = new Option("h", false, "Print stdhep usage.");
		options.addOption(opt);
		
		opt = new Option("i", true, "Input Stdhep file.");
		opt.setArgs(1);
		options.addOption(opt);
		
		opt = new Option("o", true, "Output LCIO file.");
		opt.setArgs(1);
		options.addOption(opt);
		
		return options;
	}
	
	public void parse(String[] argv) throws Exception
	{
		CommandLine cl = parser.parse(options, argv);
		
		if ( cl.hasOption("h") )
		{
			printUsage(true);
		}
		
		if (cl.hasOption("i"))
		{
			infile = new File(cl.getOptionValue("i"));
		}
		else {
			printUsage(true);
		}
		
		String lcioname = 
			infile.getAbsolutePath().replace(".stdhep",".slcio");
	
		if (cl.hasOption("o"))
		{
			lcioname = cl.getOptionValue("o");
		}
		
		outfile = new File(lcioname);
	}

	public void execute() throws Exception
	{
		StdhepConverter converter = new StdhepConverter();
		converter.convert(infile, outfile);
	}
}