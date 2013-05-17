#!/bin/sh
setenv LCIO `pwd`
setenv PATH $LCIO/bin:$LCIO/tools:$PATH
setenv PYTHONPATH $LCIO/src/python:$PYTHONPATH
alias pylcio='python $LCIO/src/python/pylcio.py'
