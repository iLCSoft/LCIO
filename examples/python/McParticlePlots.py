from pyLCIO.base.EventLoop import EventLoop
from pyLCIO.drivers.EventMarkerDriver import EventMarkerDriver
from exampleDrivers.McParticlePlotDriver import McParticlePlotDriver

import sys, os

def McParticlePlots( fileName ):
    eventLoop = EventLoop()
    eventLoop.setFile( fileName )
    
    # Add a driver to print the progress
    markerDriver = EventMarkerDriver()
    markerDriver.setInterval( 1 )
    markerDriver.setShowRunNumber( False )
    eventLoop.add( markerDriver )
    
    # Add the driver that draws the plots
    mcParticlePlotDriver = McParticlePlotDriver()
    eventLoop.add( mcParticlePlotDriver )
    
    eventLoop.skipEvents( 0 )
    
    # Execute the event loop
    eventLoop.loop( -1 )
    
def usage():
    print 'Usage:\n  python %s <fileName>' % ( os.path.split( sys.argv[0] )[1] )
    
if __name__ == "__main__":
    if len( sys.argv ) < 2:
        usage()
        sys.exit( 0 )
        
    # Read the file name from the command line input
    fileName = sys.argv[1]
    McParticlePlots( fileName )