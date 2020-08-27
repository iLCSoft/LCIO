from pyLCIO.IOIMPL import LCFactory
from pyLCIO import IOException

# create a reader and open an LCIO file
factory = LCFactory.getInstance().createLCReader()
try:
    reader.open( 'file_does_not_exist.slcio' )
except IOException as e:
    print("Caught exception:", e)
