'''
Created on Dec 4, 2012

Module to handle dynamic decoration of LCIO classes

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

from ROOT import TVector3, TLorentzVector
from pyLCIO import EVENT, IMPL, IO, UTIL
from array import array

def addMethod( self, method, name=None ):
    ''' Convenience method to add a method to a class '''
    if not name:
        name = method.__name__
    setattr( self, name, method )

def getEndpointVec( self ):
    ''' Wrapper for the getEndpoint method to return a TVector3 '''
    v = self.getEndpoint()
    return TVector3( v[0], v[1], v[2] )
    
def getLorentzVec( self ):
    ''' Convenience method for classes that have momentum and energy '''
    energy = self.getEnergy()
    v = self.getMomentum()
    return TLorentzVector( v[0], v[1], v[2], energy )

def getMomentumVec( self ):
    ''' Wrapper for the getMomentum method to return a TVector3 '''
    v = self.getMomentum()
    return TVector3( v[0], v[1], v[2] )

def setMomentumVec( self, v ):
    ''' Wrapper for the setMomentum method passing a vector '''
    self.setMomentum( array( 'f', [v.x(), v.y(), v.z()] ) )

def getPositionVec( self ):
    ''' Wrapper for the getPosition method to return a TVector3 '''
    v = self.getPosition()
    return TVector3( v[0], v[1], v[2] ) 

def setPositionVec( self, v ):
    ''' Wrapper for the setPosition method passing a vector '''
    # tracker hits take pointer to doubles, calorimeter hits pointer to floats
    try:
        self.setPosition( array( 'd', [v.x(), v.y(), v.z()] ) )
    except TypeError:
        self.setPosition( array( 'f', [v.x(), v.y(), v.z()] ) )

def setRawDataVec( self, v ):
    ''' Wrapper for the setRawData method passing a vector '''
    self.setRawData( array( 'i', v ), len(v) )

def getReferencePointVec( self ):
    ''' Wrapper for the getReferencePoint method to return a TVector3 '''
    v = self.getReferencePoint()
    return TVector3( v[0], v[1], v[2] ) 

def setReferencePointVec( self, v ):
    ''' Wrapper for the setReferencePoint method passing a vector '''
    self.setReferencePoint( array( 'f', [v.x(), v.y(), v.z()] ) )

def getVertexVec( self ):
    ''' Wrapper for the getVertex method to return a TVector3 '''
    v = self.getVertex()
    return TVector3( v[0], v[1], v[2] )

def setVertexVec( self, v ):
    ''' Wrapper for the setVertex method passing a vector '''
    self.setVertex( array( 'd', [v.x(), v.y(), v.z()] ) )

def LCCollectionIterator( self ):
    ''' Helper method to make LCCollection iterable '''
    index = 0
    nElements = self.getNumberOfElements()
    while index < nElements:
        yield self.getElementAt( index )
        index += 1
        
def LCEventIterator( self ):
    ''' Helper method to make LCEvent iterable '''
    for collectionName in self.getCollectionNames():
        yield collectionName, self.getCollection( collectionName )
        
def LCReaderIterator( self ):
    ''' Helper method to make LCReader iterable '''
    event = self.readNextEvent()
    while event:
        yield event
        event = self.readNextEvent()
        
def LCStdHepRdrIterator( self ):
    ''' Helper method to make LCStdHepRdr iterable '''
    processedEvents = 0
    while True:
        event = IMPL.LCEventImpl()
        try:
            self.updateNextEvent( event )
            event.setEventNumber( processedEvents )
        except Exception:
            break
        yield event
        processedEvents += 1

def getMcParticles( self ):
    ''' Convenience method to get the default mc particle collection '''
    return self.getCollection( EVENT.LCIO.MCPARTICLE )

def getTracks( self ):
    ''' Convenience method to get the default track collection '''
    return self.getCollection( EVENT.LCIO.TRACK )


# List of LCIO classes to decorate
lcioClasses = [ EVENT.LCEvent,
                EVENT.CalorimeterHit,
                EVENT.Cluster,
                EVENT.RawCalorimeterHit,
                EVENT.ReconstructedParticle,
                EVENT.SimCalorimeterHit,
                EVENT.SimTrackerHit,
                EVENT.TPCHit,
                EVENT.Track,
                EVENT.TrackerData,
                EVENT.TrackerHit,
                EVENT.TrackState,
                EVENT.Vertex,
                EVENT.MCParticle,
                EVENT.LCCollection,
                IMPL.MCParticleImpl,
                IMPL.CalorimeterHitImpl,
                IMPL.ClusterImpl,
                IMPL.RawCalorimeterHitImpl,
                IMPL.ReconstructedParticleImpl,
                IMPL.SimCalorimeterHitImpl,
                IMPL.SimTrackerHitImpl,
                IMPL.TPCHitImpl,
                IMPL.TrackerDataImpl,
                IMPL.TrackerHitImpl,
                IMPL.TrackerHitPlaneImpl,
                IMPL.TrackerHitZCylinderImpl,
                IMPL.TrackerPulseImpl,
                IMPL.TrackerRawDataImpl,
                IMPL.TrackImpl,
                IMPL.TrackStateImpl,
                IMPL.VertexImpl,
                IO.LCReader,
                UTIL.LCStdHepRdr,
                ]

# Cache decorated classes
decoratedClasses = []

def decorateClass( o ):
    ''' Method to wrap the decoration logic '''
    if o not in decoratedClasses:
        d = dir( o )
        if 'getEndpoint' in d:
            addMethod( o, getEndpointVec )
        if 'getMomentum' in d:
            addMethod( o, getMomentumVec )
        if 'setMomentum' in d:
            addMethod( o, setMomentumVec )
        if 'getMomentum' and 'getEnergy' in d:
            addMethod( o, getLorentzVec )
        if 'getPosition' in d:
            addMethod( o, getPositionVec )
        if 'setPosition' in d:
            addMethod( o, setPositionVec )
        if 'getReferencePoint' in d:
            addMethod( o, getReferencePointVec )
        if 'setRawData' in d:
            addMethod( o, setRawDataVec )
        if 'setReferencePoint' in d:
            addMethod( o, setReferencePointVec )
        if 'getVertex' in d:
            addMethod( o, getVertexVec )
        if 'setVertex' in d:
            addMethod( o, setVertexVec )
        if 'getCollection' in d:
            addMethod( o, getMcParticles )
            addMethod( o, getTracks )
        if 'getNumberOfElements' and 'getElementAt' in d:
            addMethod( o, LCCollectionIterator, '__iter__' )
        if 'getCollectionNames' and 'getCollection' in d:
            addMethod( o, LCEventIterator, '__iter__' )
        if 'readNextEvent' in d:
            addMethod( o, LCReaderIterator, '__iter__' )
        if 'updateNextEvent' in d:
            addMethod( o, LCStdHepRdrIterator, '__iter__' )
        decoratedClasses.append( o )

def decorateLcioClasses():
    ''' Standard method to decorate all LCIO classes '''
    for lcioClass in lcioClasses:
        decorateClass( lcioClass )
