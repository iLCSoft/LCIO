package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcio.event.LCFloatVec;
import hep.lcio.implementation.event.ILCFloatVec;
import java.io.IOException;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIOFloatVec.java,v 1.1 2003-05-06 04:58:56 tonyj Exp $
 */
class SIOFloatVec extends ILCFloatVec {
    
    SIOFloatVec(SIOInputStream in) throws IOException {
    }
    private void write(SIOOutputStream out) throws IOException
    {
    }
    static void write(LCFloatVec vec, SIOOutputStream out) throws IOException {
        if (vec instanceof SIOFloatVec) ((SIOFloatVec) vec).write(out);
        else {

        }
    }
}
