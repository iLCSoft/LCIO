package hep.lcio.util;

import hep.io.sio.SIOReader;
import hep.io.sio.SIORecord;
import hep.lcio.event.LCIO;

import java.io.BufferedInputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.util.Iterator;
import java.util.List;

/**
 * Utility method for counting the number of runs and events in an LCIO file.
 * 
 * @author jeremym
 * @version $Id: HeaderCount.java,v 1.3 2007-11-07 20:46:24 jeremy Exp $
 */
public class HeaderCount
{
	/**
	 * Count number of events and runs.
	 * 
	 * @param pstr The PrintStream where output will go.
	 * @param files A list of LCIO input files.
	 * @param printEvent Print number of events.
	 * @param printRun Print number of runs.
	 * @param printTot Print totals.
	 */
	public static void countHeaders(PrintStream pstr, List files, boolean printEvent, boolean printRun, boolean printTot)
	{		
		int evttot = 0;
		int runtot = 0;
		StringBuffer strbuff = new StringBuffer();
		for (Iterator iter = files.iterator(); iter.hasNext(); ) 
		{
			int evt = 0;
			int run = 0;
			
			File file = (File)iter.next();

			strbuff.append(file.getAbsolutePath());
			
			SIOReader reader;
			try {
				reader = new SIOReader(new BufferedInputStream(new FileInputStream(file.getAbsolutePath())));
			}
			catch (Exception e)
			{
				throw new RuntimeException(e);
			}
			
			for (;;)
			{
				SIORecord record;
				
				try {
					record = reader.readRecord();
				}
				catch (EOFException x)
				{
					break;
				}
				catch (IOException x)
				{
					throw new RuntimeException(x);
				}
				
				String name;
				try {
					name = record.getRecordName();
				}
				catch (IOException x)
				{
					throw new RuntimeException(x);
				}
				
				if (name.compareTo(LCIO.LCEVENT) == 0)
				{
					evt++;
				}
				else if (name.compareTo(LCIO.LCRUNHEADER) == 0)
				{
					run++;
				}				
			}
			
			if (printRun)
			{
				strbuff.append('\t');
				strbuff.append(run);
			}
			
			if (printEvent)
			{
				strbuff.append('\t');
				strbuff.append(evt);
			}
			
			strbuff.append('\n');			
			
			evttot += evt;
			runtot += run;
			
			try {
				reader.close();
			}
			catch (IOException x)
			{}
			
			reader = null;
		}
		
		if (printTot)
		{
			strbuff.append("TOTAL");
			strbuff.append('\t');
			strbuff.append(runtot);
			strbuff.append('\t');
			strbuff.append(evttot);
			strbuff.append('\n');
		}
		
		pstr.print(strbuff.toString());
	}
}