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

  $CYGLCIO/bin/simjob c_sim.slcio
  $CYGLCIO/bin/recjob c_sim.slcio c_rec.slcio
  $CYGLCIO/bin/anajob c_rec.slcio

  $CYGLCIO/bin/runSimJob.sh j_sim.slcio
  $CYGLCIO/bin/runRecJob.sh j_sim.slcio j_rec.slcio
  $CYGLCIO/bin/runAnalysisJob.sh j_rec.slcio

  $CYGLCIO/bin/recjob j_sim.slcio j2c_rec.slcio
  $CYGLCIO/bin/runRecJob.sh c_sim.slcio c2j_rec.slcio


  $CYGLCIO/bin/anajob c2j_rec.slcio
  $CYGLCIO/bin/runAnalysisJob.sh j2c_rec.slcio

else

  $LCIO/bin/simjob c_sim.slcio
  $LCIO/bin/recjob c_sim.slcio c_rec.slcio
  $LCIO/bin/anajob c_rec.slcio

  $LCIO/bin/runSimJob.sh j_sim.slcio
  $LCIO/bin/runRecJob.sh j_sim.slcio j_rec.slcio
  $LCIO/bin/runAnalysisJob.sh j_rec.slcio

  $LCIO/bin/recjob j_sim.slcio j2c_rec.slcio
  $LCIO/bin/runRecJob.sh c_sim.slcio c2j_rec.slcio


  $LCIO/bin/anajob c2j_rec.slcio
  $LCIO/bin/runAnalysisJob.sh j2c_rec.slcio

fi

