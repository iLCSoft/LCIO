package hep.lcio.util;

import java.io.File;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;

public class PrintCommandHandler extends CommandHandler {
	
	File f;
	int nevents = -1;
	int nskip = 0;
	int maxrec = 1000;
	boolean printHeadersOnly = false;

	public PrintCommandHandler() {	
		super("print", "Format and print contents of an LCIO file.");

		options = createPrintOptions();
	}

	private static Options createPrintOptions() {		
		Options options = new Options();

		Option opt = new Option("h", false, "Print print command usage.");
		options.addOption(opt);

		opt = new Option("f", true, "Set the LCIO file to dump.");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("n", true, "Set the maximum number of events to dump.");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("s", true, "Set the number of events to skip.");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("m", true,
				"Set the maximum number of records to print per collection.");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("H", false,
				"Print only the event headers and not the collections.");
		options.addOption(opt);

		return options;
	}

	public void parse(String[] argv) throws Exception {
		CommandLine cl = parser.parse(options, argv);

		if (cl.hasOption("h")) {
			printUsage(true);
		}

		if (cl.hasOption("f")) {
			String fname = cl.getOptionValue("f");
			f = new File(fname);
		} else if (cl.getArgList().size() > 0) {
			String fname = cl.getArgs()[0];
			f = new File(fname);
		} else {
			printUsage(true);
		}

		if (cl.hasOption("n")) {
			nevents = Integer.parseInt(cl.getOptionValue("n"));
		}

		if (cl.hasOption("s")) {
			nskip = Integer.parseInt(cl.getOptionValue("s"));
		}

		if (cl.hasOption("m")) {
			maxrec = Integer.parseInt(cl.getOptionValue("m"));
		}

		if (cl.hasOption("H")) {
			printHeadersOnly = true;
		}
	}

	public void execute() throws Exception {
		Printer p = new Printer();
		p.print(f, nevents, nskip, maxrec, printHeadersOnly);
	}
}
