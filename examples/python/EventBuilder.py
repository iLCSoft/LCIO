'''
Created on May 17, 2013

Example for creating LCIO events and filling them with MCParticles and SimTrackerHits.

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

from pyLCIO import EVENT, IMPL, IOIMPL, UTIL
from ROOT import TVector3, TLorentzVector, TRandom3, TMath
from time import time

import sys, math

def generateEvents( outputFileName, nEvents ):
    
    random = TRandom3( 12345 )
    
    # define a particle source
    sourcePosition = TVector3( 0., 0., 0. )
    sourceSpreadXY = 10.
    pdgid = 13
    charge = -1.
    mass = 0.105658
    momentum = TVector3( 0.3, 0.1, 10. )
    runNumber = 321
    
    # define a detector with positions for the tracker planes
    detectorName = 'ToyTracker'
    trackerPlanePositions = []
    hitResolution = 0.01
    planeNormal = TVector3( 0., 0., 1. )
    for planeZ in [ 100., 250., 480., 510., 640. ]:
        trackerPlanePositions.append( TVector3( 0., 0., planeZ ) )
    
    # create a writer and open the output file
    writer = IOIMPL.LCFactory.getInstance().createLCWriter()
    writer.open( outputFileName, EVENT.LCIO.WRITE_NEW )
    
    # create a run header and add it to the file (optional)
    run = IMPL.LCRunHeaderImpl()
    run.setRunNumber( runNumber )
    run.setDetectorName( detectorName )
    run.setDescription( 'This is a test run' )
    writer.writeRunHeader( run )
    
    for iEvent in range( nEvents ):
        
        # create an event and set its parameters
        event = IMPL.LCEventImpl()
        event.setEventNumber( iEvent )
        event.setDetectorName( detectorName )
        event.setRunNumber( runNumber )
        event.setTimeStamp( int( time() * 1000000000. ) )
        
        # create the mc particle collection
        mcParticles = IMPL.LCCollectionVec( EVENT.LCIO.MCPARTICLE )
        
        # calculate the origin of the particle
        x = random.Gaus( sourcePosition.x(), sourceSpreadXY )
        y = random.Gaus( sourcePosition.y(), sourceSpreadXY )
        z = sourcePosition.z()
        origin = TVector3( x, y, z )
        
        # create a particle
        mcParticle = IMPL.MCParticleImpl()
        mcParticle.setPDG( pdgid )
        mcParticle.setMass( mass )
        mcParticle.setMomentumVec( momentum )
        mcParticle.setGeneratorStatus( 1 )
        mcParticle.setVertexVec( origin )
        mcParticle.setTime( 0. )
        mcParticles.addElement( mcParticle )
        
        # create a tracker hit collection
        trackerHits = IMPL.LCCollectionVec( EVENT.LCIO.SIMTRACKERHIT )
        trackerHits.setFlag( UTIL.set_bit( trackerHits.getFlag(), EVENT.LCIO.THBIT_MOMENTUM ) )
        
        # create an IDEncoder to store hit IDs
        # defines the tags and the number of bits for the different bit fields
        encodingString = 'system:3,layer:6'
        idEncoder = UTIL.CellIDEncoder( IMPL.SimTrackerHitImpl )( encodingString, trackerHits )
        
        # add a hit for each layer
        for planePosition in trackerPlanePositions:
            # calculate the intersection with the plane
            distance = ( planePosition - origin ).Dot( planeNormal ) / momentum.Dot( planeNormal )
            intersect = TVector3( momentum )
            intersect.SetMag( distance )

            # smear the hit position with the resolution            
            hitX = random.Gaus( intersect.x(), hitResolution )
            hitY = random.Gaus( intersect.x(), hitResolution )
            hitPosition = TVector3( hitX, hitY, intersect.z() )
            
            # build the tracker hit
            trackerHit = IMPL.SimTrackerHitImpl()
            trackerHit.setPositionVec( hitPosition )
            trackerHit.setMomentumVec( momentum )
            trackerHit.setMCParticle( mcParticle )
            trackerHit.setTime( distance / TMath.C() )
            trackerHit.setEDep( 0.1 )
            
            # set the cell ID
            idEncoder.reset()
            idEncoder['layer'] = trackerPlanePositions.index( planePosition )
            idEncoder['system'] = 1
            idEncoder.setCellID( trackerHit )
            
            trackerHits.addElement( trackerHit )
        
        event.addCollection( mcParticles, EVENT.LCIO.MCPARTICLE )
        event.addCollection( trackerHits, 'SimTrackerHits' )
        
        writer.writeEvent( event )
    
    writer.flush()
    writer.close()
        

def usage():
    print('Generates an MCParticle with associated SimTrackerHits for each event')
    print('Usage:\n  python %s <outputFile> <nEvents>' % ( sys.argv[0] ))

if __name__ == '__main__':
    if len( sys.argv ) < 3:
        usage()
        sys.exit( 1 )
    generateEvents( sys.argv[1], int( sys.argv[2] ) )
