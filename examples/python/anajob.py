#!/usr/bin/env python
"""
Concise printout of the LCIO file contents
Re-implemented using python3 functions

@author: Nazar Bartosik, INFN Torino
"""

def formatVecValues(vec):
    """Helper method to transform the content of a vector into a string"""

    if len( vec ) == 0:
        return ' [empty]'
    return ', '.join([str(v) for v in vec])


def printParameters(parameters):
    """Helper method to print the content of an LCParameters object"""
    from ROOT import vector

    keyVec = vector( 'string' )()
    for intKey in parameters.getIntKeys( keyVec ):
        intVec = vector( 'int' )()
        parameters.getIntVals( intKey, intVec )
        print('- %s [int]: %s' % (intKey, formatVecValues(intVec)))
    keyVec.clear()
    for floatKey in parameters.getFloatKeys( keyVec ):
        floatVec = vector( 'float' )()
        parameters.getFloatVals( floatKey, floatVec )
        print('- %s [float]: %s' % (floatKey, formatVecValues(floatVec)))
    keyVec.clear()
    for stringKey in parameters.getStringKeys( keyVec ):
        stringVec = vector( 'string' )()
        parameters.getStringVals( stringKey, stringVec )
        print('- %s [string]: %s' % (stringKey, formatVecValues(stringVec)))


def dumpEvent(event):
    """Helper method to print information on all collections in an LCEvent"""

    print(f'RUN: {event.getRunNumber():<9d}  EVENT: {event.getEventNumber():<10d}  DETECTOR: {event.getDetectorName()}')
    print('---------------------------------------------------------------------------')
    print('{0:35}{1:27}{2:13}'.format('COLLECTION NAME', 'COLLECTION TYPE', '# OF ELEMENTS'))
    print('===========================================================================')

    for collectionName, collection in event:
        print('{0:35}{1:27}{2:>13}'.format(str(collectionName), collection.getTypeName(), collection.getNumberOfElements()))

    print('---------------------------------------------------------------------------\n\n')


def dumpRunHeader(run):
    """Helper method to print information on a LCRunHeader"""

    print('Run : %d - %s:  %s' % (run.getRunNumber(), run.getDetectorName(), run.getDescription()))
    printParameters( run.parameters() )
    print('')


def analyze_file(file_name, print_headers=False, max_events=None,
                 run_nr=None, event_nr=None):
    """Loop over all events of the given LCIO file and list the stored collections"""

    from pyLCIO import IOIMPL, UTIL

    reader = IOIMPL.LCFactory.getInstance().createLCReader()
    reader.open(file_name)

    print(f'Reading from file: {file_name}')
    print(f'# of runs: {reader.getNumberOfRuns()}   # of events: {reader.getNumberOfEvents()}\n')

    if print_headers:
        runHeader = reader.readNextRunHeader()
        while runHeader:
            dumpRunHeader(runHeader)
            runHeader = reader.readNextRunHeader()
        print()

    n_events_read = 0
    # Print the contents of each event
    reader.open( file_name )
    for event in reader:
        if max_events is not None and n_events_read >= max_events:
            break
        n_events_read += 1
        if run_nr is None and event_nr is not None:
            if event_nr != n_events_read:
                continue
            if event_nr > n_events_read:
                break
        elif run_nr is not None:
            if run_nr != event.getRunNumber():
                continue
            if event_nr is not None and event_nr != event.getEventNumber():
                continue
        # UTIL.LCTOOLS.dumpEvent( event )
        # Using the custom dump function with more compact formatting
        dumpEvent( event )
    reader.close()
    return n_events_read


if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='Prints summary of the LCIO file contents')
    parser.add_argument('files', metavar='FILE', help='Input file(s)', nargs='+')
    parser.add_argument('-e', '--event', metavar='NUMBER', help='Event number to print', type=int, default=None)
    parser.add_argument('-r', '--run', metavar='NUMBER', help='Run number to print', type=int, default=None)
    parser.add_argument('-m', '--max_events', metavar='N', help='Maximum number of events to print', type=int, default=None)
    parser.add_argument('-v', '--verbose', help='Print run headers', action='store_true')
    args = parser.parse_args()

    print(f'Analyzing {len(args.files)} file(s)')

    n_events_read = 0
    for file_name in args.files:
        max_events = None if args.max_events is None else args.max_events - n_events_read
        n_events_read += analyze_file(file_name, print_headers=args.verbose, max_events=max_events,
                                      run_nr=args.run, event_nr=args.event)
        if args.max_events is not None and n_events_read >= args.max_events:
            break
    print(f'\nFinished reading {n_events_read} event(s)')
