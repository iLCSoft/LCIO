'''
Created on Dec 4, 2012

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

from __future__ import absolute_import, unicode_literals
from __future__ import print_function
from ROOT import gSystem
import os, sys

class RootDictLoadError(Exception):
    """Raised if gSystem.Load cannot successfully load a shared lib/dict"""

def try_load_lib(lib_path):
    """Try and load the passed library via gSystem"""
    if gSystem.Load(lib_path) not in [0, 1]:
        raise RootDictLoadError


def setupLcioDictionary():
    '''Automatically loads the ROOT LCIO dictionaries. First looks for the
    corresponding libraries on LD_LIBRARY_PATH. If that doesn't succeed falls
    back to using the LCIO environment variable to try and locate them.
    '''
    print('Loading LCIO ROOT dictionaries ...')
    try:
        # First try via ROOT and automatic lookup on LD_LIBRARY_PATH
        try_load_lib('liblcio.so')
        try_load_lib('liblcioDict.so')
        return
    except RootDictLoadError:
        pass

    print('Could not load liblcio.so or liblcioDict.so from LD_LIBRARY_PATH. Falling back to search via LCIO environment variable')
    lcioPath = os.environ.get( 'LCIO' )
    if not lcioPath:
        print('Environment variable $LCIO is not defined!', file=sys.stderr)
        sys.exit( 2 )
    if not os.path.exists( lcioPath ):
        print('LCIO path %s does not exist' % (lcioPath))
        sys.exit( 2 )

    for lib_dir in ['lib', 'lib64']:
        try:
            try_load_lib(os.path.join(lcioPath, lib_dir, 'liblcio.so'))
            try_load_lib(os.path.join(lcioPath, lib_dir, 'liblcioDict.so'))
            return
        except RootDictLoadError:
            pass

    # If now we are still here, we have a problem
    print('Error loading dictionaries from liblcio.so and liblcioDict.so. Could not find them in lib or lib64 in {}'.format(lcioPath))
    sys.exit(2)
