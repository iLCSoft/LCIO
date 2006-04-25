package hep.lcio.util;

import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.SimCalorimeterHit;
import hep.lcio.implementation.event.ILCCollection;
import hep.lcio.implementation.event.ILCEvent;
import hep.lcio.implementation.event.IMCParticle;
import hep.lcio.implementation.event.ISimCalorimeterHit;
import hep.lcio.implementation.event.ISimTrackerHit;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.implementation.sio.SIOEvent;
import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;

import java.io.File;
import java.io.IOException;

/**
 * Utility methods for merging LCIO files, events, and collections,
 * with possible application of time delta.
 * 
 * @author jeremym
 * @version $Id: MergeUtil.java,v 1.2 2006-04-25 21:58:16 jeremy Exp $
 */
abstract public class MergeUtil
{	
	/** 
	 * Merge nEventsToRead events from each File in infiles into a single event in outfile,
	 * until records in infiles are exhausted or maxevents are created. 
	 * @return The number of combined events created.
	 * @param outfile Output target file.
	 * @param infiles Input events to merge in.
	 * @param nEventsToRead Number of events to read at once into one output event.
	 * @param maxEventsToWrite Maximum number of output events to create.
	 * @param dt The time delta.
	 */
	public static int mergeFiles( 
			File outfile, 
			File[] infiles, 
			int nEventsToRead, 
			int maxEventsToWrite, 
			float dt,
			boolean incrTime) throws IOException
	{
		// Create the writer.
		LCWriter writer = LCFactory.getInstance().createLCWriter();
		
		// Open the writer for the new file containing the merged events.
		writer.open(outfile.getCanonicalPath(), LCIO.WRITE_NEW);

		// Create the array of LCReaders.
		LCReader[] readers = createReaders(infiles);

		// Count of total output events.
		int nevents = 0;

		// File read loop.
		for (;;)
		{
			System.err.println("nevents: " + nevents);

			// Check if max output events is reached.
			if (nevents >= maxEventsToWrite)
				break;

			// Create the new output event.
			ILCEvent target = new ILCEvent();

			// First time, the event header needs to
			// be set from the first LCEvent read.
			boolean setEventHeader = true;

			// Total events merged in from all sources in this pass.
			int totmerged = 0;

			// Loop over the readers.
			for (int i = 0; i < readers.length; i++)
			{
				// Get the next reader.
				LCReader reader = readers[i];

				// Merge ntoread events from this reader into target with delta time of dt.
				int nmerged = MergeUtil.mergeEvents(target, reader, nEventsToRead, setEventHeader, dt, incrTime);

				// DEBUG
				System.err.println("nmerged: " + nmerged);

				// Increment total merged.
				totmerged += nmerged;

				// Next time, don't need to set the header.
				setEventHeader = false;
			}

			// Write out the combined event if something got merged in.
			if (totmerged > 0)
			{
				//System.err.println("totmerged: " + totmerged);

				writer.writeEvent(target);
				nevents++;
			}
			else
			{
				// Done!
				break;
			}
		} // file read loop

		// Close the writer.
		writer.close();

		// Close the readers.
		closeReaders(readers);
		
		// Return number of events created.
		return nevents;
	}
	
	/**
	 * Merge n events read from a reader into a single target event.
	 * @return Number of events overlayed from overlayEvents reader.
	 * @param targetEvent target LCEvent to merge events into
	 * @param overlayEvents LCReader with overlay events to read
	 * @param ntoread number of events to overlay from overlayEvents
	 * @param dt delta time to be applied to the sim types
	 */
	public static int mergeEvents(
			LCEvent targetEvent, 
			LCReader overlayEvents, 
			int ntoread, 			 
			boolean setEventHeader,
			float dt,
			boolean incrTime) throws IOException
	{
		// Read the next event from the reader.
		LCEvent nextOverlayEvent = overlayEvents.readNextEvent();

		// Return 0 if reader is exhausted.
		if (nextOverlayEvent == null)
		{
			return 0;
		}

		// Set the event header from first event if setEventHeader flag is true.
		if (setEventHeader)
		{
			ILCEvent itargetEvent = (ILCEvent) targetEvent;
			itargetEvent.setDetectorName(nextOverlayEvent.getDetectorName());
			itargetEvent.setEventNumber(nextOverlayEvent.getEventNumber());
			itargetEvent.setRunNumber(nextOverlayEvent.getRunNumber());
			itargetEvent.setTimeStamp(nextOverlayEvent.getTimeStamp());
		}

		// Read ntoread events from reader and merge into targetEvent.
		int nevt = 0;
		float time = dt;
		for (; nevt < ntoread && nextOverlayEvent != null; nevt++)
		{
			// Merge single overlay event onto targetEvent.
			mergeSingleEvent((ILCEvent) targetEvent, nextOverlayEvent, dt);

			// Get next event to merge in. (could be null)
			nextOverlayEvent = overlayEvents.readNextEvent();
			
			// Increment the delta time for next event.
			if (incrTime)
				time += dt;
		}

		// Return the number of events that were overlayed.
		return nevt;
	}
		
	/** 
	 * Overlay the overlay event onto target event, applying time delta.
	 * @param targetEvent The event that will be overlayed onto from overlayEvent.
	 * @param overlayEvent An event to be overlayed onto targetEvent.
	 * @param dt A delta time to apply to sim types.
	 */
	public static void mergeSingleEvent(ILCEvent targetEvent, LCEvent overlayEvent, float dt)
	{
		if (overlayEvent.getCollectionNames().length == 0)
		{
			// Empty event, nothing to do.
			return;
		}

		if (overlayEvent instanceof SIOEvent)
		{
			// Turn off readonly mode on overlay event.
			((SIOEvent) overlayEvent).setReadOnly(false);
		}

		// Get names of collections already existing in target.
		String[] tcolls = targetEvent.getCollectionNames();

		// Get names of collections to be merged in.
		String[] ocolls = overlayEvent.getCollectionNames();

		// Iterate over the overlay collections that will be merged into target.
		for (int i = 0; i < ocolls.length; i++)
		{
			// Get name of this overlay collection.
			String collname = ocolls[i];

			// Get this overlay collection.
			LCCollection ocoll = overlayEvent.getCollection(collname);

			// Declare the target collection reference,
			// which will be either a new collection or
			// an existing one from the target.
			LCCollection tcoll = null;

			// Find the index of this overlay collection in the target.
			int tidx = getCollectionIdx(tcolls, collname);

			// If index is -1, there is no existing collection in the target.
			if (tidx == -1)
			{
				// Create an empty target collection with the overlay collection's settings.
				tcoll = copy(overlayEvent.getCollection(collname));

				// Add the new collection to the target event.
				targetEvent.addCollection(tcoll, collname);
			}
			else
			{
				// Get the existing target collection.
				tcoll = targetEvent.getCollection(collname);
			}

			// Handle overlay for each pair of target-overlay collections.
			mergeCollection(tcoll, ocoll, dt);
		}

		return;
	}
	
	/**
	 * overlayCollection overlays the overlayColl onto the targetColl applying dt.
	 * @param targetColl The target collection to be modified.
	 * @param overlayColl The overlay collection containing objects to merge into targetColl.
	 * @param dt A delta time parameter applied to sim types of overlayColl before merge-in.
	 */
	public static void mergeCollection(LCCollection targetColl, LCCollection overlayColl, float dt)
	{
		String colltype = targetColl.getTypeName();

		// Handle a SimTrackerHit collection.
		if (colltype.compareTo(LCIO.SIMTRACKERHIT) == 0)
		{
			// Loop over hits to merge in.
			for (int ii = 0; ii < overlayColl.size(); ii++)
			{
				// Get the hit from the overlay collection.
				ISimTrackerHit hit = (ISimTrackerHit) overlayColl.getElementAt(ii);

				// Adjust time.
				hit.setTime(hit.getTime() + dt);

				// Add the hit to the target collection.
				targetColl.add(hit);
			}
		}
		// Handle a SimCalorimeterHit collection.
		else if (colltype.compareTo(LCIO.SIMCALORIMETERHIT) == 0)
		{
			// Loop over the overlay hits.
			for (int ii = 0; ii < overlayColl.size(); ii++)
			{
				// Get an overlay hit.
				ISimCalorimeterHit ohit = (ISimCalorimeterHit) overlayColl.getElementAt(ii);

				// Find a matching hit in target collection.
				ISimCalorimeterHit thit = findMatching(targetColl, ohit);

				if (thit != null)
					System.err.println("existing hit");
				
				// No matching hits?
				if (thit == null)
				{
					System.out.println("new hit");

					// Copy the overlay hit without MCParticle contributions.
					thit = copy(ohit);

					// Add the hit to the target collection.
					targetColl.addElement(thit);
				}

				// Apply MCParticle contributions from overlay to target.
				addMCParticleContributions(thit, ohit, dt);
			}
		}
		// Handle an MCParticle collection.
		else if (colltype.compareTo(LCIO.MCPARTICLE) == 0)
		{
			System.err.println("mcparticle");
			
			for (int ii = 0; ii < overlayColl.size(); ii++)
			{
				System.err.println("mcp #: " + ii);
				
				// Get the next MCParticle to add in.
				IMCParticle p = (IMCParticle) overlayColl.getElementAt(ii);

				// Apply dt to particle time.
				p.setTime(p.getTime() + dt);

				// Add to target collection.
				targetColl.add(p);
			}
		}
		// Handle a default collection.
		else
		{
			for (int ii = 0; ii < overlayColl.size(); ii++)
			{
				targetColl.add(overlayColl.getElementAt(ii));
			}
		}

	}
	
	/** 
	 * Find a SimCalorimeterHit in coll with matching cellId0, cellId1. 
	 * @return The first match of hit in coll.
	 * @param Collection of type SIMCALORIMETERHIT.
	 * @param hit Matching hit to be searched for in coll.
	 */
	public static ISimCalorimeterHit findMatching(LCCollection coll, SimCalorimeterHit hit)
	{	
		ISimCalorimeterHit match = null;

		for (int i = 0; i < coll.size(); i++)
		{
			ISimCalorimeterHit checkhit = (ISimCalorimeterHit) coll.getElementAt(i);
			if ((checkhit.getCellID0() == hit.getCellID0() && checkhit.getCellID1() == hit.getCellID1()) || checkhit == hit)
			{
				match = checkhit;
				break;
			}
		}
		
		return match;
	}

	/** 
	 * Find the given name in collnames.
	 * @return The index of name in collnames or -1 if not found.
	 * @param collnames A list of collection names.
	 * @param name A collection name to find in collnames. 
	 */
	public static int getCollectionIdx(String[] collnames, String name)
	{
		int idx = -1;
		for (int i = 0; i < collnames.length; i++)
		{
			if (collnames[i].compareTo(name) == 0)
			{
				idx = i;
				break;
			}
		}
		return idx;
	}

	/** 
	 * Add MCParticle contributions from an overlay hit to a target hit. 
	 * @param target Hit that receives MCParticle contributions.
	 * @param hit Hit containing MCParticle contributions to be copied.
	 * @param dt Delta time to apply to contributions.
	 */
	public static void addMCParticleContributions(ISimCalorimeterHit target, SimCalorimeterHit hit, float dt)
	{
		// Get the hit energy.
		float e = hit.getEnergy();
		
		//System.err.println("nmcp: " + hit.getNMCContributions());
		for (int j = 0; j < hit.getNMCContributions(); j++)
		{
			// PDGID might not be set.
			int pdgid = 0;
			try
			{
				pdgid = target.getPDGCont(j);
			}
			catch (Exception x)
			{}

			// Add this MCContrib to the existing hit, applying dt.
			System.err.println("mcp contrib e: " + hit.getEnergyCont(j));
			target.addMCParticleContribution(
					hit.getParticleCont(j), 
					hit.getEnergyCont(j), 
					hit.getTimeCont(j) + dt, 
					pdgid);
			
			// Increment the energy by this particle contribution.
			e += hit.getEnergyCont(j);
		}
		
		// Set the energy in the new hit.
		target.setEnergy(e);
	}

	/** 
	 * Shallow copy a SimCalorimeterHit.  
	 * Do not copy MCParticle contributions.
	 * @return A copy of hit. 
	 * @param hit The hit to be copied.
	 */
	private static ISimCalorimeterHit copy(SimCalorimeterHit hit)
	{
		ISimCalorimeterHit newhit = new ISimCalorimeterHit();
		newhit.setCellID0(hit.getCellID0());
		newhit.setCellID1(hit.getCellID1());
		newhit.setPosition(hit.getPosition());
		System.err.println("copied calhit pos: " + hit.getPosition()[0] + hit.getPosition()[1] + hit.getPosition()[2]);
		return newhit;
	}

	/** 
	 * Shallow copy an LCCollection.  
	 * Do not copy LCObject members.
	 * @return A copy of collection. 
	 * @param coll The LCCollection to be copied.
	 */
	private static ILCCollection copy(LCCollection coll)
	{
		ILCCollection newcoll = new ILCCollection(coll.getTypeName());
		newcoll.setDefault(coll.isDefault());
		newcoll.setFlag(coll.getFlag());
		newcoll.setParameters(coll.getParameters());
		newcoll.setSubset(coll.isSubset());
		newcoll.setTransient(coll.isTransient());

		return newcoll;
	}
	
	/** 
	 * Create an array of LCReaders to be read in parallel.
	 * @return Array of open LCReaders.
	 * @param Array of input files. 
	 */
	public static LCReader[] createReaders(File[] infiles) throws IOException
	{
		// Make a reader for each input event.		
		LCReader[] readers = new LCReader[infiles.length];
		for (int i = 0; i < infiles.length; i++)
		{
			// Create a new reader.
			readers[i] = LCFactory.getInstance().createLCReader();

			// Open the reader for this file.
			readers[i].open(((File) infiles[i]).getCanonicalPath());
		}
		return readers;
	}

	/** 
	 * Close all of the readers.
	 * @param readers Array of LCReaders to be closed. 
	 */
	public static void closeReaders(LCReader[] readers) throws IOException
	{
		for (int i = 0; i < readers.length; i++)
		{
			try
			{
				readers[i].close();
			}
			catch (Throwable t)
			{
				// Ignore errors on close.
			}
		}
	}
	
	/**
	 * Return an array of files from an array of file paths.
	 * @param fstr Array of file paths.
	 * @return Array of File objects.
	 */
	public static File[] createFiles(String[] fstr)
	{
		File[] infiles = new File[fstr.length];
		for (int i = 0; i < fstr.length; i++)
		{
			String ifile = (String) fstr[i];
			infiles[i] = new File(ifile);

			// DEBUG
			//System.out.println("added input file: " + ifile);
		}
		return infiles;
	}
}