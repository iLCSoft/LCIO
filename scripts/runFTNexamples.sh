#!/bin/sh

#
# run all example C++ and Java for testing 
#  (output has to be checked manually)
#
# OS specific support for Cygwin 

cygwin=false;
case "`uname`" in
  CYGWIN*) cygwin=true ;;
esac
# For Cygwin, switch paths to Windows format before running java
if $cygwin; then

  CYGLCIO=`cygpath --path --windows "$LCIO"`

  $CYGLCIO/bin/simjob_chain_F
  $CYGLCIO/bin/simjob_F
  $CYGLCIO/bin/recjob_F
  $CYGLCIO/bin/anajob_F f_recjob.slcio

else

  $LCIO/bin/simjob_chain_F
  $LCIO/bin/simjob_F
  $LCIO/bin/recjob_F
  $LCIO/bin/anajob_F f_recjob.slcio

fi

