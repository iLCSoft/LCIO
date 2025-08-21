'''
Created on Dec 4, 2012

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

import os

from pyLCIO.io.Reader import Reader
from pyLCIO import EVENT, IMPL, UTIL

class StdHepReader( Reader ):
    ''' Class to hold an LCStdHepRdr object '''

    def __init__( self, fileName=None):
        ''' Constructor '''
        
        Reader.__init__(self, None, fileName)
        self.processedEvents = 0
        
    def __open__( self, fileName ):
        if self.isOpen:
            self.__close__()
        self.reader = UTIL.LCStdHepRdr( fileName )
        self.isOpen = True
      
    def __read__( self ):
        ''' Get the next event from the stream '''
        event = IMPL.LCEventImpl()
        try:
            self.reader.updateNextEvent( event, EVENT.LCIO.MCPARTICLE )
            event.setEventNumber( self.processedEvents )
            self.processedEvents += 1
            return event
        except:
            return
    
