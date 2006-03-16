#!/usr/bin/python

import lcio
import gc

print dir(lcio)

# Factory
fac=lcio.LCFactory.getInstance()
print repr(fac)

# LCReader
rdr=fac.createLCReader()
rdr.open("test.slcio")
print repr(rdr)

# Event
evt=rdr.readNextEvent()
print repr(evt)

# LCIO.h
print dir(lcio.LCIO)
print lcio.cvar

# SimCalorimeterHit extension
coll=evt.getSimCalorimeterHitCollection("EcalBarrHits")
hit=coll.getElementAt(0)
print repr(hit)

# MCParticle extension
coll=evt.getMCParticleCollection("MCParticle")
mcp=coll.getElementAt(0)
print repr(mcp)
print repr(mcp.getVertex())
print repr(mcp.getParents())

print "Collecting garbage ..."
gc.collect()
print "Done collecting garbage."

nms = evt.getCollectionNames()
print repr(nms)
