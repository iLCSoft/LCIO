package hep.lcio.util;

/**
 * 
 * A CommandHandler handles the command-line options for a
 * single command in the CommandLineTool.
 * 
 * @author jeremym
 * @version $Id: CommandHandler.java,v 1.1 2006-04-24 22:08:34 jeremy Exp $
 */

public abstract class CommandHandler 
{
	String name;
	String description;
	
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
