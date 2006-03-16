import lcio

fac = lcio.LCFactory.getInstance()
rdr = fac.createLCReader()
rdr.open("write_test.slcio")
evt = rdr.readNextEvent()
coll = evt.getSimCalorimeterHitCollection("hits")
print repr(coll)
hit=coll.getElementAt(0)
print repr(hit)
