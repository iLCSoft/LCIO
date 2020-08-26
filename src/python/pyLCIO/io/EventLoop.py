'''
Created on Dec 4, 2012

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''
import os

from pyLCIO.drivers.Driver import Driver
from pyLCIO.io.LcioReader import LcioReader
from pyLCIO.io.StdHepReader import StdHepReader

class EventLoop:
    ''' Class that manages a loop of LCIO events '''
    
    def __init__( self ):
        ''' Constructor '''
        self.reader = None
        self.driver = Driver()
    
    def addFile( self, fileName ):
        if not self.reader:
            extension = fileName.split('.')[-1]
            if extension in ['slcio']:
                self.reader = LcioReader( fileName )
            elif extension in ['stdhep']:
                self.reader = StdHepReader( fileName )
            else:
                print('Unknown extension for %s' % ( extension ))
        else:
            self.reader.addFile( fileName )
        
    def addFiles( self, files):
        for fileName in files:
            self.addFile( fileName )
    
    def addFileList( self, fileListName ):
        fileListFile = open( fileList, 'r' )
        for line in fileListFile:
            self.addFile( line.strip() )
        fileListFile.close()
    
    def printStatistics( self ):
        self.driver.printStatistics()
    
    def add( self, driver ):
        ''' Add a driver which will be executed within the event loop '''
        self.driver.add( driver )
    
    def loop( self, nEvents= -1 ):
        ''' Loop over a defined number of events. A negative number will loop over all events '''
        self.driver.startOfData()
        iEvent = 0
        for event in self.reader:
            if iEvent == nEvents:
                break
            self.driver.processEvent( event )
            iEvent += 1
        self.driver.endOfData()
    
    def skipEvents( self, nEvents ):
        ''' Skip events from the current event loop '''
        self.reader.skip( int( nEvents ) )
            
