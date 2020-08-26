'''
Created on Nov 5, 2013

Module to add handling of LCIO C++ exception in method calls.
If an LCIO exception is encountered, the corresponding python exception is raised. 

@author: <a href="mailto:christian.grefe@cern.ch">Christian Grefe</a>
'''

import inspect, sys, ROOT, pyLCIO
import pyLCIO.exceptions.Exceptions

# helper method to handle LCIO exceptions in a method call
def handleLcioExceptions(method):
    def wrappedMethod(*args, **kargs):
        try:
            return method(*args, **kargs)
        except Exception as e:
            message = e.message
            if 'lcio::Exception' in message:
                message = message.split('lcio::Exception: ')[1].split(' (C++ exception)')[0]
                raise pyLCIO.LcioException(message).with_traceback(sys.exc_info()[2])
            elif 'lcio::EventException' in message:
                message = message.split('lcio::EventException: ')[1].split(' (C++ exception)')[0]
                raise pyLCIO.EventException(message).with_traceback(sys.exc_info()[2])
            elif 'lcio::DataNotAvailableException' in message:
                message = message.split('lcio::DataNotAvailableException: ')[1].split(' (C++ exception)')[0]
                raise pyLCIO.DataNotAvailableException(message).with_traceback(sys.exc_info()[2])
            elif 'lcio::ReadOnlyException' in message:
                message = message.split('lcio::ReadOnlyException: ')[1].split(' (C++ exception)')[0]
                raise pyLCIO.ReadOnlyException(message).with_traceback(sys.exc_info()[2])
            elif 'lcio::IOException' in message:
                message = message.split('lcio::IOException: ')[1].split(' (C++ exception)')[0]
                raise pyLCIO.IOException(message).with_traceback(sys.exc_info()[2])
            elif 'lcio::EndOfDataException' in message:
                message = message.split('lcio::EndOfDataException: ')[1].split(' (C++ exception)')[0]
                raise pyLCIO.EndOfDataException(message).with_traceback(sys.exc_info()[2])
            raise
    return wrappedMethod

def setupExceptionHandling():
    # add the exception classes to the package level
    for name, object in list(vars(pyLCIO.exceptions.Exceptions).items()):
        if inspect.isclass(object):
            setattr(pyLCIO, name, object)

    # replace all method calls with wrapped method calls
    for namespace in pyLCIO.namespaces:
        module = getattr(pyLCIO, namespace)
        for name, object in list(vars(module).items()):
            if inspect.isclass(object):
                for methodName, method in list(vars(object).items()):
                    if isinstance(method, ROOT.MethodProxy):
                        # can not wrap singleton calls
                        if methodName not in ["getInstance"]:
                            setattr(object, methodName, handleLcioExceptions(method))
                        