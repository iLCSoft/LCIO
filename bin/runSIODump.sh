#!/bin/sh

LOCALCLASSPATH=$LCIO/lib/lcio.jar:$LCIO/tools/sio.jar:$LCIO/tools/jdom.jar:$LCIO/tools/saxpath.jar
LOCALCLASSPATH=$LOCALCLASSPATH:$LCIO/tools/freehep-sio-2.1-SNAPSHOT.jar:$LCIO/tools/freehep-xdr-2.0.4-SNAPSHOT.jar
LOCALCLASSPATH=$LOCALCLASSPATH:$LCIO/tools/jaxen-jdom.jar:$LCIO/tools/jaxen-core.jar:$LCIO/tools/jel.jar

# OS specific support for Cygwin 
cygwin=false;
case "`uname`" in
  CYGWIN*) cygwin=true ;;
esac
# For Cygwin, switch paths to Windows format before running java
if $cygwin; then
  LOCALCLASSPATH=`cygpath --path --windows "$LOCALCLASSPATH"`
  java -cp "$LOCALCLASSPATH" hep.lcio.util.SIODump $*
else
  java -cp $LOCALCLASSPATH hep.lcio.util.SIODump $*
fi

