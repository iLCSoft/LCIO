#!/usr/bin/python

import lcio
import gc

print dir(lcio)

# Factory
fac=lcio.LCFactory.getInstance()
print repr(fac)

# LCWriter
writer=fac.createLCWriter()
writer.open("write_test", lcio.LCIO.WRITE_NEW)

# write out 10k events
nevents = 1
for i in range(0,nevents):

#    coll = lcio.LCCollectionVec(lcio.cvar.LCIO_RAWCALORIMETERHIT)
    coll = lcio.LCCollectionVec(lcio.cvar.LCIO_SIMCALORIMETERHIT)
    coll.thisown = 0 # set C++ ownership

    #flag = lcio.LCFlagImpl()
    #flag.setBit( lcio.LCIO.CHBIT_LONG )
    #coll.setFlag( flag.getFlag() )
    #print "set coll flags"

    for i in range(0,1):
        hit = lcio.SimCalorimeterHitImpl()
#        hit  = lcio.RawCalorimeterHitImpl()

#        hit.setAmplitude(1)
        hit.thisown = 0 # set C++ ownership
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
