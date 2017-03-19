#!/bin/bash
cd bin

echo "====================BEGINING===================" > out.txt
./program lib generate data1 1000 4
cp data1 data2
echo "===============1000 records 4 bytes lib========" >> out.txt

./program lib sort data1 1000 4 >> out.txt
./program lib shuffle data1 1000 4 >> out.txt
echo "===============1000 records 4 bytes sys========" >> out.txt
./program lib sort data2 1000 4 >> out.txt
./program lib shuffle data2 1000 4 >> out.txt

./program lib generate data3 1000 512
cp data3 data4
echo "===============1000 records 512 bytes lib========" >> out.txt

./program lib sort data1 1000 512 >> out.txt
./program lib shuffle data1 1000 512 >> out.txt
echo "===============1000 records 512 bytes sys========" >> out.txt
./program lib sort data2 1000 512 >> out.txt
./program lib shuffle data2 1000 512 >> out.txt

./program lib generate data5 1000 4096
cp data5 data6
echo "===============1000 records 4096 bytes lib========" >> out.txt

./program lib sort data1 1000 4096 >> out.txt
./program lib shuffle data1 1000 4096 >> out.txt
echo "===============1000 records 4096 bytes sys========" >> out.txt
./program lib sort data2 1000 4096 >> out.txt
./program lib shuffle data2 1000 4096 >> out.txt
