#!/usr/bin/env bash

echo "Building Bohrium"
pushd /tmp
wget https://github.com/bh107/bohrium/archive/master.zip
unzip master.zip
mkdir bh_build
pushd bh_build
cmake -DCMAKE_BUILD_TYPE=Release -DBRIDGE_CIL=OFF -DBRIDGE_NUMCIL=OFF -DFILTER_FUSEPRINTER=OFF -DFILTER_PPRINT=OFF -DVEM_CLUSTER=OFF -DVEM_PROXY=OFF -DEXT_VISUALIZER=OFF -DVE_GPU=OFF -DTEST_CIL=OFF -DBENCHMARK_CIL=OFF ../bohrium-master
make -j4
make install
popd
popd
