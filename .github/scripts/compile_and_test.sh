#!/bin/bash

source /cvmfs/sft.cern.ch/lcg/views/${VIEW}/setup.sh

cd /Package
mkdir build
cd build
cmake -GNinja -DCMAKE_CXX_STANDARD=${STANDARD} -DBUILD_ROOTDICT=ON -DCMAKE_CXX_FLAGS="-fdiagnostics-color=always" .. && \
ninja  -k 0 && \
ninja install && \
ctest --output-on-failure
