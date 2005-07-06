/*
 * LCIOSplit.java
 *
 * Created on July 6, 2005, 10:32 AM
 */

package hep.lcio.util;

import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCEventListener;
import hep.lcio.io.LCRunListener;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCRunHeader;
import java.io.PrintStream;
import java.io.File;
import java.io.IOException;

/**
 *
 * @author jeremym
 *
 * This is a simple utility class to split an LCIO file into separate files with N events each.
 * It does not do any modification of the run or event numbers but just writes them verbatim.
 *
 */
public class LCIOSplitter implements LCEventListener, LCRunListener
{
    String _infilePath;
    String _outfileBasename;
    String _currFilename;
    File _infile;
    int _nEventsSplit;
    int _currEventNum;
    int _currFileNum;
    int _totEvents;
    LCReader _reader;
    LCWriter _writer;
    boolean _firstTime = true;
    
    /** Creates a new instance of LCIOSplitter */
    public LCIOSplitter()
    {
        _reader = LCFactory.getInstance().createLCReader();
        _reader.registerLCEventListener(this);
        _reader.registerLCRunListener(this);
        
        _writer = LCFactory.getInstance().createLCWriter();
        _nEventsSplit = 500;
        _currEventNum = -1;
        _currFileNum = 0;
    }
    
    public static void main(String args[])
    {
        LCIOSplitter splitter = new LCIOSplitter();
        
        try
        {
            splitter.Main(args);
        }
        catch (Exception e)
        {
            throw new RuntimeException("Error running LCIOSplitter.", e);
        }
    }
    
    private void Main(String args[]) throws IOException
    {
        if ( args.length < 1 )
        {
            printUsage(System.out);
        }
        
        _infilePath = args[0];
        
        if ( args.length > 1 )
        {
            _nEventsSplit = Integer.parseInt(args[1]);
            
            if ( _nEventsSplit <= 0 )
            {
                throw new IllegalArgumentException("Number of events to split must be > 0.");
            }
        }
        
        System.out.println("infile: " + _infilePath);
        System.out.println("nEventsSplit: " + _nEventsSplit);
        
        openInfile(_infilePath);
        setOutfileBasename();
        setupNextOutfile();
        readEvents();
        closeWriter();
        renameLastFile();
        
        System.out.println("totEvents: " + _totEvents);
    }
    
    private void readEvents() throws IOException
    {
        _reader.readStream();
    }
    
    private void openInfile(String fpath) throws IOException
    {               
        _infile = new File(fpath);
        
        if ( !_infile.exists() )
        {
            throw new IOException("File does not exist: " + fpath);
        }
        
        if ( !_infile.canRead() )
        {
            throw new IOException("File is not readable: " + fpath);
        }
        
        _reader.open( _infile.getCanonicalPath() );
    }
    
    private String makeNextFilename(int fileNum, int evtNum)
    {
        return _outfileBasename + "-" + fileNum + "-" + evtNum;
    }
    
    private String makeNextFilename()
    {
        return makeNextFilename(_currFileNum, _nEventsSplit);
    }
    
    private void setOutfileBasename()
    {
        int extIdx = _infile.getName().indexOf(".slcio");
        
        if ( extIdx == -1 )
        {
            throw new IllegalArgumentException("No .slcio extension on input file: " + _infile);
        }
        
        _outfileBasename = _infile.getName().substring(0, extIdx);
    }
    
    private void setupNextOutfile() throws IOException
    {                
        if ( !_firstTime )
        {
            closeWriter();            
        }
        else
        {
            _firstTime = false;
        }        
        
        _currFilename = makeNextFilename();        
        _writer.open(_currFilename);                
        ++_currFileNum;
    }
    
    private void closeWriter()
    {
        try
        {
            _writer.flush();
            _writer.close();
            
            System.out.println("wrote file: " + _currFilename);
        }
        catch (IOException ioe)
        {}
    }
    
    private void renameLastFile()
    {                
        /* Rename last file if didn't get N events. */
        if ( !isNextOutfile() )
        {          
            File o = new File(_currFilename + ".slcio");            
            o.renameTo(new File(makeNextFilename(_currFileNum - 1, _currEventNum + 1) + ".slcio"));
        }
    }
    
    public void processEvent(LCEvent event)
    {}
    
    public void processRunHeader(LCRunHeader header)
    {
        try
        {
            _writer.writeRunHeader(header);
        }
        catch (IOException ioe)
        {
            throw new RuntimeException("Error writing next LCRunHeader.", ioe);
        }
    }
    
    public boolean isNextOutfile()
    {
        return _currEventNum > (_nEventsSplit - 1);
    }
    
    public void modifyEvent(LCEvent event)
    {
        ++_currEventNum;        
        
        if ( isNextOutfile() )
        {
            try
            {
                setupNextOutfile();
            }
            catch (IOException ioe)
            {
                throw new RuntimeException("Error setting up next output file.", ioe);
            }
            
            _currEventNum = 0;
        }
        
        try
        {            
            _writer.writeEvent(event);
            ++_totEvents;
        }
        catch (IOException ioe)
        {
            throw new RuntimeException("Error writing next LCEvent.", ioe);
        }
    }
    
    public void modifyRunHeader(LCRunHeader header)
    {}
    
    public static void printUsage(PrintStream ps)
    {
        ps.println("LCIOSplitter input_file [nevents]");
        System.exit(1);
    }
}
