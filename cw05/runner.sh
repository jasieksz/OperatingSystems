#!/bin/bash

rm -r bin
rm -r build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../bin .. 
make
make install

#cp /home/student/Desktop/OperatingSystems/cw05/in /home/student/Desktop/OperatingSystems/cw05/bin/zad1


