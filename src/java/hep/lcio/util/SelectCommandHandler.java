package hep.lcio.util;

import hep.lcio.event.LCCollection;
import hep.lcio.implementation.event.ILCEvent;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;

/**
 * {@link CommandHandler} for the <i>select</i> command.
 * 
 * @author jeremym
 * @version $Id: SelectCommandHandler.java,v 1.2 2007-06-15 23:37:29 jeremy Exp $
 */
public class SelectCommandHandler extends CommandHandler
{       
    List<String> includePatterns=new ArrayList<String>();
    List<String> includeTypes=new ArrayList<String>();
    boolean hasIncludePatterns=false;
    boolean hasIncludeTypes=false;
    File inputFile;
    File outputFile;
    
    public SelectCommandHandler()
    {
        super("select","Select collections from an event and write to new file.");
    
        options = createSelectOptions();
    }

    public void execute() throws Exception
    {
        // Create the reader.
        LCReader reader = LCFactory.getInstance().createLCReader();
        reader.open(inputFile.getCanonicalPath());
        
        // Create the writer for the merged events.
        LCWriter writer = LCFactory.getInstance().createLCWriter();
        writer.open(outputFile.getCanonicalPath());
                        
        while (true)
        {
            ILCEvent inputEvent = (ILCEvent)reader.readNextEvent();

            if (inputEvent == null)
            {
                break;
            }
            
            ILCEvent outputEvent = copyEventMeta(inputEvent);
            
            String[] collectionNames = inputEvent.getCollectionNames();
            for (String collectionName : collectionNames)
            {
                // Include the collection by default.
                boolean include=true;

                // Check if this collection matches a pattern from the -i switch.
                if (hasIncludePatterns)
                {
                    include=matchPatterns(collectionName,includePatterns);                
                }
                
                // Check to include by type.
                if (include && hasIncludeTypes)
                {
                    include=matchType(inputEvent.getCollection(collectionName));
                }
                
                // Write out the collection if it was matched.
                if (include)
                {
                    outputEvent.addCollection(inputEvent.getCollection(collectionName), collectionName);
                }
            }
                                               
            // Write out the event.
            writer.writeEvent(outputEvent);
        }
        
        writer.flush();
        writer.close();
    }
    
    private boolean matchPatterns(String collectionName, List<String> patterns)
    {
        boolean match=false;
        for (String pattern : patterns)
        {            
            if (collectionName.matches(pattern))
            {
                match=true;
                break;
            }            
        }
        return match;
    }
        
    private boolean matchType(LCCollection collection)
    {
        boolean match=false;
        for (String type : includeTypes)
        {                   
            if (collection.getTypeName().equalsIgnoreCase(type))
            {
                match=true;
                break;
            }
        }
        return match;
    }
    
    public void parse( String[] argv) throws Exception
    {
        CommandLine cl = parser.parse(options, argv);
        
        if ( cl.hasOption("h") )
        {
            printUsage(true);
        }        
        
        if (cl.hasOption("f"))
        {
            inputFile = new File(cl.getOptionValue("f"));
            if (!inputFile.exists())
            {
                System.err.println("Input file does not exist: " + inputFile.getCanonicalPath());
                printUsage(true);
            }
        }
        else 
        {
            System.err.println("Missing mandatory -f argument.");
            printUsage(true);
        }
        
        if (cl.hasOption("o"))
        {
            outputFile = new File(cl.getOptionValue("o"));
        }
        else
        {
            System.err.println("Missing mandatory -o argument.");
            printUsage(true);
        }
        
        if (cl.hasOption("i"))
        {
            for (String includePattern : cl.getOptionValues("i") )
            {
                includePatterns.add(includePattern);               
            }
            hasIncludePatterns=true;
        }
        
        if (cl.hasOption("t"))
        {
            for (String includePattern : cl.getOptionValues("t") )
            {
                includeTypes.add(includePattern);
            }
            hasIncludeTypes=true;
        }        
    }

    /**
     * Creates the CLI options for the compare command.
     * @return Options for the compare command.
     */
    private static Options createSelectOptions()
    {
        Options options = new Options();
        
        Option opt = new Option("h", false, "Print select usage.");
        options.addOption(opt);

        opt = new Option("f", true, "Set the input LCIO file.");
        opt.setArgs(1);
        options.addOption(opt);
        
        opt = new Option("o", true, "Set the output LCIO file.");
        opt.setArgs(1);
        options.addOption(opt);

        opt = new Option("i", false, "Add a pattern to include.");
        opt.setArgs(1);
        options.addOption(opt);
        
        opt = new Option("t", false, "Add an LCIO type to include.");
        opt.setArgs(1);
        options.addOption(opt);

        return options;
    }
    
    /**
     * Copy an event and its meta-data, i.e. detector name, event number,
     * and run number.  Does not copy any collections.
     * 
     * @param inputEvent
     * @return
     */
    private ILCEvent copyEventMeta(ILCEvent inputEvent)
    {
        ILCEvent outputEvent = new ILCEvent();
        
        outputEvent.setDetectorName(inputEvent.getDetectorName());
        outputEvent.setEventNumber(inputEvent.getEventNumber());
        outputEvent.setRunNumber(inputEvent.getRunNumber());
        outputEvent.setTimeStamp(inputEvent.getTimeStamp());
        
        return outputEvent;
    }        
}