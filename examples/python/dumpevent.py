'''
Created on May 19, 2013

Re-implementation of dumpevent.cc

@author: Christian Grefe, CERN
'''
import sys

def dumpevent( fileName, eventNumber, runNumber=None ):
    from pyLCIO import IOIMPL
    reader = IOIMPL.LCFactory.getInstance().createLCReader()
    reader.open( fileName )
    
    event = None
    if runNumber:
        event = reader.readEvent( runNumber, eventNumber )
    else:
        if eventNumber > 1:
            reader.skipNEvents( eventNumber - 1 )
        event = reader.readNextEvent()
        
    if event:
        from pyLCIO import UTIL
        UTIL.LCTOOLS.dumpEventDetailed( event )
    elif runNumber:
        print(" couldn't find event %d - run %d in file %s" % ( eventNumber, runNumber, fileName )) 
    else:
        print(' less than %d events in file %s' % ( eventNumber, fileName ))

def usage():
    ''' Helper method to inform about the usage of this script '''
    print(' usage: python dumpevent.py filename runNum evtNum ')
    print('    or: python dumpevent.py filename n      ')
    print('  where the first dumps the event with the specified run and event number')
    print('  and the second simply dumps the n-th event in the file')

if __name__ == '__main__':
    # check the command line parameters
    if len( sys.argv ) < 3 or len( sys.argv ) > 4 or sys.argv[1] in ['-h', '--help']:
        usage()
        sys.exit( 1 )
    
    if len( sys.argv ) == 3:
        dumpevent( sys.argv[1], int( sys.argv[2] ) )
        sys.exit( 0 )
    
    if len( sys.argv ) == 4:
        dumpevent( sys.argv[1], int( sys.argv[2] ), int( sys.argv[3] ) )
        sys.exit( 0 )
