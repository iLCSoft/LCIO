'''
Created on Dec 4, 2012

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''
from pyLCIO import IO 
from time import time

class Driver():
    ''' Generic plug in to an @EventLoop '''
    
    def __init__( self ):
        ''' Constructor '''
        self.type = self.__class__.__name__
        self.drivers = []
        self.processedTime = 0.
        self.processedEvents = 0
    
    def add( self, driver ):
        ''' Adds a daughter driver that will be executed '''
        self.drivers.append( driver )
    
    def getType( self ):
        ''' Return the type of this driver '''
        return self.type
    
    def startOfData( self ):
        ''' Method that is executed at the beginning of an event loop '''
        for driver in self.drivers:
            startTime = time()
            driver.startOfData()
            driver.processedTime += time() - startTime
    
    def processRunHeader( self, runHeader ):
        ''' Method that is executed for each run header '''
        for driver in self.drivers:
            startTime = time()
            driver.processRunHeader( run )
            driver.processedTime += time() - startTime

    def processEvent( self, event ):
        ''' Method that is executed for each event '''
        for driver in self.drivers:
            startTime = time()
            driver.processEvent( event )
            driver.processedTime += time() - startTime
            driver.processedEvents += 1
    
    def endOfData( self ):
        ''' Method that is executed at the end of an event loop '''
        for driver in self.drivers:
            startTime = time()
            driver.endOfData()
            driver.processedTime += time() - startTime
            
    def printStatistics( self ):
        ''' Method that gives feedback on the processing time '''
        print('\nExecuted drivers:')
        for driver in self.drivers:
            msPerEvent = 0
            if driver.processedEvents != 0:
                msPerEvent = int(1000*driver.processedTime/driver.processedEvents) 
            print('  {0:25} {1:>7} events {2:>8} ms {3:>7} ms/event'.format(driver.getType(), driver.processedEvents, int(1000*driver.processedTime), msPerEvent))

