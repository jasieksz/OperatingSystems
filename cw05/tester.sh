#!/bin/bash

cd bin/zad2

touch pip

./slave pip 1000000 100
./master pip 600