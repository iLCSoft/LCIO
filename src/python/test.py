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

# LCWriter
writer=fac.createLCWriter()
writer.open("writer_test", lcio.LCIO.WRITE_NEW)
coll = lcio.LCCollectionVec(lcio.cvar.LCIO_SIMCALORIMETERHIT)
flag = lcio.LCFlagImpl()
flag.setBit( lcio.LCIO.CHBIT_LONG )
coll.setFlag( flag.getFlag() )
evt = lcio.LCEventImpl()
hit = lcio.SimCalorimeterHitImpl()
hit.setPosition((1.1, 1.2, 1.3))
print hit.getPosition()
coll.addElement(hit)

#print "Collecting garbage ..."
#gc.collect()
#print "Done collecting garbage."

evt.addCollection(coll, "test_collection")
writer.writeEvent(evt)
writer.close()
print "Finished with LCWriter test"

coll = None

print "Collecting garbage ..."
gc.collect()
print "Done collecting garbage."


