#!/bin/bash

source /cvmfs/sft.cern.ch/lcg/views/${VIEW}/setup.sh
cd /Package

echo "RUNNING PYLINT PY3K CHECK"
find . -name "*.py" -and -not -name 'sixlcio.py' -exec pylint --rcfile=.github/scripts/lcio.pylint.py3k.rc --py3k --msg-template="{path}:{line}: [{msg_id}({symbol}), {obj}] {msg}" {} +

