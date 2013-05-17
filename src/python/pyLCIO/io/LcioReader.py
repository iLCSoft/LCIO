'''
Created on Dec 4, 2012

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

import os

from pyLCIO.io.Reader import Reader
from ROOT import IOIMPL

class LcioReader( Reader ):
    ''' Class to hold an LCReader object '''
    
    def __init__( self, fileName=None ):
        ''' Constructor allowing initialization with an LCIO file '''
        
        # Create a new LCReader
        reader = IOIMPL.LCFactory.getInstance().createLCReader()
        Reader.__init__(self, reader, fileName)
    
    def setFile( self, fileName ):
        ''' Open a new LCIO file with the LCReader '''
        self.reader.open( fileName )
        
        # Give some feedback on the new file
        #nRuns = self.getNumberOfRuns()
        #nEvents = self.getNumberOfEvents()
        #print 'Opened LCIO file with %d events in %d runs' % ( nEvents, nRuns )
    
    def getNumberOfRuns( self ):
        ''' Get the number of runs in the current file '''
        return self.reader.getNumberOfRuns()
    
    def readEvent( self ):
        ''' Get the next event from the stream '''
        return self.reader.readNextEvent()
    
#    def skipEvents( self, nEvents ):
#        ''' Skip events from the stream '''
#        self.reader.skipNEvents( nEvents )
            
