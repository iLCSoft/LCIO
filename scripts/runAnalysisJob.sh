#!/bin/sh

LOCALCLASSPATH=$LCIO/lib/lcio.jar:$LCIO/tools/freehep-sio-2.1.jar:$LCIO/tools/freehep-xdr-2.0.4.jar

# OS specific support for Cygwin 
cygwin=false;
case "`uname`" in
  CYGWIN*) cygwin=true ;;
esac
# For Cygwin, switch paths to Windows format before running java
if $cygwin; then
  LOCALCLASSPATH=`cygpath --path --windows "$LOCALCLASSPATH"`
  java $JVM_ARGS -cp "$LOCALCLASSPATH" hep.lcio.example.AnalysisJob $@
else
  java $JVM_ARGS -cp $LOCALCLASSPATH hep.lcio.example.AnalysisJob $@
fi

