#!/usr/bin/env bash

echo "Building Bohrium"
get https://github.com/bh107/bohrium/archive/master.zip
unzip master.zip
mkdir bohrium
pushd bohrium
cmake -DCMAKE_BUILD_TYPE=Release ../bohrium-master
make -j4
make install
popd
