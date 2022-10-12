#!/bin/bash

mkdir build && cd build

# CPU
#cmake -DUSE_CPU=ON .. && make
# GPU
cmake -DUSE_GPU=ON .. && make

