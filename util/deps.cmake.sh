#!/usr/bin/env bash

VER=3.4.0

echo "Building cmake"
pushd /tmp
wget http://cmake.org/files/v3.4/cmake-$VER.tar.gz
tar xzf cmake-$VER.tar.gz
pushd cmake-$VER && ./configure --prefix=/tmp/tools/cmake && make && make install
export PATH=/tmp/tools/cmake/bin:$PATH
popd
popd
