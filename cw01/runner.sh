#!/bin/bash

rm -r bin
rm -r build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../bin .. -DOPTIMIZATION=2 #set optimization level
make
make install

