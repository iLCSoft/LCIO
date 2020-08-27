'''
Created on May 8, 2013

Script to merge a given number of consecutive events into a single event

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

from pyLCIO import EVENT, IOIMPL, IMPL
from pyLCIO.io.LcioReader import LcioReader
from ROOT import vector
import os, sys


# Not all types might work out of the box. LCRelations seem to be problematic!
supportedTypes = [ 'MCParticle', 'Track', 'Cluster', 'ReconstructedParticle', 'SimTrackerHit', 'TrackerHit', 'CalorimeterHit', 'SimCalorimeterHit', 'Vertex' ]

def copyObjectParameters( obj, copyObj ):
    ''' Copies the parameters of an LCObject to another LCObject'''
    parameters = obj.getParameters()
    copyParameters = copyObj.parameters()
    keyVec = vector('string')()
    for intKey in parameters.getIntKeys( keyVec ):
        intVec = vector('int')()
        copyParameters.setValues( intKey, parameters.getIntVals( intKey, intVec ) )
    keyVec.clear()
    for floatKey in parameters.getFloatKeys( keyVec ):
        floatVec = vector('float')()
        copyParameters.setValues( floatKey, parameters.getFloatVals( floatKey, floatVec ) )
    keyVec.clear()
    for stringKey in parameters.getStringKeys( keyVec ):
        stringVec = vector('string')()
        copyParameters.setValues( stringKey, parameters.getStringVals( stringKey, stringVec ) )

def mergeEvents( event, combinedEvent ):
    ''' Merges all collections from the given event into the combinedEvent'''
    for collectionName in event.getCollectionNames():
        if collectionName in combinedEvent.getCollectionNames():
            # important to take ownership of the collection to prevent that its objects get deleted at the reading of the next event
            # the alternative would be a deep copy of each element
            collection = event.takeCollection( collectionName )
            combinedCollection = combinedEvent.getCollection( collectionName )
            for element in collection:
                combinedCollection.addElement( element )

def merge( inputFileName, outputFileName, nEventsPerEvent ):
    ''' Merges the events from the input file and writes them to the output file.
        Each output event will contain the information from the defined number of input events per event.'''
    # create a reader
    reader = LcioReader( inputFileName )
    
    print('Processing %d events from %s'%(reader.getNumberOfEvents(), inputFileName))
    
    # create a writer
    writer = IOIMPL.LCFactory.getInstance().createLCWriter()
    writer.open( outputFileName, EVENT.LCIO.WRITE_NEW )
    
    readEvents = 0
    readEventsSinceLastWrite = 0
    combinedEvents = 0
    combinedEvent = None
    
    # loop over the input file
    for event in reader:
        # check if we have merged sufficient events to write it to the output file
        if not combinedEvent or readEventsSinceLastWrite % nEventsPerEvent == 0:
            if combinedEvent:
                writer.writeEvent( combinedEvent )
                combinedEvents += 1
            readEventsSinceLastWrite = 0
            
            # create a new event and copy all parameters
            combinedEvent = IMPL.LCEventImpl()
            combinedEvent.setEventNumber( combinedEvents )
            combinedEvent.setRunNumber( event.getRunNumber() )
            combinedEvent.setDetectorName( event.getDetectorName() )
            combinedEvent.setTimeStamp( event.getTimeStamp() )
            copyObjectParameters( event, combinedEvent )
            
            # create new empty collections in the merged event for every collection in the input event
            for collectionName in event.getCollectionNames():
                collection = event.getCollection( collectionName )
                collectionType = collection.getTypeName()
                
                # only treat supported types
                if collectionType in supportedTypes:
                    combinedEvent.addCollection( IMPL.LCCollectionVec( collection.getTypeName() ), collectionName )
                    copyObjectParameters( collection, combinedEvent.getCollection( collectionName ) )

        mergeEvents( event, combinedEvent )
        if readEvents % 100 == 0 and readEvents != 0:
            print('Processed %d events'%(readEvents))
        readEvents += 1
        readEventsSinceLastWrite += 1
    
    # check if there are events not yet written
    if readEventsSinceLastWrite > 0:
        if combinedEvent:
            writer.writeEvent( combinedEvent )
    
    writer.flush()
    writer.close()

def usage():
    print('Merges the given number of consecutive events into a single event by concatenating all collections.')
    print('Usage:\n  python %s <inputFile> <outputFile> <nEventsPerEvent>'%(sys.argv[0]))

if __name__ == '__main__':
    if len(sys.argv) < 4:
        usage()
        sys.exit( 1 )
    merge( sys.argv[1], sys.argv[2], int(sys.argv[3]) )
    
