'''
Created on Dec 4, 2012

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

from pyLCIO import EVENT

class Reader( object ):
    ''' Generic reader class '''

    def __init__( self, reader, fileName=None ):
        ''' Constructor '''
        self.reader = reader
        self.fileList = []
        self.fileIter = iter( self.fileList )
        self.isOpen = False
        
        if fileName and isinstance( fileName, str ):
            self.addFile( fileName )
        elif isinstance( fileName, list ):
            self.addFileList( fileName )
    
    def __iter__( self ):
        return self
    
    def getNumberOfEvents( self ):
        ''' Get the number of events in the stream '''
        if len(self.fileList) == 0:
            return 0
        if not self.isOpen:
            self.__open__( self.fileList[0] )
        return self.reader.getNumberOfEvents()
    
    def addFile( self, fileName ):
        ''' Add a file to the list of files to be read '''
        self.fileList.append( fileName )
            
    def addFiles( self, files ):
        ''' Add a list of file names to be read '''
        self.fileList.extend( files )
    
    def addFileList( self, fileListName ):
        ''' Add a list of files to the reader using a text file containing the names of the input files (one per line)'''
        fileListFile = open( fileListName, 'r' )
        for line in fileListFile:
            self.fileList.append( line.strip() )
        fileListFile.close()
    
    def __next__( self ):
        ''' Reads the next event from the stream. '''
        if not self.isOpen:
            self.__open__( next(self.fileIter) )
        event = self.__read__()
        if event:
            return event
        else:
            self.__close__()
            return next(self)
    
    def skip( self, nEntries ):
        ''' Skip entries from the stream '''
        for i in range( int( nEntries ) ):
            next(self)
        
    def __close__( self ):
        ''' Close the reader and all streams '''
        try:
            self.reader.close()
        except Exception:
            pass
        self.isOpen = False
            
    def __read__( self ):
        ''' Reads the next entry from the stream. Needs to be implemented by the derived class '''
        return
    
    def __open__( self, fileName ):
        ''' Opens the given file. Needs to be implemented by the derived class '''
        self.isOpen = True
