#!/usr/bin/env bash

VER=3.0.4

echo "Building Bison"
pushd /tmp
wget http://ftp.gnu.org/gnu/bison/bison-$VER.tar.gz
tar xzf bison-$VER.tar.gz
pushd bison-$VER && ./configure --prefix=/tmp/tools/bison  && make && make install
export PATH=/tmp/tools/bison/bin:$PATH
popd
popd
