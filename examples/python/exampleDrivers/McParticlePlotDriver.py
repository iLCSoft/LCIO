'''
Created on Dec 4, 2012

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

from pyLCIO.drivers.Driver import Driver
from ROOT import TH1D, TCanvas

class McParticlePlotDriver( Driver ):
    ''' Example driver to illustrate how to fill a histogram in an @EventLoop '''
    
    def __init__( self ):
        ''' Constructor '''
        Driver.__init__( self )
        self.histograms = {}
    
    def startOfData( self ):
        ''' Method called by the event loop at the beginning of the loop '''
        
        # Create two histograms for energy and pT
        self.histograms['Energy'] = TH1D( 'Energy', 'Energy;Energy [GeV];Entries', 50, 0., 260. )
        self.histograms['Pt'] = TH1D( 'Pt', 'pT;p_T [GeV];Entries', 50, 0., 100. )
        self.histograms['PDGID'] = TH1D( 'PDGID', 'PDG ID;PDG ID;Entries', 1200, -600, 600. )
        self.histograms['GeneratorStatus'] = TH1D( 'GeneratorStatus', 'Generator Status;Generator Status;Entries', 4, -0.5, 3.5 )
    
    def processEvent( self, event ):
        ''' Method called by the event loop for each event '''
        
        # Get the MCParticle collection from the event
        mcParticles = event.getMcParticles()
        
        # Loop over all MCParticles
        for mcParticle in mcParticles:
            
            # Get the four vector of the MCParticle
            v = mcParticle.getLorentzVec()
            
            # Fill the histograms
            self.histograms['Energy'].Fill( v.Energy() )
            self.histograms['Pt'].Fill( v.Pt() )
            self.histograms['PDGID'].Fill( mcParticle.getPDG() )
            self.histograms['GeneratorStatus'].Fill( mcParticle.getGeneratorStatus() )
        
    def endOfData( self ):
        ''' Method called by the event loop at the end of the loop '''
        
        # Create a canvas for each histogram and draw them
        plots = []
        for histogramName in self.histograms:
            plot = TCanvas( 'c%s' % ( histogramName ), histogramName )
            self.histograms[histogramName].Draw()
            plots.append( plot )
        
        userInput = input( 'Press any key to continue' )
