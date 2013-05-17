#!/bin/sh
export LCIO=`pwd`
export PATH=$LCIO/bin:$LCIO/tools:$PATH
export PYTHONPATH=$LCIO/src/python:$PYTHONPATH
alias pylcio='python $LCIO/src/python/pylcio.py'
