'''
Created on Dec 4, 2012

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

from pyLCIO.drivers.Driver import Driver

class EventMarkerDriver ( Driver ):
    ''' Driver to give feedback on the progress of an @EventLoop ''' 
    
    def __init__( self ):
        ''' Constructor '''
        Driver.__init__( self )
        self.eventPrefix = 'Event:'
        self.runPrefix = 'Run:'
        self.interval = 1
        self.showRunNumber = False
    
    def setEventPrefix( self, prefix ):
        ''' Set the prefix that is printed for each event. Default is "Event:" '''
        self.eventPrefix = prefix
        
    def setRunPrefix( self, prefix ):
        ''' Set the prefix that is printed for each run. Default is "Run:" '''
        self.runPrefix = prefix
    
    def setInterval( self, interval ):
        ''' Set the number of events between two printouts '''
        self.interval = interval
        
    def setShowRunNumber( self, show ):
        ''' Set if the run number is printed '''
        self.showRunNumber = show
    
    def processEvent( self, event ):
        ''' Method called by the event loop for every event '''
        eventNumber = event.getEventNumber()
        if eventNumber % self.interval == 0:
            if self.showRunNumber:
                print(self.runPrefix, event.getRunNumber(), self.eventPrefix, eventNumber)
            else:
                print(self.eventPrefix, eventNumber)
