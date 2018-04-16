#!/bin/bash

rm -r bin
rm -r build
mkdir build
cd build
/usr/lib/CLion-2017.3.3/clion-2017.3.3/bin/cmake/bin/cmake -DCMAKE_INSTALL_PREFIX=../bin .. 
make
make install

#cp /home/student/Desktop/OperatingSystems/cw05/in /home/student/Desktop/OperatingSystems/cw05/bin/zad1


