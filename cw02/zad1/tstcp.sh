#!/bin/bash
cd /home/jasiek/Documents/AGH/6_Semestr/sysopy/2017/OperatingSystems/cw02/bin/zad1
echo "=======================BEGINING==========================" > out.txt
./program lib generate data1 5000 4
echo "===============5000 records 4 bytes lib==================" >> out.txt
./program lib copy data1 data2 5000 4 >> out.txt
./program lib sort data2 5000 4 >> out.txt
echo "===============5000 records 4 bytes sys==================" >> out.txt
./program sys copy data1 data3 5000 4 >> out.txt
./program sys sort data3 5000 4 >> out.txt
###########################################################################
./program lib generate data4 5000 512
echo "===============5000 records 512 bytes lib==================" >> out.txt
./program lib copy data4 data5 5000 512 >> out.txt
./program lib sort data5 5000 512 >> out.txt
echo "===============5000 records 512 bytes sys==================" >> out.txt
./program sys copy data4 data6 5000 512 >> out.txt
./program sys sort data6 5000 512 >> out.txt
############################################################################
./program lib generate data7 5000 4096
echo "===============5000 records 4096 bytes lib==================" >> out.txt
./program lib copy data7 data8 5000 4096 >> out.txt
./program lib sort data8 5000 4096 >> out.txt
echo "===============5000 records 4096 bytes sys==================" >> out.txt
./program sys copy data7 data9 5000 4096 >> out.txt
./program sys sort data9 5000 4096 >> out.txt
############################################################################
./program lib generate data10 5000 8192
echo "===============5000 records 8192 bytes lib==================" >> out.txt
./program lib copy data01 data11 5000 8192 >> out.txt
./program lib sort data11 5000 8192 >> out.txt
echo "===============5000 records 8192 bytes sys==================" >> out.txt
./program sys copy data10 data12 5000 8192 >> out.txt
./program sys sort data12 5000 8192 >> out.txt
echo "=========================END=============================" >> out.txt
