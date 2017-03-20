#!/bin/bash
cd /home/student/Desktop/OperatingSystems/cw02/bin/zad2
echo "======================NFTW PROGRAM=======================" > out1.txt
./nftwprogram /home/student/Desktop/OperatingSystems 25 >> out1.txt
echo "====================REGULAR PROGRAM=======================" > out2.txt
./normprogram /home/student/Desktop/OperatingSystems 25 >> out2.txt