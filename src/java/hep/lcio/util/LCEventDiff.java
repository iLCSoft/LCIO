package hep.lcio.util;

import hep.lcio.event.LCEvent;
import hep.lcio.io.LCReader;
import hep.lcio.io.ILCFactory;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.util.ObjectComparator;
import java.io.File;
import java.io.IOException;

public class LCEventDiff {
    public static void main(String[] args) throws IOException {
        if ( args.length > 1 ) {            
            try {
                File f1 = new File(args[0]);
                File f2 = new File(args[1]);
                
                ILCFactory factory = LCFactory.getInstance();
                LCReader rdr = factory.createLCReader();
                rdr.open(f1.getAbsolutePath());
                LCEvent e1 = rdr.readNextEvent();
                rdr.close();
                rdr.open(f2.getAbsolutePath());
                LCEvent e2 = rdr.readNextEvent();
                
                ObjectComparator cmp = new ObjectComparator();
                cmp.compare(e1, e2);
                System.out.println("Comparison between LCIO files " +
                        f1 + " and " + f2 + ": " + cmp.getResultString() );
            } catch (Throwable t) {
                t.printStackTrace();
            }
        } else {
            System.out.println("Usage: LCEventDiff [file1] [file2]");
        }
    }
}
