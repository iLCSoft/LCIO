package hep.lcio.util;

import hep.io.sio.SIOReader;
import hep.io.sio.SIORecord;
import hep.lcio.event.LCIO;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;

/**
 * Command-line handling for Tony's SIODump utility.
 * 
 * @author jeremym
 * @version $Id: SIODumpCommandHandler.java,v 1.3 2007-11-07 20:46:24 jeremy Exp $
 */
public class SIODumpCommandHandler extends CommandHandler
{
	File inputfile;
	int verbosity = 4;
	int ntoskip = 0;
	int ntodump = 1;

	public SIODumpCommandHandler()
	{
		super("siodump", "Dump low-level SIO information from an LCIO file.");

		options = createSIODumpOptions();
	}

	/**
	 * Create options for the siodump command.
	 * @return An apache CLI Options object with siodump options.
	 */
	private static Options createSIODumpOptions()
	{
		Options options = new Options();

		Option opt = new Option("h", false, "Print siodump usage.");
		options.addOption(opt);
		
		opt = new Option("f", false, "Set the input LCIO file.");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("s", true, "Set number of events to skip.  (Must be >= 0) .");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("v", true, "Set verbosity level.  (Must be between 1 and 4) .");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("n", true, "Set number of events to dump.  (Must be > 0) .");
		opt.setArgs(1);
		options.addOption(opt);

		return options;
	}

	/**
	 * Execute the siodump command with current options.
	 */
	public void execute() throws Exception
	{
		// Create the SIO reader.
		SIOReader reader = new SIOReader(new BufferedInputStream(new FileInputStream(inputfile)));

		// Skip some records if selected.
		if (ntoskip > 0)
		{
			int nread = 0;

			for (;;)
			{
				SIORecord rec = reader.readRecord();

				String recname = rec.getRecordName();
				if (recname.compareTo(LCIO.LCEVENT) == 0)
				{
					// Increment number of events.
					++nread;
				}

				if (nread >= ntoskip)
					break;
			}
		}

		// Create the dumper.
		SIODump dumper = new SIODump();
		
		// Get the root of lcio.xml.
		Element lcioxml = getLCIOXML();
		
		// Dump the file.
		dumper.dump(lcioxml, reader);
	}

	/**
	 * Parse the arguments for siodump options.
	 * @param argv The raw command-line options.
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
			inputfile = new File(cl.getOptionValue("f"));
		}		
		else
		{
			printUsage(true);
		}

		if (cl.hasOption("n"))
		{
			ntodump = Integer.parseInt(cl.getOptionValue("n"));
			
			if (ntodump > 0) {
				SIODump.setMaxEvents(ntodump);
			}
			else {
				printUsage(true);
			}
		}

		if (cl.hasOption("s"))
		{
			ntoskip = Integer.parseInt(cl.getOptionValue("s"));
			
			if (ntoskip < 0) {
				printUsage(true);
			}
		}

		if (cl.hasOption("v"))
		{
			verbosity = Integer.parseInt(cl.getOptionValue("v"));
			SIODump.setVerbosity(verbosity);
			
			if (verbosity < 0 || verbosity > 4) {
				printUsage(true);
			}
		}
	}

	/**
	 * Get an @org.jdom.Element for the lcio.xml root.
	 * @return The Element representing the lcio.xml root node.
	 * @throws Exception
	 */
	public static Element getLCIOXML() throws Exception
	{
		InputStream in = SIODumpCommandHandler.class.getResourceAsStream("/doc/lcio.xml");
		SAXBuilder builder = new SAXBuilder();
		Document doc = builder.build(in);
		return doc.getRootElement();
	}
}