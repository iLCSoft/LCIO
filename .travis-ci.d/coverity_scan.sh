#!/bin/bash

source /Package/.travis-ci.d/init_x86_64.sh

cd /Package
mkdir build
cd build
cmake -DCMAKE_CXX_STANDARD=${STANDARD} -DBUILD_ROOTDICT=ON .. && \
export PATH=/Package/cov-analysis-linux64/bin:$PATH && \
cov-build --dir cov-int make -j2 && \
tar czvf myproject.tgz cov-int
