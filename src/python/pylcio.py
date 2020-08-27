from pyLCIO.io.EventLoop import EventLoop
from xml.etree import ElementTree

import sys, os
import importlib

def parseValue( value ):
    ''' Helper method to extract values from XML.
        The input will be interpreted as a number if possible.
        Otherwise it will be interpreted as a string. '''
    try:
        return float( value )
    except ValueError:
        pass
    return str( value ).strip()

def parseBoolean( value ):
    ''' Helper method to extract boolean values from XML. '''
    return parseValue( value ) in [ 1, 'true', 'True' ]

class PyLCIOXML:
    
    def __init__( self ):
        self.eventLoop = EventLoop()
        self.nEvents = -1
        self.skipEvents = 0
        self.printDrivers = False
        self.driverDict = {}
    
    def parseXML( self, fileName ):
        tree = ElementTree.parse( fileName )
        baseNode = tree.getroot()
        self.__parseInputFilesNode__( baseNode.find( 'inputFiles' ) )
        self.__parseControlNode__( baseNode.find( 'control' ) )
        self.__parseDriversNode__( baseNode.find( 'drivers' ) )
        self.__parseExecuteNode__( baseNode.find( 'execute' ) )
    
    def __parseInputFilesNode__( self, inputFilesNode ):
        for fileNode in inputFilesNode.findall( 'file' ):
            fileName = os.path.abspath( parseValue( fileNode.text ) )
            if os.path.isfile( fileName ):
                self.eventLoop.addFile( fileName )
        for fileNode in inputFilesNode.findall( 'fileList' ):
            fileListName = os.path.abspath( parseValue( fileNode.text ) )
            if os.path.isfile( fileListName ):
                self.eventLoop.addFile( fileListName )
                
    def __parseControlNode__( self, controlNode ):
        nEventsNode = controlNode.find( 'numberOfEvents' )
        skipEventsNode = controlNode.find( 'skipEvents' )
        printDriversNode = controlNode.find( 'printDrivers' )
        printStatisticsNode = controlNode.find( 'printStatistics' )
        if nEventsNode is not None:
            self.nEvents = parseValue( nEventsNode.text )
        if skipEventsNode is not None:
            self.skipEvents = parseValue( skipEventsNode.text )
        if printDriversNode is not None:
            self.printDrivers = parseBoolean( printDriversNode.text )
        if printStatisticsNode is not None:
            self.printStatistics = parseBoolean( printStatisticsNode.text )
            
    def __parseDriversNode__( self, driversNode ):
        if self.printDrivers:
            print('Drivers:')
        for driverNode in driversNode.findall( 'driver' ):
            driverName = driverNode.get( 'name' )
            driverType = driverNode.get( 'type' )
            driverClassName = driverType.split('.')[-1]
            driverModuleName = driverType.rstrip( driverClassName ).rstrip( '.' )
            try:
                driverModule = importlib.import_module( driverModuleName )
            except ImportError:
                print('Module "%s" does not exist'%(driverModuleName), file=sys.stderr)
                sys.exit( 2 )
            try:
                driverClass = getattr( driverModule , driverClassName )
            except AttributeError:
                print('%s does not have a class "%s"'%(driverModuleName, driverClassName), file=sys.stderr)
                sys.exit( 2 )
            driver = driverClass()
            self.driverDict[driverName] = driver
            if self.printDrivers:
                print('  %s of type %s'%(driverName, driverType))
            for parameterNode in driverNode:
                parameterName = parameterNode.tag
                parameterValue = parseValue( parameterNode.text )
                parameterMethodName = 'set%s%s'%(parameterName[0].upper(), parameterName[1:])
                try:
                    parameterMethod = getattr( driver, parameterMethodName )
                except AttributeError:
                    print('%s does not have a method "%s"'%(driverClassName, parameterMethodName), file=sys.stderr)
                    sys.exit( 2 )
                parameterMethod( parameterValue )
                if self.printDrivers:
                    print('    %s = %s'%(parameterName, parameterValue))
        if self.printDrivers:
            print('')
            
    def __parseExecuteNode__( self, executeNode ):
        if self.printDrivers:
            print('Executed drivers:')
            index = 1
        for driverNode in executeNode.findall( 'driver' ):
            driverName = driverNode.get( 'name' )
            self.eventLoop.add( self.driverDict[driverName] )
            if self.printDrivers:
                print('  %d) %s'%(index, driverName ))
                index += 1
        if self.printDrivers:
            print('')
                
    
    def run( self ):
        self.eventLoop.skipEvents( self.skipEvents )
        self.eventLoop.loop( self.nEvents )
        if self.printStatistics:
            self.eventLoop.printStatistics()

if __name__ == '__main__':
    pyLCIO = PyLCIOXML()
    pyLCIO.parseXML( sys.argv[1] )
    pyLCIO.run()

