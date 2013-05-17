'''
Created on Dec 4, 2012

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

import os

from pyLCIO.io.Reader import Reader
from ROOT import EVENT, UTIL

class StdHepReader( Reader ):
    ''' Class to hold an LCStdHepRdr object '''

    def __init__( self, fileName=None):
        ''' Constructor '''
        
        Reader.__init__(self, None, fileName)
        self.processedEvents = 0
        
    def setFile( self, fileName ):
        ''' Open a new StdHep file with the LCReader '''
        self.reader = UTIL.LCStdHepRdr( fileName )
        
        #nEvents = self.getNumberOfEvents()
        #print 'Opened %s with %d events' % ( os.path.split( fileName )[1], nEvents )
        
    def readEvent( self ):
        ''' Get the next event from the stream '''
        event = ROOT.IMPL.LCEventImpl()
        self.reader.updateNextEvent( event, EVENT.LCIO.MCPARTICLE )
        event.setEventNumber( self.processedEvents )
        self.processedEvents += 1
        return event
    
