#
# run all example C++ and Java for testing 
#  (output has to be checked manually)
#

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


