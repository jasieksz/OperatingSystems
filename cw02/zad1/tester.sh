#!/bin/bash
cd /home/student/Desktop/OperatingSystems/cw02/bin/zad1
echo "=======================BEGINING==========================" > out.txt
./program lib generate data1 5000 4
cp data1 data2
echo "===============5000 records 4 bytes lib==================" >> out.txt
./program lib sort data1 5000 4 >> out.txt
./program lib shuffle data1 5000 4 >> out.txt
echo "===============5000 records 4 bytes sys==================" >> out.txt
./program lib sort data2 5000 4 >> out.txt
./program lib shuffle data2 5000 4 >> out.txt

./program lib generate data3 5000 512
cp data3 data4
echo "===============5000 records 512 bytes lib================" >> out.txt

./program lib sort data3 5000 512 >> out.txt
./program lib shuffle data3 5000 512 >> out.txt
echo "===============5000 records 512 bytes sys================" >> out.txt
./program lib sort data4 5000 512 >> out.txt
./program lib shuffle data4 5000 512 >> out.txt

./program lib generate data5 5000 4096
cp data5 data6
echo "===============5000 records 4096 bytes lib===============" >> out.txt

./program lib sort data5 5000 4096 >> out.txt
./program lib shuffle data5 5000 4096 >> out.txt
echo "===============5000 records 4096 bytes sys===============" >> out.txt
./program lib sort data6 5000 4096 >> out.txt
./program lib shuffle data6 5000 4096 >> out.txt

./program lib generate data7 5000 8192
cp data7 data8
echo "===============5000 records 8192 bytes lib===============" >> out.txt

./program lib sort data7 5000 8192 >> out.txt
./program lib shuffle data7 5000 8192 >> out.txt
echo "===============5000 records 8192 bytes sys===============" >> out.txt
./program lib sort data8 5000 8192 >> out.txt
./program lib shuffle data8 5000 8192 >> out.txt

echo "=========================END=============================" >> out.txt