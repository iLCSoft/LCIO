import hep.aida.*;
import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.MCParticle;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.ILCFactory;
import hep.lcio.io.LCEventListener;
import hep.lcio.io.LCReader;
import java.io.IOException;


public class LCIOAnalysis implements LCEventListener
{
    public LCIOAnalysis() throws IOException
    {
        IAnalysisFactory analysisFactory = IAnalysisFactory.create();
        tree = analysisFactory.createTreeFactory().create("c:\\lcio2.aida","xml",false,true);

        // Create histogram here
        IHistogramFactory histogramFactory = analysisFactory.createHistogramFactory(tree);
        nMcHist = histogramFactory.createHistogram1D("nMc", 50, 0, 100);
        etotCloud = histogramFactory.createCloud1D("Etot2"); 

        // Create tuple here
        ITupleFactory tupleFactory = analysisFactory.createTupleFactory(tree);
        tuple = tupleFactory.create("myTuple","My Title", "int nmc, float etot");
    }
    public void processEvent(LCEvent event)
    {
        LCCollection collection = event.getCollection("MCParticle");
        int nMc =  collection.getNumberOfElements();
        double etot = 0.0;
        for(int i = 0; i < nMc; i++)
        {
            MCParticle mcparticle = (MCParticle) collection.getElementAt(i);
            if(mcparticle.getHepEvtStatus() == 1 && mcparticle.getCharge() != 0) etot += mcparticle.getEnergy();
        }


        // Fill histograms here
        nMcHist.fill(nMc);
        etotCloud.fill(etot);


        // Fill tuple here
        tuple.fill(0,nMc);
        tuple.fill(1,(float) etot);
        tuple.addRow();
        n++;
    }
    private IHistogram1D nMcHist;
    private ICloud1D etotCloud;
    private ITuple tuple;
    private ITree tree;
    private int n;

    public static void main(String[] args) throws Exception
    {
       LCIOAnalysis2 analysis = new LCIOAnalysis2();
       ILCFactory factory = LCFactory.getInstance();
       LCReader reader = factory.createLCReader();
       reader.open(args[0]);
       reader.registerLCEventListener(analysis);
       reader.readStream();
       reader.close();
       System.out.println("Analyzed "+analysis.n+" events");
       analysis.tree.commit();
    }  
    
    public void modifyEvent(LCEvent lCEvent)
    {
       // No thanks
    } 
}
