package hep.lcio.implementation.sio;

import hep.lcio.event.CalorimeterHit;
import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;
import hep.lcio.event.LCIO;
import hep.lcio.event.MCParticle;
import hep.lcio.implementation.event.ICalorimeterHit;
import java.io.IOException;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOCalorimeterHit.java,v 1.1 2003-05-06 04:58:56 tonyj Exp $
 */
class SIOCalorimeterHit extends ICalorimeterHit {
    SIOCalorimeterHit(SIOInputStream in, int flags) throws IOException
    {
            cellId0 = in.readInt();
            cellId1 = in.readInt();
            energy = in.readFloat();
            //FIXME: C++ version ignores the flag!
//            if ((flags & LCIO.CHBIT_LONG) != 0)
//            {
                //FIXME: C++ version reads float, while lcio.xml says double
                position[0] = in.readFloat();
                position[1] = in.readFloat();
                position[2] = in.readFloat();
//            }
            nContributions = in.readInt();
            particle = new SIORef[nContributions];
            energyContrib = new float[nContributions];
            time = new float[nContributions];
            //FIXME: C++ version ignores the flag!
            //boolean hasPDG = (flags & LCIO.CHBIT_PDG) != 0;
            boolean hasPDG = true;
            if (hasPDG) pdg = new int[nContributions];
            for (int i=0; i<nContributions; i++)
            {
                particle[i] = in.readPntr();
                energyContrib[i] = in.readFloat();
                time[i] = in.readFloat();
                if (hasPDG) pdg[i] = in.readInt();
            }
    }
    private void write(SIOOutputStream out, int flags) throws IOException
    {
        out.writeInt(cellId0);
        out.writeInt(cellId1);
        out.writeFloat(energy);
        //FIXME:
//        if ((flags & LCIO.CHBIT_LONG) != 0)
//        {
            out.writeFloat(position[0]);
            out.writeFloat(position[1]);
            out.writeFloat(position[2]);            
//        }
        //FIXME:
        //boolean hasPDG = (flags & LCIO.CHBIT_PDG) != 0;
        boolean hasPDG = true;
        out.writeInt(nContributions);
        for (int i=0; i<nContributions; i++)
        {
            out.writePntr(particle[i]);
            out.writeFloat(energyContrib[i]);
            out.writeFloat(time[i]);
            if (hasPDG) out.writeInt(pdg[i]);
        }            
    } 
    static void write(CalorimeterHit hit, SIOOutputStream out, int flags) throws IOException {
        if (hit instanceof SIOCalorimeterHit) ((SIOCalorimeterHit) hit).write(out, flags);
        else {
            out.writeInt(hit.getCellID0());
            out.writeInt(hit.getCellID1());
            out.writeFloat(hit.getEnergy());
//            if ((flags & LCIO.CHBIT_LONG) != 0)
//            {
                float[] pos = hit.getPosition();
                out.writeFloat(pos[0]);
                out.writeFloat(pos[1]);
                out.writeFloat(pos[2]);            
//            }
//            boolean hasPDG = (flags & LCIO.CHBIT_PDG) != 0;
            boolean hasPDG = true;
	    int n = hit.getNMCParticles();
            out.writeInt(n);
            for (int i=0; i<n; i++)
            {
                out.writePntr(hit.getParticleCont(i));
                out.writeFloat(hit.getEnergyCont(i));
                out.writeFloat(hit.getTimeCont(i));
                if (hasPDG) out.writeInt(hit.getPDGCont(i));
            }  
        }
    }
    public MCParticle getParticleCont(int i) {
        if (particle[i] instanceof SIORef) particle[i] = ((SIORef) particle[i]).getObject();
        return (MCParticle) particle[i];
    }
}