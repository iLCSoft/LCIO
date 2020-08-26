'''
Created on May 17, 2013

Example for setting up an @EventLoop

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

from pyLCIO.io.EventLoop import EventLoop
from pyLCIO.drivers.EventMarkerDriver import EventMarkerDriver
from exampleDrivers.McParticlePlotDriver import McParticlePlotDriver

import sys, os

def McParticlePlots( fileName ):
    '''Set up an event loop to read the input file and add a driver to
       plot some MCParticle properties.'''
    
    # Create the event loop
    eventLoop = EventLoop()
    
    # Set the input file. The actual reader is determined from the file ending (stdhep or slcio)
    eventLoop.addFile( fileName )
    
    # Add a driver to print the progress
    markerDriver = EventMarkerDriver()
    markerDriver.setInterval( 1 )
    markerDriver.setShowRunNumber( False )
    eventLoop.add( markerDriver )
    
    # Add the driver that draws the MCParticle plots
    mcParticlePlotDriver = McParticlePlotDriver()
    eventLoop.add( mcParticlePlotDriver )
    
    # Skip some events if desired
    eventLoop.skipEvents( 0 )
    
    # Execute the event loop
    eventLoop.loop( -1 )
    
def usage():
    print('Usage:\n  python %s <fileName>' % ( os.path.split( sys.argv[0] )[1] ))
    
if __name__ == "__main__":
    if len( sys.argv ) < 2:
        usage()
        sys.exit( 0 )
        
    # Read the file name from the command line input
    fileName = sys.argv[1]
    McParticlePlots( fileName )
