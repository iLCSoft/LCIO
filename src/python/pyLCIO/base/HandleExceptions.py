'''
Created on Nov 5, 2013

Module to add handling of LCIO C++ exception in method calls.
If an LCIO exception is encountered, the corresponding python exception is raised. 

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

import inspect
import ROOT
import pyLCIO
import pyLCIO.exceptions.Exceptions

# Cope with API change with ROOT version >= 6.32
if ROOT.gROOT.GetVersionCode() >= (6<<16) + (32<<8) :
  from cppyy.types import Function as CPPOverload
# Cope with API change with ROOT version >= 6.21
elif ROOT.gROOT.GetVersionCode() >= (6<<16) + (21<<8) :
  # libcppyy has been moved in ROOT 6.38 and can no longer be imported
  from libcppyy import CPPOverload
else:
  CPPOverload = ROOT.MethodProxy 

# helper method to handle LCIO exceptions in a method call
def handleLcioExceptions(method):
    def wrappedMethod(*args, **kargs):
        try:
            return method(*args, **kargs)
        except Exception as e:
            message = str(e)
            if 'lcio::Exception' in message:
                message = message.split('lcio::Exception: ')[1].split(' (C++ exception)')[0]
                raise pyLCIO.LcioException(message) from e
            elif 'lcio::EventException' in message:
                message = message.split('lcio::EventException: ')[1].split(' (C++ exception)')[0]
                raise pyLCIO.EventException(message) from e
            elif 'lcio::DataNotAvailableException' in message:
                message = message.split('lcio::DataNotAvailableException: ')[1].split(' (C++ exception)')[0]
                raise pyLCIO.DataNotAvailableException(message) from e
            elif 'lcio::ReadOnlyException' in message:
                message = message.split('lcio::ReadOnlyException: ')[1].split(' (C++ exception)')[0]
                raise pyLCIO.ReadOnlyException(message) from e
            elif 'lcio::IOException' in message:
                message = message.split('lcio::IOException: ')[1].split(' (C++ exception)')[0]
                raise pyLCIO.IOException(message) from e
            elif 'lcio::EndOfDataException' in message:
                message = message.split('lcio::EndOfDataException: ')[1].split(' (C++ exception)')[0]
                raise pyLCIO.EndOfDataException(message) from e
            raise
    return wrappedMethod

def setupExceptionHandling():
    # add the exception classes to the package level
    for name, object in vars(pyLCIO.exceptions.Exceptions).items():
        if inspect.isclass(object):
            setattr(pyLCIO, name, object)

    # replace all method calls with wrapped method calls
    for namespace in pyLCIO.namespaces:
        module = getattr(pyLCIO, namespace)
        for name, object in vars(module).items():
            if inspect.isclass(object):
                for methodName, method in vars(object).items():
                    if isinstance(method, CPPOverload):
                        # can not wrap singleton calls
                        if methodName not in ["getInstance"]:
                            setattr(object, methodName, handleLcioExceptions(method))
