'''
Created on Dec 4, 2012

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

from ROOT import gSystem
import os, sys

def setupLcioDictionary():
    ''' Automatically loads the ROOT LCIO dictionaries. Requires the environment variable $LCIO to be set to the LCIO installation directory. '''
    
    print('Loading LCIO ROOT dictionaries ...')
    lcioPath = os.environ.get( 'LCIO' )
    if not lcioPath:
        print('Environment variable $LCIO is not defined!', file=sys.stderr)
        sys.exit( 2 )
    if not os.path.exists( lcioPath ):
        print('LCIO path %s does not exist' % ( lcioPath ))
        sys.exit( 2 )
    liblcioPath = os.path.join( lcioPath, 'lib', 'liblcio.so' )
    # See http://root.cern.ch/root/html/TSystem.html#TSystem:Load for error codes
    result = gSystem.Load( liblcioPath )
    if result not in [0, 1]:
        print('Error loading %s' % ( liblcioPath ), file=sys.stderr)
        sys.exit( 2 )
    liblcioDictPath = os.path.join( lcioPath, 'lib', 'liblcioDict.so' )
    # See http://root.cern.ch/root/html/TSystem.html#TSystem:Load for error codes
    result = gSystem.Load( liblcioDictPath )
    if result not in [0, 1]:
        print('Error loading %s' % ( liblcioDictPath ), file=sys.stderr)
        sys.exit( 2 )