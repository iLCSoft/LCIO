'''
Created on Dec 4, 2012

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

class Reader( object ):
    ''' Generic reader class '''

    def __init__( self, reader, fileName=None ):
        ''' Constructor '''
        self.reader = reader
        self.fileList = None
        
        if fileName and isinstance( fileName, basestring ):
            self.setFile( fileName )
        elif isinstance( fileName, list ):
            self.setFileList( fileName )
    
    def getNumberOfEvents( self ):
        ''' Get the number of events in the current file '''
        return self.reader.getNumberOfEvents()
    
    def setFile( self, fileName ):
        pass
    
    def readEvent( self ):
        ''' Reads the next event from the stream. Needs to be implemented by the derived class '''
        pass
    
    def setFileList( self, fileList ):
        self.fileList = fileList
        self.nextFile()
        
    def nextEvent( self ):
        event = self.readEvent()
        if event:
            return event
        elif self.nextFile():
            return self.nextEvent()
    
    def nextFile( self ):
        if self.fileList:
            fileName = self.fileList.pop(0)
            if fileName:
                self.setFile( fileName )
                return True
            else:
                return False

    def skipEvents( self, nEvents ):
        ''' Skip events from the stream '''
        for i in xrange( int( nEvents ) ):
            self.nextEvent()
            
