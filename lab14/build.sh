#!/bin/bash
set -e

mkdir build && cd build

MODE=$1
if [[ -z $MODE ]]; then
    echo "[ ERROR ] First arg must be a mode (GPU|CPU)."
    exit 1
fi

if [[ $MODE == "CPU" ]]; then
    cmake -DUSE_CPU=ON .. && make
elif [[ $MODE == "GPU" ]]; then
    cmake -DUSE_GPU=ON .. && make
else
    echo "[ ERROR ] Not handled mode."
    exit 1
fi

