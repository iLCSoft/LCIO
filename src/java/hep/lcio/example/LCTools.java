/*
 * LCTools.java
 *
 * Created on May 4, 2003, 9:48 PM
 */
package hep.lcio.example;

import hep.lcio.event.LCFloatVec;
import hep.lcio.event.LCIntVec;
import hep.lcio.event.LCStrVec;

import hep.lcio.event.CalorimeterHit;
import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.MCParticle;
import hep.lcio.event.SimCalorimeterHit;
import hep.lcio.event.SimTrackerHit;
import hep.lcio.event.TPCHit;


/**
 *
 * @author  Tony Johnson
 */
public class LCTools
{
   public static void dumpEvent(LCEvent evt)
   {
      // the event:
      System.out.println("event  : " + evt.getEventNumber() + " - run " + evt.getRunNumber() + " detector : " + evt.getDetectorName() + " - collections  : ");

      String[] strVec = evt.getCollectionNames();

      // loop over collections:
      for (int j = 0; j < strVec.length; j++)
      {
         String name = strVec[j];

         System.out.print("     " + name + " " + evt.getCollection(name).getTypeName() + " : ");

         LCCollection col = evt.getCollection(name);

         // print SimCalorimeterHit collections:
         if (evt.getCollection(name).getTypeName().equals(LCIO.SIMCALORIMETERHIT))
         {
            int nHits = col.getNumberOfElements();
            System.out.print(nHits + " hits - first hit: ");

            int nPrint = (nHits > 0) ? 1 : 0;

            if (nPrint == 0)
               System.out.println();
            for (int i = 0; i < nPrint; i++)
            {
               SimCalorimeterHit hit = (SimCalorimeterHit) col.getElementAt(i);

               float[] x = hit.getPosition();

               System.out.print("    hit -  e: " + hit.getEnergy() + "  pos: " + x[0] + ", " + x[1] + ", " + x[2]);

               if (hit.getNMCParticles() > 0)
                  System.out.print(" - mc: " + hit.getParticleCont(0).getPDG());

               System.out.println();
            }
         }

         // print CalorimeterHit collections:
         else if (evt.getCollection(name).getTypeName().equals(LCIO.CALORIMETERHIT))
         {
            int nHits = col.getNumberOfElements();
            System.out.print(nHits + " hits - first hit: ");

            int nPrint = (nHits > 0) ? 1 : 0;

            if (nPrint == 0)
               System.out.println();
            for (int i = 0; i < nPrint; i++)
            {
               CalorimeterHit hit = (CalorimeterHit) col.getElementAt(i);

               float[] x = hit.getPosition();

               System.out.print("    hit -  e: " + hit.getEnergy() + "  pos: " + x[0] + ", " + x[1] + ", " + x[2]);
               System.out.println();
            }
         }
         else if (evt.getCollection(name).getTypeName().equals(LCIO.SIMTRACKERHIT))
         {
            int nHits = col.getNumberOfElements();
            System.out.print(nHits + " hits : ");

            int nPrint = (nHits > 0) ? 1 : 0;

            if (nPrint == 0)
               System.out.println();
            for (int i = 0; i < nPrint; i++)
            {
               SimTrackerHit hit = (SimTrackerHit) col.getElementAt(i);

               double[] x = hit.getPosition();

               int pdg = -999;
               MCParticle part = hit.getMCParticle();
               if( part != null)
                 pdg = part.getPDG();

               System.out.print("    hit -  dEdx: " + hit.getdEdx() + "  mc: " + pdg + "  pos: " + x[0] + ", " + x[1] + ", " + x[2]);
            }
            System.out.println();
         }
         else if (evt.getCollection(name).getTypeName().equals(LCIO.TPCHIT))
         {
         // check whether we are dealing with raw data
            int flag = col.getFlag();
            int mask = 1<<LCIO.TPCBIT_RAW;
            boolean raw = (flag & mask) == mask;

             
            int nHits = col.getNumberOfElements();
            System.out.print(nHits + " hits : ");

            int nPrint = (nHits > 0) ? 1 : 0;

            if (nPrint == 0)
               System.out.println();
            for (int i = 0; i < nPrint; i++)
            {
               TPCHit hit = (TPCHit) col.getElementAt(i);
	       if(raw){
		   int nRawWords = hit.getNRawDataWords();
		   System.out.print( nRawWords +" raw data words:");
		   for(int rawWord = 0; rawWord< nRawWords; ++rawWord) {
		       System.out.print( " | " + hit.getRawDataWord(rawWord) );
		   }
		   System.out.println(" |");
	       }
            else
            {
               System.out.print("    hit -  id: " + hit.getCellID() + " time: " + hit.getTime()
                + "  charge: " + hit.getCharge() + ", quality: " + hit.getQuality() );
                }
            }
             
         }
         else if (evt.getCollection(name).getTypeName().equals(LCIO.LCFLOATVEC))
         {
            int nHits = col.getNumberOfElements();
            System.out.print(nHits + " vectors: ");

            int nPrint = (nHits > 0) ? 1 : 0;

            if (nPrint == 0)
               System.out.println();
            for (int i = 0; i < nPrint; i++)
            {
               LCFloatVec vec = (LCFloatVec) col.getElementAt(i);

               System.out.print(" values(" + i + "): ");

               float[] data = vec.toArray();
               for (int k = 0; k < data.length; k++)
                  System.out.print(data[k] + ", ");
               System.out.println();
            }
         }
         else if (evt.getCollection(name).getTypeName().equals(LCIO.LCINTVEC))
         {
            int nHits = col.getNumberOfElements();
            System.out.print(nHits + " vectors: ");

            int nPrint = (nHits > 0) ? 1 : 0;

            if (nPrint == 0)
               System.out.println();
            for (int i = 0; i < nPrint; i++)
            {
               LCIntVec vec = (LCIntVec) col.getElementAt(i);

               System.out.print(" values(" + i + "): ");

               int[] data = vec.toArray();
               for (int k = 0; k < data.length; k++)
                  System.out.print(data[k] + ", ");
               System.out.println();
            }            
         }         
         else if (evt.getCollection(name).getTypeName().equals(LCIO.LCSTRVEC))
         {
            int nHits = col.getNumberOfElements();
            System.out.print(nHits + " vectors: ");

            int nPrint = (nHits > 0) ? 1 : 0;

            if (nPrint == 0)
               System.out.println();
            for (int i = 0; i < nPrint; i++)
            {
               LCStrVec vec = (LCStrVec) col.getElementAt(i);

               System.out.print(" values(" + i + "): ");

               String[] data = vec.toArray();
               for (int k = 0; k < data.length; k++)
                  System.out.print(data[k] + ", ");
               System.out.println();
            }            
         }         
         else if (evt.getCollection(name).getTypeName().equals(LCIO.MCPARTICLE))
         {
            int nHits = col.getNumberOfElements();
            System.out.print(nHits + " particles : ");

            int nPrint = (nHits > 0) ? 1 : 0;

            if (nPrint == 0)
               System.out.println();
            for (int i = 0; i < nPrint; i++)
            {
               MCParticle part = (MCParticle) col.getElementAt(i);

               System.out.println("           " + part.getPDG() + " p: " + part.getMomentum()[0] + ", " + part.getMomentum()[1] + ", " + part.getMomentum()[2]);
            }
         }
         else
         {
            System.out.println();
         }
      }
      System.out.println();
   }
}
