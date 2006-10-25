import lcio
import gc

v = lcio.LCCollectionVec(lcio.GLOBALS.LCIO_SIMCALORIMETERHIT)
v = None
gc.collect()

e = lcio.LCEventImpl()
e = None
gc.collect()
