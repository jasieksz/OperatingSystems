#!/bin/bash

rm -r bin
rm -r build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../bin ..
make
make install
sleep 1
../bin/zad2/generator2 ../bin/zad2/in 10
