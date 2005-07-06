/*
 * Class to concatenate multiple LCIO files into single file.
 *
 * Includes options to ignore run headers or renumber them sequentially.
 *
*/

package hep.lcio.util;

import java.io.*;
import java.util.ArrayList;

import hep.lcio.event.*;
import hep.lcio.io.*;
import hep.lcio.implementation.io.*;
import hep.lcio.implementation.event.*;

public class LCIOConcat implements LCEventListener, LCRunListener
{
    private LCWriter writer;
    private LCReader reader;    

    private int nevents;
    private int nheaders;
    private int nfiles;

    private int evtnum;
    private int runnum;

    private int noptions;
    private boolean ignoreRunHeaders;
    private boolean renumberRunHeaders;
    private boolean printUsage;
    
    private boolean firstRunHeader;    

    private String outfilePath;
    private ArrayList infilePaths;
    private int ninfiles;

    public LCIOConcat()
    {
	reader = LCFactory.getInstance().createLCReader();
	reader.registerLCEventListener(this);
	reader.registerLCRunListener(this);

	writer = LCFactory.getInstance().createLCWriter();

	infilePaths = new ArrayList();

	noptions = 0;
	ignoreRunHeaders = false;
	renumberRunHeaders = false;
	firstRunHeader = true;
	printUsage = false;

	nevents = nheaders = nfiles = 0;	

	evtnum = runnum = 0;

	ninfiles = 0;
    }

    public static void main(String[] args) throws Exception
    {
	LCIOConcat concat = new LCIOConcat();
	concat.Main(args);
    }

    public void Main(String[] args)
    {
	try {
	    // parse CL args
	    parseArgs(args);

	    // open the writer
	    openWriter(outfilePath);
	    
	    // concat input files
	    concatFiles();

	    // close writer
	    closeWriter();

	    // print stats
	    printStats();
	}
	catch (Exception e) {
	    System.out.println(e.getMessage() );
	    RuntimeException rte = new RuntimeException("Exception in main().");
	    rte.initCause(e);
	    throw rte;
	}
    }

    public void concatFiles() throws java.io.IOException
    {
	// read and concat input files
	for (int i = 0; i < (infilePaths.size()); i++) {
	    System.out.println("reading input file " + infilePaths.get(i) );
	    openFile((String)infilePaths.get(i));
	    readCurrentFile();
	    reader.close();
	}
    }

    public void parseArgs(String[] args) throws java.lang.IllegalArgumentException
    {       
	for (int i = 0; i < args.length; i++) {
	    String s = args[i];
	    // options starting with "--"
	    if ( s.startsWith("--") ) {
	        ++noptions;
		if ( s.equals("--ignore-run-headers") ) {
		    ignoreRunHeaders = true;
		}
		else if ( s.equals("--renumber-run-headers" )) {
		    renumberRunHeaders = true;
		}
		else if ( s.equals("--help" )) {
		    printUsage = true;
		}	    
		else {
		    throw new IllegalArgumentException("Unknown option: " + s);
		}
	    }
	    // files
	    else {
		// output file is last arg
		if (i == ( args.length - 1 ) ) {
		    outfilePath = s;
		}
		// must be an infile
		else {
		    infilePaths.add(s);
		    ++ninfiles;
		}
	    }
	}

	// must have at least one input and an output file OR
	// usage was requested
	if ( ( ninfiles + 1 < 2 ) || printUsage ) {
	    usage();
	}	
	
	System.out.println("noptions=" + noptions);
	System.out.println("ninfiles=" + ninfiles);	   	
	System.out.println("concatenate to file " + outfilePath);
    }

    public void openWriter(String fpath) throws java.io.IOException
    {
	if (fileExists(fpath)) {
	    throw new java.io.IOException("Refusing to clobber existing file.");
	}

	writer.open(fpath);
    }
 
    public void closeWriter() throws java.io.IOException
    {
	writer.close();
    }

    public void openFile(String fpath) throws java.io.IOException
    {
	if (!fileExists(fpath)) {
	    throw new java.io.IOException("Input LCIO file does not exist.");
	}

	reader.open(fpath);
	++nfiles;
    }

    public void readCurrentFile() throws java.io.IOException
    {
	reader.readStream();
    }

    public void processEvent(LCEvent event)
    {}

    public void processRunHeader(LCRunHeader header) 
    {}

    public void modifyEvent(LCEvent event)
    {
	try {
	    ILCEvent ievt = (ILCEvent)event;

	    // renumber events if using one run header
	    if (ignoreRunHeaders) {
		ievt.setEventNumber(nevents);
	    }

	    writer.writeEvent(ievt);
	}
	catch (java.io.IOException e) {
	    RuntimeException rte = new RuntimeException("IOException in processEvent().");
	    rte.initCause(e);
	    throw rte;
	}
	++nevents;
    }

    public void modifyRunHeader(LCRunHeader header)
    {
    	try {
	    // flag to only write first header
	    if ( ignoreRunHeaders ) {
		// write the first header
		if ( firstRunHeader ) {
		    System.out.println("firstHeader");
		    ILCRunHeader firstHeader = (ILCRunHeader)header;
		    firstHeader.setRunNumber(0);
		    writer.writeRunHeader(firstHeader);
		    firstRunHeader = false;
		}
		// ignore additional ones
		else {
		    System.out.println("ignored additional header");
		}
	    }
	    // writer header like usual
	    else {

		// renumber run headers sequentially
		ILCRunHeader ihdr = (ILCRunHeader)header;
		if (renumberRunHeaders) {
		    System.out.println("renumbering header to " + nheaders);		    
		    ihdr.setRunNumber(nheaders);
		}				
		writer.writeRunHeader(ihdr);
	    }
	}
	catch (java.io.IOException e) {
	    RuntimeException rte = new RuntimeException("IOException in processRunHeader().");
	    rte.initCause(e);
	    throw rte;
	}

	++nheaders;
    }

    public static void usage()
    {
	System.out.println("java " + LCIOConcat.class.getName() + " [options] infile1 infile2 [...] outfile");
	System.out.println("[options] == --ignore-run-headers, --renumber-run-headers, --help");
	System.exit(1);
    }

    public void printStats()
    {
	System.out.println("num files read   = " + nfiles);
	System.out.println("num headers read = " + nheaders);
	System.out.println("num events read  = " + nevents);
    }

    public boolean fileExists(String fpath)
    {
	boolean e = false;
	File f = new File(fpath);
	if ( f.exists() ) {
	    e = true;
	}
	return e;
    }
}
