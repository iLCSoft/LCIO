#!/bin/sh
################### script to run the pysimjob_F executable ###################
#                                                                             #
#                   output files: pythia.data, pysimjob_F.log                 #
#                                                                             #
###############################################################################

# creates pythia.data and pythiahepevt.log (needs cernlib 2002 or higher)
pythiahepevt.sh

# creates pysimjob.slcio and pysimjob_F.log
rm -f pysimjob_F.log pysimjob.slcio
pysimjob_F > pysimjob_F.log

