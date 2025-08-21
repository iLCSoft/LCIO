from .base.SetupLcioDictionary import setupLcioDictionary
setupLcioDictionary()

import inspect, sys, ROOT, pyLCIO

namespaces = ["EVENT", "IMPL", "IO", "IOIMPL", "UTIL"]
for namespace in namespaces:
    module = getattr( ROOT, namespace )
    setattr(pyLCIO, namespace, module)
    sys.modules['pyLCIO.' + namespace] = module

from .base.DecorateLcioClasses import decorateLcioClasses
decorateLcioClasses()

from .base.HandleExceptions import setupExceptionHandling
setupExceptionHandling()
