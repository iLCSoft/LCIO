'''
Created on Dec 4, 2012

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

from pyLCIO.drivers.Driver import Driver
from ROOT import EVENT, IOIMPL

class WriteLcioDriver( Driver ):
    ''' Driver to write events to an LCIO file '''

    def __init__( self ):
        ''' Constructor '''
        Driver.__init__( self )
        self.writer = IOIMPL.LCFactory.getInstance().createLCWriter()
        self.fileName = None
        self.writeMode = EVENT.LCIO.WRITE_NEW
        
    def setOutputFileName( self, fileName ):
        self.fileName = fileName
    
    def setWriteMode( self, writeMode=EVENT.LCIO.WRITE_NEW ):
        self.writeMode = writeMode
        
    def startOfData( self ):
        self.writer.open( self.fileName, self.writeMode )
    
    def processRunHeader( self, run ):
        self.writer.writeRunHeader( run )
    
    def processEvent( self, event ):
        self.writer.writeEvent( event )
        
    def endOfData( self ):
        self.writer.flush()
        self.writer.close()
        
    
        
