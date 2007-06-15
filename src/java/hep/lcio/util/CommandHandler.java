package hep.lcio.util;

import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.Parser;
import org.apache.commons.cli.PosixParser;

/**
 * 
 * A CommandHandler handles the command-line options for a
 * single command in the CommandLineTool.
 * 
 * @author jeremym
 * @version $Id: CommandHandler.java,v 1.6 2007-06-15 23:16:38 jeremy Exp $
 */

public abstract class CommandHandler 
{
	String name;
	String description;
	Options options = new Options();
	protected static Parser parser = new PosixParser();
	
	/**
	 * CommandHandler ctor.
	 * @param name The string that invokes this command.
	 * @param description The description of this command.
	 */
	public CommandHandler(String name, String description)
	{
		this.name = name;
		this.description = description;
	}
	
	/**
	 * Ctor with default description.
	 * @param name The string that invokes this command.	
	 */
	public CommandHandler(String name)
	{
		this.name = name;
		this.description = "NONE";
	}
	
	/**
	 * Get the name of this command.
	 * @return A String of the name of this command.
	 */
	public String getName()
	{
		return this.name;
	}
	
	/**
	 * Get the description of this command.
	 * @return A String of the description of this command.
	 */
	public String getDescription()
	{
		return this.description;
	}
	
	/**
	 * Options for this command.
	 * @return Options object for this command.
	 */
	public Options getOptions()
	{
		return options;
	}
	
	/**
	 * Print usage and (optionally) exit the program.
	 * @param doExit Whether or not to exit after printing usage.
	 */
	public void printUsage(boolean doExit)
	{
		HelpFormatter help = new HelpFormatter();
       
		help.printHelp(getName(), getOptions());
		
		if (doExit)
		{
			System.exit(0);
		}
	}
	
	/** 
	 * Parse this command's options from the command line. 
	 * @param argv Raw options array following after the command string. 
	 */
    public abstract void parse(String[] argv) throws Exception;
    
    /** 
     * Execute the command from parsed args.
     * @param Execute the command with arguments built parse. 
     */
    public abstract void execute() throws Exception;
}
