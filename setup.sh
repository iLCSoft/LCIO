#!/bin/sh

#
# initialize the current LCIO version by sourcing this in the top level directory
#

export LCIO=$(pwd)
export PATH=$LCIO/bin:$LCIO/tools:$PATH
for d in lib lib64; do
    if [ -d ${LCIO}/${d} ]; then
        export LD_LIBRARY_PATH=${LCIO}/${d}:${LD_LIBRARY_PATH}
    fi
done
export PYTHONPATH=$LCIO/python:$PYTHONPATH
alias pylcio='python $LCIO/python/pylcio.py'
# Necessary for python bindings to work properly
export CPATH=$LCIO:$LCIO/include:$CPATH
