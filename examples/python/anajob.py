'''
Created on May 19, 2013

Re-implementation of anajob.cc
Illustrates looping over events, run headers and collections, as well as some string formatting.

@author: Christian Grefe, CERN
'''
import sys

def formatVecValues( vec ):
    ''' Helper method to transform the content of a vector into a string '''
    if len( vec ) == 0:
        return ' [empty]'
    string = ''
    for val in vec:
        string += '%s, ' % ( val )
    return string

def printParameters( parameters ):
    from ROOT import vector
    ''' Helper method to print the content of an LCParameters object '''
    keyVec = vector( 'string' )()
    for intKey in parameters.getIntKeys( keyVec ):
        intVec = vector( 'int' )()
        parameters.getIntVals( intKey, intVec )
        print(' parameter %s [int]: %s' % ( intKey, formatVecValues( intVec ) ))
    keyVec.clear()
    for floatKey in parameters.getFloatKeys( keyVec ):
        floatVec = vector( 'float' )()
        parameters.getFloatVals( floatKey, floatVec )
        print(' parameter %s [float]: %s' % ( floatKey, formatVecValues( floatVec ) ))
    keyVec.clear()
    for stringKey in parameters.getStringKeys( keyVec ):
        stringVec = vector( 'string' )()
        parameters.getStringVals( stringKey, stringVec )
        print(' parameter %s [string]: %s' % ( stringKey, formatVecValues( stringVec ) ))

def dumpEvent( event ):
    ''' Helper method to print information on all collections in an LCEvent'''
    # the event
    print('///////////////////////////////////')
    print('EVENT:', event.getEventNumber())
    print('RUN:', event.getRunNumber())
    print('DETECTOR:', event.getDetectorName())
    print('COLLECTIONS: (see below)')
    print('///////////////////////////////////\n')
    
    print('---------------------------------------------------------------------------')
    print('{0:30}{1:25}{2:20}'.format( 'COLLECTION NAME', 'COLLECTION TYPE', 'NUMBER OF ELEMENTS' ))
    print('===========================================================================')
    
    for collectionName, collection in event:
        print('{0:30}{1:25}{2:9}'.format( collectionName, collection.getTypeName(), collection.getNumberOfElements() ))

    print('---------------------------------------------------------------------------\n\n')

def dumpRunHeader( run ):
    ''' Helper method to print information on a LCRunHeader'''
    print('  Run : %d - %s:  %s' % ( run.getRunNumber(), run.getDetectorName(), run.getDescription() ))
    printParameters( run.parameters() )
    print(' --------------------------------------------------------------- ')

def anajob( fileNames ):
    from ROOT import vector
    ''' Method to loop over all events of the given slcio files and print
        information about all collections'''

    from pyLCIO import IOIMPL
    reader = IOIMPL.LCFactory.getInstance().createLCReader()
    
    # first loop over all files: open them and print the total number of runs and events
    fileNameVec = vector( 'string' )()
    print('anajob: will open and read from files:')
    for fileName in fileNames:
        fileNameVec.push_back( fileName )
        reader.open( fileName )
        print('     %s     [ number of runs: %d, number of events: %d ]\n' % ( fileName, reader.getNumberOfRuns(), reader.getNumberOfEvents() ))
        reader.close()
    
    # second loop: print information on all run headers
    # this time we open all files in one stream by passing them as a std::vector<std::string>
    reader.open( fileNameVec )
    runHeader = reader.readNextRunHeader()
    while runHeader:
        dumpRunHeader( runHeader )
        runHeader = reader.readNextRunHeader()
    print('')
    reader.close()
    
    print(' will reopen and read from files:')
    for fileName in fileNames:
        print('     %s' % ( fileName ))
    
    # final loop: print information on all events
    reader.open( fileNameVec )
    nEvents = 0
    for event in reader:
        # use the c++ method or the one defined above for printing
        # from pyLCIO import UTIL
        # UTIL.LCTOOLS.dumpEvent( event )
        dumpEvent( event )
        nEvents += 1
    reader.close()
        
    print('  %s events read from files:' % ( nEvents ))
    for fileName in fileNames:
        print('     %s' % ( fileName ))

def usage():
    ''' Helper method to inform about the usage of this script '''
    print('usage: python anajob.py <input-file1> [[input-file2],...]')

if __name__ == '__main__':
    # check the command line parameters
    if len( sys.argv ) < 2 or sys.argv[1] in ['-h', '--help']:
        usage()
        sys.exit( 1 )
    
    # pass all arguments except for the first one as file names
    anajob( sys.argv[1:] )
    sys.exit( 0 )
