#!/bin/sh

LOCALCLASSPATH=$LCIO/lib/lcio.jar:$LCIO/tools/freehep-sio-2.1.jar:$LCIO/tools/freehep-xdr-2.0.4.jar

case "`uname`" in
  CYGWIN*)
    # For Cygwin, switch paths to Windows format before running java
    LOCALCLASSPATH=`cygpath --path --windows "$LOCALCLASSPATH"`
    LCIO=`cygpath --path --windows "$LCIO"`
  ;;
esac

java -cp $LOCALCLASSPATH hep.lcio.test.RandomEventTest
java -cp $LOCALCLASSPATH hep.lcio.test.CreateRandomEvent rand_java.slcio
$LCIO/bin/copyfix rand_java.slcio rand_cpp.slcio
java -cp $LOCALCLASSPATH hep.lcio.test.CompareEvents rand_java.slcio rand_cpp.slcio
