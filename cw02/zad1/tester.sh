#!/bin/bash
cd bin/zad1

echo "====================BEGINING===================" > out.txt
echo "===============1000 records 4 bytes lib========" >> out.txt

./program lib generate data1 1000 4
cp data1 data2

./program lib sort data1 1000 4 >> out.txt
./program lib shuffle data1 1000 4 >> out.txt
echo "===============1000 records 4 bytes sys========" >> out.txt
./program lib sort data2 1000 4 >> out.txt
./program lib shuffle data2 1000 4 >> out.txt

