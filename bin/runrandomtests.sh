#!/bin/sh

LOCALCLASSPATH=$LCIO/lib/lcio.jar:$LCIO/tools/sio.jar

# OS specific support for Cygwin 
cygwin=false;
case "`uname`" in
  CYGWIN*) cygwin=true ;;
esac
# For Cygwin, switch paths to Windows format before running java
if $cygwin; then
  LOCALCLASSPATH=`cygpath --path --windows "$LOCALCLASSPATH"`
  java -cp "$LOCALCLASSPATH" hep.lcio.test.RandomEventTest
  java -cp "$LOCALCLASSPATH" hep.lcio.test.CreateRandomEvent rand_java.slcio
  ./bin/copyfix rand_java.slcio rand_cpp.slcio
  java -cp "$LOCALCLASSPATH" hep.lcio.test.CompareEvents rand_java.slcio rand_cpp.slcio
else
  java -cp $LOCALCLASSPATH hep.lcio.test.RandomEventTest
  java -cp $LOCALCLASSPATH hep.lcio.test.CreateRandomEvent rand_java.slcio
  ./bin/copyfix rand_java.slcio rand_cpp.slcio
  java -cp $LOCALCLASSPATH hep.lcio.test.CompareEvents rand_java.slcio rand_cpp.slcio
fi

