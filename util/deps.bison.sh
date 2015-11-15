#!/usr/bin/env bash

V=3.0.4

echo "Building Bison"
pushd /tmp
wget http://ftp.gnu.org/gnu/bison/bison-$V.tar.gz
tar -x -z -f bison-$V.tar.gz
cd bison-$V && ./configure --prefix=/tmp/tools/bison && make && make install
rm -rf bison-$V
export PATH=/tmp/tools/bison/bin:$PATH
popd
