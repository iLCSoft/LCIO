package hep.lcio.util; 

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.Parser;
import org.apache.commons.cli.PosixParser;
//import org.apache.commons.cli.Parser;
//import org.apache.commons.cli.PosixParser;

/**
 * CommandLineTool provides a command-line interface
 * to LCIO utilities, similar to how the cvs command 
 * works.
 * 
 * lcio [global_options] [command] [command_options]
 * 
 * @see hep.lcio.util.Compare compare
 * @see hep.lcio.util.Concat concat
 * @see hep.lcio.util.Headers head
 * @see hep.lcio.util.MergeCommandHandler merge
 * @see hep.lcio.util.PrintEvent print
 * @see hep.lcio.util.SioDump siodump
 * @see hep.lcio.util.Split split
 * 
 * @author jeremym
 * @version $Id: CommandLineTool.java,v 1.2 2006-04-26 00:56:53 jeremy Exp $
 */
public class CommandLineTool
{
	private Map handlers = new HashMap();
	private String command;
	private Parser parser = new PosixParser();
	private Options options = new Options();
	private CommandLine cl;
	private static CommandLineTool instance = new CommandLineTool();
	
	public static CommandLineTool instance()
	{
		return instance;
	}
	
	private CommandLineTool()
	{
		//System.out.println("CommandLineTool");

		registerOptions();
		registerHandlers();
	}	
	
	public CommandLine getGlobalOptions()
	{
		return cl;
	}
	
	// main entry point.
	public static void main(String[] args) throws Exception
	{
		//System.err.println("main");

		CommandLineTool cl = CommandLineTool.instance();

		// try {
		cl.parse(args);
		// }
		// catch (Exception e)
		// {
		// throw new RuntimeException("Problem parsing args.",e);
		// }
	}

	// Register the default CommandHandlers.
	private void registerHandlers()
	{
		// addCommandHandler("compare", Compare)
		// addCommandHandler("concat", Concat);
		// addCommandHandler("printevent", PrintEvent);
		// addCommandHandler("header", HeaderScan);
		addCommandHandler(new MergeCommandHandler());
		// addCommandHandler("siodump", SioDump);
		// addCommandHandler("split", Split);
	}

	// Register global options. Sub-commands have their own option set.
	private void registerOptions()
	{
//		Option opt = new Option("h", false, "print lcio command-line tool usage");
//		options.addOption(opt);

//		opt = new Option("v", false, "set the verbosity");
//		opt.setArgs(1);
//		options.addOption(opt);
	}

	// Set the name of the active sub-command.
	public void setCommand(String command)
	{
		this.command = command;
	}

	// Get the name of the active sub-command.
	public String getCommand()
	{
		return this.command;
	}

	// Parse the command line for global options.  
	// Then find and execute the appropriate CommandHandler.
	public void parse(String[] args) throws Exception
	{
		int icmd = scanForCommand(args);

		if (icmd == -1)
		{
			printUsage(true);
		}

		// Set the command to execute.
		setCommand(args[icmd]);

		System.out.println("command: " + getCommand());

		// Get a command handler for this command string.
		CommandHandler handler = getCommandHandler(getCommand());

		// Global arguments.
		int nglob = icmd - 1;
		String[] globargv = new String[0];
		if (nglob > 0)
		{
			globargv = new String[nglob];
			System.arraycopy(args, 0, globargv, 0, nglob);
			for (int i = 0; i < globargv.length; i++)
			{
				System.out.println("globargv[" + i + "]=" + globargv[i]);
			}
		}

		// Arguments are passed verbatim to the subcommand.
		int ncmd = args.length - (icmd + 1);
		String[] cmdargv = new String[ncmd];
		System.arraycopy(args, icmd + 1, cmdargv, 0, ncmd);
		for (int i = 0; i < cmdargv.length; i++)
		{
			System.out.println("cmdargv[" + i + "]=" + cmdargv[i]);
		}

		// Parse global options.
		cl = parser.parse(options, globargv);

		// Pass the subcommand the command line options
		// with globals stripped out.
		handler.parse(cmdargv);

		// Execute the CommandHandler.
		handler.execute();
	}

	// Add a class to handle a command string.
	public void addCommandHandler(CommandHandler handler)
	{
		handlers.put(handler.getName(), handler);
	}

	// Get a CommandHandler from the command string.
	public CommandHandler getCommandHandler(String command) throws Exception
	{
		return (CommandHandler) handlers.get(command);
	}

	// Scan the argv for an input command string, e.g. "merge", etc.
	private int scanForCommand(String[] argv)
	{
		for (int i = 0; i < argv.length; i++)
		{
			if (handlers.get(argv[i]) != null)
			{
				//System.err.println("found " + argv[i] + " @ idx " + i);
				return i;
			}
		}
		return -1;
	}

	// Print usage using CLI.
	public void printUsage(boolean doExit)
	{
		HelpFormatter help = new HelpFormatter();

		StringBuffer s = new StringBuffer();

		for (Iterator it = handlers.keySet().iterator(); it.hasNext();)
		{
			s.append((String) it.next() + '\n');
		}

		help.printHelp("LcioCommandLineTool", "Commands:\n" + s.toString(),
				options, "");

		if (doExit)
		{
			System.exit(0);
		}
	}
}