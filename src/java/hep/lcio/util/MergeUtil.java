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
import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;

import java.io.File;
import java.io.IOException;
import java.util.Iterator;
import java.util.List;

/**
 * Utility methods for merging together LCIO 
 * files, events, and collections, with optional 
 * application of a delta time.
 * 
 * @author jeremym
 * @version $Id: MergeUtil.java,v 1.9 2009-05-22 20:18:59 cassell Exp $
 */
abstract public class MergeUtil
{
	/** 
	 * Merge events from mergeFiles into a single event in outfile,
	 * until all the files in mergeFiles are exhausted, or maxevents 
	 * are created. 
	 * @return The number of merged events that were created.
	 * @param outfile Target file that will contain the merged events.
	 * @param A list of @see MergeFileOptions objects.
	 * @param maxEvents Set the maximum number of merged events.
	 */
	public static int mergeFiles(File outfile, List mergeFiles, int maxEvents) throws IOException
	{
		// Create the writer for the merged events.
		LCWriter writer = LCFactory.getInstance().createLCWriter();

		// Open the writer.
		writer.open(outfile.getCanonicalPath(), LCIO.WRITE_NEW);

		// Count of total merged events that were created.
		int nevents = 0;
		
		boolean done=false;

		// File read loop.
		for (;;)
		{			
			// Check if max output events is reached.
			if (nevents >= maxEvents)
				break;
			
			System.out.println("merge - processing event <"+(nevents+1)+">");

			// Create the new output event.
			ILCEvent targetEvent = new ILCEvent();

			// On the first time through, the event header needs to
			// be set from the first LCEvent that is read.
			boolean setEventHeader = true;

			// Total events merged in from all files.
			int totmerged = 0;

			// Loop over the files to be merged in.
			for (Iterator iter = mergeFiles.iterator(); iter.hasNext();)
			{
				// Get the next set of merge file parameters.
				MergeFileParameters mfile = (MergeFileParameters) iter.next();

				// Get the number of events to read.
				int ntoread = mfile.nreads();

				// Get the reader.
				LCReader reader = mfile.reader();

				// Get the starting time.
				float startt = mfile.startt();

				// Get the delta time.
				float dt = mfile.dt();

				// Merge ntoread events from this reader into target,
				// using starting time of startt, delta time of dt.
                                int nmerged = 0;
                                if(ntoread > 0)
                                {
				nmerged = MergeUtil.mergeEvents(targetEvent, reader, ntoread, setEventHeader, startt, dt);
				
				// Check if the input file ran out of events.
				if (nmerged==0) {
				
					System.out.println("EOF on <"+mfile.file().getCanonicalPath()+">.  This merge event will be discarded.");
				
					// Invalidate this event.
					totmerged=0;
					
					// Stop processing this event.
					break;
				}
                                }

				// Increment total merged.
				totmerged += nmerged;

				// Next time, don't need to set the header.
				setEventHeader = false;
			}

			// Write out the combined event if it is valid.
			// The totmerged counter will be reset if any
			// of the input events runs out of events.
			if (totmerged > 0)
			{
				writer.writeEvent(targetEvent);
				nevents++;
			}
			else
			{
				// Done!
				break;
			}
		} // file read loop

		System.out.println("Created " + nevents + " merged events.");

		// Close the writer.
		writer.close();

		// Close the readers.
		for (Iterator iter = mergeFiles.iterator(); iter.hasNext();)
		{
			try
			{
				((MergeFileParameters) iter.next()).close();
			}
			catch (Exception x)
			{
			}
		}

		// Return number of events created.
		return nevents;
	}

	/** 
	 * Merge nEventsToRead events from each File in mergeFiles into a single event in outfile,
	 * until records in mergeFiles are exhausted. 
	 * @return The number of merged events that were created.
	 * @param outfile Output Target file containing the merged events.
	 * @param A list of @see MergeFileOptions objects.  
	 */
	public static int mergeFiles(File outfile, List mergeFiles) throws IOException
	{
		return mergeFiles(outfile, mergeFiles, Integer.MAX_VALUE);
	}

	/**
	 * Merge ntoread events from a reader into a single target event, applying time
	 * delta dt with starting time of startt.
	 * @return Number of events overlayed from overlayEvents reader.
	 * @param targetEvent target LCEvent to merge events into
	 * @param overlayEvents LCReader with overlay events to read
	 * @param ntoread number of events to overlay from overlayEvents
	 * @param dt delta time to be applied to the sim types
	 */
	public static int mergeEvents(LCEvent targetEvent, LCReader overlayEvents, int ntoread, boolean setEventHeader, float startt, float dt) throws IOException
	{
		// Read the next event from the reader.
		LCEvent nextEvent = overlayEvents.readNextEvent(LCIO.UPDATE);

		// Return 0 if reader is exhausted.
		if (nextEvent == null)
		{
			return 0;
		}

		// Set the event header from the first event if setEventHeader flag is true.
		if (setEventHeader)
		{
			ILCEvent itargetEvent = (ILCEvent) targetEvent;
			itargetEvent.setDetectorName(nextEvent.getDetectorName());
			itargetEvent.setEventNumber(nextEvent.getEventNumber());
			itargetEvent.setRunNumber(nextEvent.getRunNumber());
			itargetEvent.setTimeStamp(nextEvent.getTimeStamp());
		}

		// Number of events merged in.
		int nevt = 0;

		// Set starting time.
		float time = startt;

		// Read loop.
		while (nextEvent != null)
		{
			// Merge single overlay event into targetEvent.
			mergeSingleEvent((ILCEvent) targetEvent, nextEvent, dt);

			// Increment number of events read.
			nevt++;

			// Break if read max num events.
			if (nevt >= ntoread)
				break;

			// Get next event to merge in. (could be null)
			nextEvent = overlayEvents.readNextEvent(LCIO.UPDATE);

			// Increment the time for next event.
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
		//System.out.println("mergeSingleEvent");
		
		if (overlayEvent.getCollectionNames().length == 0)
		{
			// Empty event, nothing to do.
			return;
		}

		// Get names of collections already existing in target.
		String[] tcolls = targetEvent.getCollectionNames();

		// Get names of collections to be merged in.
		String[] ocolls = overlayEvent.getCollectionNames();

		// Iterate over the overlay collections that will be merged into target.
		for (int i = 0; i < ocolls.length; i++)
		{
			// Get the name of this overlay collection.
			String collname = ocolls[i];
			
			//System.out.println("collname="+ocolls[i]);

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
				// Create an empty target collection with the overlay 
				// collection's settings.
				tcoll = copy(overlayEvent.getCollection(collname));

				// Add the new collection to the target event.
				targetEvent.addCollection(tcoll, collname);
			}
			else
			{
				// Get the existing target collection.
				tcoll = targetEvent.getCollection(collname);
			}		
			
			//System.out.println("merging " + tcolls[i] + " and " + ocolls[i]);

			// Overlay overlay collection into target collection.
			mergeCollection(tcoll, ocoll, dt);
		}

		return;
	}

	/**
	 * overlayCollection overlays the overlayColl onto the targetColl applying 
	 * the delta time, dt.
	 * @param targetColl The target collection to be modified.
	 * @param overlayColl The overlay collection containing objects to merge into targetColl.
	 * @param dt A delta time parameter applied to sim types of overlayColl before merge-in.
	 */
	public static void mergeCollection(LCCollection targetColl, LCCollection overlayColl, float dt)
	{
		//System.out.println("mergeCollection");
				
		String colltype = targetColl.getTypeName();
		
		//System.out.println("colltype="+colltype);

		// Handle a SimTrackerHit collection.
		if (colltype.compareTo(LCIO.SIMTRACKERHIT) == 0)
		{
			mergeSimTrackerHitCollection(targetColl, overlayColl, dt);
		}
		// Handle a SimCalorimeterHit collection.
		else if (colltype.compareTo(LCIO.SIMCALORIMETERHIT) == 0)
		{
			mergeSimCalorimeterHitCollection(targetColl, overlayColl, dt);
		}
		// Handle an MCParticle collection.
		else if (colltype.compareTo(LCIO.MCPARTICLE) == 0)
		{
			mergeMCParticleCollection(targetColl, overlayColl, dt);
		}
		// Handle a default collection.
		else
		{
			mergeDefaultCollection(targetColl, overlayColl);
		}

	}

	/**
	 * Merge overlayColl into targetColl without applying a delta time.
	 * This is used to merge non-simulator collections.
	 * @param targetColl The target collection to be modified.
	 * @param overlayColl The overlay collection containing objects to merge into targetColl.
	 */
	public static void mergeDefaultCollection(LCCollection targetColl, LCCollection overlayColl)
	{
		for (int ii = 0; ii < overlayColl.size(); ii++)
		{
			targetColl.add(overlayColl.getElementAt(ii));
		}
	}

	/**
	 * Merge MCParticle collection overlayColl into targetColl.
	 * @param targetColl The target collection to be modified.
	 * @param overlayColl The overlay collection containing objects to merge into targetColl.
	 * @param dt A delta time parameter added to the particle's production time.
	 */
	public static void mergeMCParticleCollection(LCCollection targetColl, LCCollection overlayColl, float dt)
	{
		for (int ii = 0; ii < overlayColl.size(); ii++)
		{
			// Get the next MCParticle to add in.
			IMCParticle p = (IMCParticle) overlayColl.getElementAt(ii);

			// Apply dt to particle time.
			p.setTime(p.getTime() + dt);

			// Add to target collection.
			targetColl.add(p);
		}
	}

	/**
	 * Merge SimCalorimeterHit collection overlayColl into targetColl.
	 * Hits with matching cell ID's are combined.
	 * @param targetColl The target collection to be modified.
	 * @param overlayColl The overlay collection containing objects to merge into targetColl.
	 * @param dt A delta time parameter added to MCParticle contributions from overlayColl's hits.
	 */
	public static void mergeSimCalorimeterHitCollection(LCCollection targetColl, LCCollection overlayColl, float dt)
	{
		// Loop over the overlay hits.
		for (int ii = 0; ii < overlayColl.size(); ii++)
		{
			// Get an overlay hit.
			ISimCalorimeterHit ohit = (ISimCalorimeterHit) overlayColl.getElementAt(ii);

			// Find a matching hit in target collection.
			ISimCalorimeterHit thit = findMatching(targetColl, ohit);

			// No matching hits?
			if (thit == null)
			{
				//System.out.println("new hit");

				// Copy the overlay hit without MCParticle contributions.
				thit = copy(ohit);

				// Add the hit to the target collection.
				targetColl.addElement(thit);
			}

			// Apply MCParticle contributions from overlay to target.
			addMCParticleContributions(thit, ohit, dt);
		}
	}

	/**
	 * Merge SimTrackerHit collection overlayColl into targetColl.
	 * @param targetColl The target collection to be modified.
	 * @param overlayColl The overlay collection containing objects to merge into targetColl.
	 * @param dt A delta time parameter added to the time of hits from overlayColl.
	 */
	public static void mergeSimTrackerHitCollection(LCCollection targetColl, LCCollection overlayColl, float dt)
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
	public static void addMCParticleContributions(ISimCalorimeterHit targetHit, SimCalorimeterHit hit, float dt)
	{
		// Get the hit energy.
		float e = targetHit.getEnergy();

		for (int j = 0; j < hit.getNMCContributions(); j++)
		{
			// PDGID might not be set.
			int pdgid = 0;
			try
			{
				pdgid = targetHit.getPDGCont(j);
			}
			catch (Exception x)
			{
			}

			// Add this MCParticle contribution.
			targetHit.addMCParticleContribution(hit.getParticleCont(j), hit.getEnergyCont(j), hit.getTimeCont(j) + dt, pdgid);

			// Increment the energy by this particle contribution.
			e += hit.getEnergyCont(j);
		}

		// Set the energy in the new hit.
		targetHit.setEnergy(e);
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
		return newhit;
	}

	/** 
	 * Shallow copy an LCCollection.  
	 * Does not copy LCObject members.
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
}
