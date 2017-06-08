#!/bin/bash

rm -r bin
rm -r build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../bin .. 
make
make install
sleep 2
../bin/zad1/generator ../bin/zad1/in 20


