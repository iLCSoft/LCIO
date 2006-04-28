package hep.lcio.util;

import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.implementation.event.ILCCollection;
import hep.lcio.implementation.event.ILCEvent;
import hep.lcio.implementation.event.IMCParticle;
import hep.lcio.implementation.event.ISimCalorimeterHit;
import hep.lcio.implementation.event.ISimTrackerHit;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCReader;
import hep.lcio.io.LCWriter;

/**
 * Create some very simple test events for the Merge utility.
 * 
 * @author jeremym
 *
 */
public class MergeTestEvents
{
	public static void main(String[] argv) throws Exception 
	{ 		
		System.out.println("MergeTestEvents.main");
		
		LCWriter writer = 
			LCFactory.getInstance().createLCWriter();
		
		writer.open("test1", LCIO.WRITE_NEW);		

		ILCEvent event = new ILCEvent();
		event.setDetectorName("TEST");
		event.setEventNumber(0);
		event.setRunNumber(0);
		event.setTimeStamp(0);
		
		ILCCollection coll = new ILCCollection(LCIO.MCPARTICLE);
		IMCParticle p = new IMCParticle();
		coll.add(p);
		event.addCollection(coll, "particles");
				
		coll = new ILCCollection(LCIO.SIMCALORIMETERHIT); 
		ISimCalorimeterHit chit = new ISimCalorimeterHit();
		chit.addMCParticleContribution(p, (float)1.0, 0, 11);
		coll.add(chit);
		event.addCollection(coll, "calhits");
		
		coll = new ILCCollection(LCIO.SIMTRACKERHIT);
		ISimTrackerHit thit = new ISimTrackerHit();
		thit.setMCParticle(p);
		coll.add(thit);
		event.addCollection(coll, "tkrhits");
		
		writer.writeEvent(event);
		writer.close();
		
		writer.open("test2", LCIO.WRITE_NEW);
		writer.writeEvent(event);
		writer.close();
		
		LCReader reader = LCFactory.getInstance().createLCReader();
		reader.open("test1.slcio");
		LCEvent checkevent = reader.readNextEvent();
		System.out.println("read back event: " + checkevent);
	}
}
