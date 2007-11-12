package hep.lcio.util;

import hep.io.stdhep.StdhepEvent;
import hep.io.stdhep.StdhepExtendedEvent;
import hep.io.stdhep.StdhepReader;
import hep.io.stdhep.StdhepRecord;
import hep.lcio.event.LCIO;
import hep.lcio.implementation.event.ILCCollection;
import hep.lcio.implementation.event.ILCEvent;
import hep.lcio.implementation.event.IMCParticle;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCWriter;
import hep.physics.particle.properties.ParticlePropertyManager;
import hep.physics.particle.properties.ParticlePropertyProvider;
import hep.physics.particle.properties.ParticleType;
import hep.physics.particle.properties.UnknownParticleIDException;

import java.io.EOFException;
import java.io.File;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * A class that converts StdhepEvents to LCCollections of MCParticles.
 * Based on Tony Johnson's StdhepConverter from the org.lcsim package.
 * Changed to use LCIO objects instead of MCEvent and modified for
 * Java 1.4 compatibility.
 *
 * @author Jeremy McCormick <jeremym@slac.stanford.edu>
 * @version $Id: StdhepConverter.java,v 1.8 2007-11-12 06:46:19 tonyj Exp $
 */
class StdhepConverter
{
   private ParticlePropertyProvider ppp = ParticlePropertyManager.getParticlePropertyProvider();
   
   // c_light copied from CLHEP 1.9.2.2
   // CLHEP/Units/PhysicalConstants.h
   private static final double c_light = 2.99792458e+8;
   private boolean haveWarned = false;
   
   StdhepConverter()
   {}
   
   public void convert(File stdhep, File lcio, int skipEvents, int maxEvents) throws Exception
   {
      // Create StdhepReader with the input file path.
      StdhepReader reader =
              new StdhepReader(stdhep.getAbsolutePath());
      
      // Create LCWriter with the output file path.
      LCWriter writer =
              LCFactory.getInstance().createLCWriter();
      writer.open(lcio.getAbsolutePath());
      
      // Skip events.
      if (skipEvents != -1)
      {
         for (int i=0; i<skipEvents; i++)
         {
            reader.nextRecord();
         }
      }
      
      int cntr = 0;
      
      try
      {
         // Loop over all records in the Stdhep file.
         for (;;)
         {
            if (maxEvents != -1 && cntr >= maxEvents)
               break;
            
            // Get the next Stdhep event.
            StdhepRecord record = reader.nextRecord();
            
            // Only process StdhepEvent records.
            if (record instanceof StdhepEvent)
            {
               // Convert to an LCCollection of MCParticle objects.
               ILCCollection mcpcoll =
                       convert((StdhepEvent) record);
               
               // Make a new LCEvent.
               ILCEvent event = new ILCEvent();
               
               event.setDetectorName("NONE");
               
               // FIXME: What values for these?
               event.setEventNumber(cntr);
               event.setRunNumber(0);
               event.setTimeStamp(0);
               
               // Add event weight
               if (record instanceof StdhepExtendedEvent)
               {
                  StdhepExtendedEvent extended = (StdhepExtendedEvent) record;
                  event.setWeight(extended.getEventWeight());
                  // In the ILC WhizData events IDRUP is used to flag the event type
                  int idrup = extended.getIDRUP();
                  if (idrup != 0) event.getParameters().setValue("_idrup",idrup);
               }
               
               // Add the MCParticle collection to the event.
               event.addCollection(mcpcoll, LCIO.MCPARTICLE);
               
               // Write out the event to the LCIO file.
               writer.writeEvent(event);
               
               // Garbage collect every 1000 events.
               if (cntr % 1000 == 0)
               {
                  System.gc();
               }
               
               // Increment event counter.
               ++cntr;
            }
         }
      }
      catch (EOFException e)
      {
         // End of Stdhep file.
      }
      
      reader.close();
      
      writer.close();
   }
   
   private void checkAndAddDaughter(IMCParticle[] particle, List ancestors, int parentID, int childID)
   {
      if (parentID == childID)
         return; // Can't be parent of self
      Set ancestor = (Set) ancestors.get(childID);
      boolean added = ancestor.add(particle[parentID]);
      if (added)
         particle[parentID].addDaughter(particle[childID]);
   }
   
   private int fillIndexVec(int[] vec, int idx1, int idx2)
   {
      int l = 0;
      try
      {
         if (idx1 >= 0 && idx2 >= 0)
         {
            if (idx1 < idx2)
            {
               for (int i = idx1; i < (idx2 + 1); i++)
               {
                  vec[l++] = i;
               }
            }
            else if (idx1 > idx2)
            {
               vec[l++] = idx1;
               vec[l++] = idx2;
            }
            // indices are equal
            else
            {
               vec[l++] = idx1;
            }
         }
         else if (idx1 >= 0)
         {
            vec[l++] = idx1;
         }
      }
      catch (ArrayIndexOutOfBoundsException x)
      {
         if (!haveWarned) System.err.println("Warning: Array index out of bounds exception caused by corrupt stdhep file ignored");
         haveWarned = true;
      }
      return l;
   }
   
   /**
    * Convert a StdhepEvent to an LCCollection of MCParticle objects.
    * @param hepevt The StdhepEvent to be converted.
    * @return An LCCollection of MCParticle objects converted from hepevt.
    */
   public ILCCollection convert(StdhepEvent hepevt)
   {
      ILCCollection mcpcoll = new ILCCollection(LCIO.MCPARTICLE);
      
      int n = hepevt.getNHEP();
      
      IMCParticle particles[] = new IMCParticle[n];
      
      for (int i = 0; i < n; i++)
      {
         // Create new MCParticle for this Stdhep record.
         IMCParticle particle = new IMCParticle();
         
         // Add MCParticle to the temp array.
         particles[i] = particle;
         
         // Set vertex from VHEP.
         double vertex[] =
         { hepevt.getVHEP(i, 0), hepevt.getVHEP(i, 1), hepevt.getVHEP(i, 2) };
         particle.setVertex(vertex);
         
         // Set momentum from PHEP.
         double momentum[] =
         { hepevt.getPHEP(i, 0), hepevt.getPHEP(i, 1), hepevt.getPHEP(i, 2) };
         particle.setMomentum(momentum);
         
         // Lookup the particle by PDG using the Particle Property Provider.
         ParticleType type;
         try
         {
            // Get the particle type.
            type = ppp.get(hepevt.getIDHEP(i));
            
            // Set the charge.
            particle.setCharge((float) type.getCharge());
         }
         catch (UnknownParticleIDException e)
         {
            // Flag the particle with NaN for unknown charge.
            particle.setCharge(Float.NaN);
         }
         
         // Set mass from PHEP.
         particle.setMass((float)hepevt.getPHEP(i, 4));
         
         // Set PDG from IDHEP.
         particle.setPDG(hepevt.getIDHEP(i));
         
         
         particle.setGeneratorStatus( hepevt.getISTHEP(i) ) ;
         
         // Set time from VHEP(4).
         // Convert to mm/c^2 from mm/c, as in slic/StdHepToLcioConvertor .
         particle.setTime((float)(hepevt.getVHEP(i, 3) / c_light));
      }
      
      int[] vec = new int[n];
      //List<Set<BasicParticle>> ancestors = new ArrayList<Set<BasicParticle>>(n);
      List ancestors = new ArrayList();
      for (int i = 0; i < n; i++)
         ancestors.add(new HashSet());
      // Deal with parents
      for (int i = 0; i < n; i++)
      {
         int idx1 = hepevt.getJMOHEP(i, 0) - 1;
         int idx2 = hepevt.getJMOHEP(i, 1) - 1;
         int l = fillIndexVec(vec, idx1, idx2);
         //System.out.println("parent: "+i+" "+idx1+" "+idx2+" "+l);
         for (int j = 0; j < l; j++)
         {
            checkAndAddDaughter(particles, ancestors, vec[j], i);
         }
      }
      // Deal with daughters
      for (int i = 0; i < n; i++)
      {
         int idx1 = hepevt.getJDAHEP(i, 0) % 10000 - 1;
         int idx2 = hepevt.getJDAHEP(i, 1) % 10000 - 1;
         int l = fillIndexVec(vec, idx1, idx2);
         //System.out.println("child: "+i+" "+idx1+" "+idx2+" "+l);
         for (int j = 0; j < l; j++)
         {
            checkAndAddDaughter(particles, ancestors, i, vec[j]);
         }
      }
      
      // Add particles to the collection.
      for (int i=0; i<n; i++)
      {
         mcpcoll.add(particles[i]);
      }
      
      return mcpcoll;
   }
}
