#!/bin/bash

cd bin
export LD_LIBRARY_PATH=.
./test_shared >> results_shared.txt
./test_static >> results_static.txt
./test_dynamic >> results_dynamic.txt