package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOBlock;
import hep.lcd.io.sio.SIOReader;
import hep.lcd.io.sio.SIORecord;

import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.LCRunHeader;

import hep.lcio.io.LCEventListener;
import hep.lcio.io.LCReader;
import hep.lcio.io.LCRunListener;

import java.io.EOFException;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOLCReader.java,v 1.12 2004-07-14 15:50:45 gaede Exp $
 */
class SIOLCReader implements LCReader
{
   private List eventListeners = new ArrayList();
   private List runListeners = new ArrayList();
   private SIOReader reader;

   private String[] _filenames ;
   private int _currentIndex ;

   public void close() throws IOException
   {
      reader.close();
   }

   public void open(String filename) throws IOException
   {
      reader = new SIOReader(new FileInputStream(filename));
   }

    /** Opens a list of files for reading (read-only). All subsequent
     * read operations will operate on the list, i.e. if an EOF is encountered
     * the next file in the list will be opened and read transparently to the
     * user.
     *
     * @throws IOException
     */
    public void open(String[] filenames) throws IOException{
	_filenames = filenames ;
	_currentIndex=0 ;
	open( _filenames[ _currentIndex ] );
    }


   public LCEvent readEvent(int runNumber, int evtNumber) throws IOException
   {
      try
      {
         for (;;)
         {            
            SIORecord record = reader.readRecord();
            String name = record.getRecordName();
            if (SIOFactory.eventHeaderRecordName.equals(name)) continue;

            SIOEvent event = new SIOEvent(record,LCIO.READ_ONLY);
            if (event.getRunNumber() == runNumber && event.getEventNumber() == evtNumber)
            {
               event.readData(reader.readRecord());
               return event;
            }
            else reader.readRecord();
         }
      }
      catch (EOFException x)
      {
	if( _filenames != null  && ++_currentIndex < _filenames.length ){
	    close() ;
	    open( _filenames[ _currentIndex ] ) ;
	    return readEvent( runNumber, evtNumber) ;
	} 
	return null;
      }
   }

   public LCEvent readNextEvent(int accessMode) throws IOException
   {
      try
      {
         for (;;)
         {
            SIORecord record = reader.readRecord();
            String name = record.getRecordName();
            if (!name.equals(SIOFactory.eventHeaderRecordName))
               continue;

            SIOEvent event = new SIOEvent(record,accessMode);
            event.readData(reader.readRecord());
            return event;
         }
      }
      catch (EOFException x)
      {
	if( _filenames != null  && ++_currentIndex < _filenames.length ){
	    close() ;
	    open( _filenames[ _currentIndex ] ) ;
	    return readNextEvent( accessMode ) ;
	} 
         return null;
      }
   }

   public LCEvent readNextEvent() throws IOException
   {
      return readNextEvent(LCIO.READ_ONLY);
   }

   public LCRunHeader readNextRunHeader() throws IOException
    {
	return readNextRunHeader(LCIO.READ_ONLY);
    }

   public LCRunHeader readNextRunHeader(int accessMode) throws IOException
   {
      try
      {
         for (;;)
         {
            SIORecord record = reader.readRecord();
            String name = record.getRecordName();
            if (!name.equals(SIOFactory.runRecordName))
               continue;

            SIOBlock block = record.getBlock();
			int major = block.getMajorVersion() ;
			int minor = block.getMinorVersion() ;
			if (( major < 1) && ( minor < 8))
               throw new IOException("Sorry: files created with versions older than v00-08" + " are no longer supported !");

	    // FIX ME: need to set access mode here....
            return new SIORunHeader(block.getData(),major,minor);
         }
      }
      catch (EOFException x)
      {
	if( _filenames != null  && ++_currentIndex < _filenames.length ){
	    close() ;
	    open( _filenames[ _currentIndex ] ) ;
	    return readNextRunHeader( accessMode ) ;
	} 
         return null;
      }
   }

   public void readStream() throws IOException
   {
      readStream(-1);
   }

   public void readStream(int maxRecords) throws IOException
   {
      try
      {
         for (int nRead = 0; (maxRecords < 0) || (nRead < maxRecords);
               nRead++)
         {
            SIORecord record = reader.readRecord();
            if (record.getRecordName().equals(SIOFactory.runRecordName))
            {
               int l = runListeners.size();
               if (l > 0)
               {
                  SIOBlock block = record.getBlock();

     			  int major = block.getMajorVersion() ;
	    		  int minor = block.getMinorVersion() ;
		    	  if (( major < 1) && ( minor < 8))
                     throw new IOException("Sorry: files created with versions older than v00-08" + " are no longer supported !");

                  SIORunHeader header = new SIORunHeader(block.getData(),major,minor);
                  for (int i = 0; i < l; i++){
		      // FIX ME: need to set access mode here....
                     ((LCRunListener) runListeners.get(i)).processRunHeader(header);
                     ((LCRunListener) runListeners.get(i)).modifyRunHeader(header);
		          }
               }
            }
            else if (record.getRecordName().equals(SIOFactory.eventHeaderRecordName))
            {
               int l = eventListeners.size();
               if (l > 0)
               {
                  SIOEvent event = new SIOEvent(record,LCIO.READ_ONLY);
                  event.readData(reader.readRecord());
                  for (int i = 0; i < l; i++){ ((LCEventListener) eventListeners.get(i)).processEvent(event); }
                  event.setReadOnly(false);
                  for (int i = 0; i < l; i++){ ((LCEventListener) eventListeners.get(i)).modifyEvent(event); }
               }
            }
         }
      }
      catch (EOFException x)
      {
	if( _filenames != null  && ++_currentIndex < _filenames.length ){
	    close() ;
	    open( _filenames[ _currentIndex ] ) ;
	    readStream( maxRecords ) ;
	} 

	if (maxRecords < 0)
            return;
	else
            throw x;
      }
   }

   public void registerLCEventListener(LCEventListener ls)
   {
      eventListeners.add(ls);
   }

   public void registerLCRunListener(LCRunListener ls)
   {
      runListeners.add(ls);
   }

   public void removeLCEventListener(LCEventListener ls)
   {
      eventListeners.remove(ls);
   }

   public void removeLCRunListener(LCRunListener ls)
   {
      runListeners.remove(ls);
   }
}
