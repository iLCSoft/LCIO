#!/usr/bin/python

import lcio
import gc

# Factory
fac=lcio.LCFactory.getInstance()

# LCReader
rdr=fac.createLCReader()
rdr.open("test.slcio")

# Event
evt=rdr.readNextEvent()

# Read some collections
hits = evt.getCollection('EcalBarrHits')
particles = evt.getCollection("MCParticle")
mcp = particles.getElementAt(0)

print "MCParticle energy: ", mcp.getEnergy() ;

rdr.close()

