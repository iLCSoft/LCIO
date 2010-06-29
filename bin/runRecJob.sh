#!/bin/sh

LOCALCLASSPATH=$LCIO/lib/lcio.jar
LOCALCLASSPATH=$LOCALCLASSPATH:$LCIO/tools/freehep-sio-2.1.jar:$LCIO/tools/freehep-xdr-2.0.4.jar

# OS specific support for Cygwin 
cygwin=false;
case "`uname`" in
  CYGWIN*) cygwin=true ;;
esac
# For Cygwin, switch paths to Windows format before running java
if $cygwin; then
  LOCALCLASSPATH=`cygpath --path --windows "$LOCALCLASSPATH"`
  java -cp "$LOCALCLASSPATH" hep.lcio.example.RecJob $1 $2
else
  java -cp $LOCALCLASSPATH hep.lcio.example.RecJob $1 $2
fi

