#!/usr/bin/python

import lcio
import gc

#print dir(lcio)

# Factory
fac=lcio.LCFactory.getInstance()
#print repr(fac)

# LCWriter
writer=fac.createLCWriter()
writer.open("write_test", lcio.LCIO.WRITE_NEW)

# write out 10k events
nevents = 10
for i in range(0,nevents):

    coll = lcio.LCCollectionVec(lcio.GLOBALS.LCIO_SIMCALORIMETERHIT)

    for i in range(0,1):
        hit = lcio.SimCalorimeterHitImpl()
        coll.addElement(hit)

    evt = lcio.LCEventImpl()
    evt.setRunNumber(0)
    evt.setEventNumber(0)

    evt.addCollection(coll, "hits")

    writer.writeEvent(evt)
    writer.flush()
    evt = None

    coll = None

    gc.collect()

writer.close()

gc.collect()

print "Finished with LCWriter test."
