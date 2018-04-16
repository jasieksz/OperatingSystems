#!/bin/bash
cd /home/jasiek/Documents/AGH/6_Semestr/sysopy/2017/OperatingSystems/cw02/bin/zad1
echo "=======================BEGINING==========================" > out.txt
###########################################################################
./program lib generate data4 5000 512
echo "===============5000 records 512 bytes lib==================" >> out.txt
./program lib copy data4 data5 5000 512 >> out.txt
./program lib sort data5 5000 512 >> out.txt
echo "===============5000 records 512 bytes sys==================" >> out.txt
./program sys copy data4 data6 5000 512 >> out.txt
./program sys sort data6 5000 512 >> out.txt
############################################################################
echo "=========================END=============================" >> out.txt
