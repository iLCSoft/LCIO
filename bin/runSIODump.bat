@echo off
java -cp "%LCIO%/lib/lcio.jar;%LCIO%/tools/freehep-sio-2.1.jar;%LCIO%/tools/freehep-xdr-2.0.4.jar;%LCIO%/tools/jdom.jar;%LCIO%/tools/saxpath.jar;%LCIO%/tools/jaxen-jdom.jar;%LCIO%/tools/jaxen-core.jar;%LCIO%/tools/jel.jar" hep.lcio.util.SIODump %*
