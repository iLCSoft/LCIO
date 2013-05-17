'''
Created on Dec 4, 2012

Module to handle dynamic decoration of LCIO classes

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

from ROOT import TVector3, TLorentzVector, EVENT


def addMethod( self, method, name=None ):
    ''' Convenience method to add a method to a class '''
    if not name:
        name = method.func_name
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

def getPositionVec( self ):
    ''' Wrapper for the getPosition method to return a TVector3 '''
    v = self.getPosition()
    return TVector3( v[0], v[1], v[2] ) 

def getReferencePointVec( self ):
    ''' Wrapper for the getReferencePoint method to return a TVector3 '''
    v = self.getReferencePoint()
    return TVector3( v[0], v[1], v[2] ) 

def getVertexVec( self ):
    ''' Wrapper for the getVertex method to return a TVector3 '''
    v = self.getVertex()
    return TVector3( v[0], v[1], v[2] )

def LCCollectionIterator( self ):
    ''' Helper method to make LCCollection iterable '''
    index = 0
    nElements = self.getNumberOfElements()
    while index < nElements:
        yield self.getElementAt( index )
        index += 1

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
                EVENT.LCCollection ]

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
        if 'getMomentum' and 'getEnergy' in d:
            addMethod( o, getLorentzVec )
        if 'getPosition' in d:
            addMethod( o, getPositionVec )
        if 'getReferencePoint' in d:
            addMethod( o, getReferencePointVec )
        if 'getVertex' in d:
            addMethod( o, getVertexVec )
        if 'getCollection' in d:
            addMethod( o, getMcParticles )
            addMethod( o, getTracks )
        if 'getNumberOfElements' and 'getElementAt' in d:
            addMethod( o, LCCollectionIterator, '__iter__' )
        decoratedClasses.append( o )

def decorateLcioClasses():
    ''' Standard method to decorate all LCIO classes '''
    for lcioClass in lcioClasses:
        decorateClass( lcioClass )
