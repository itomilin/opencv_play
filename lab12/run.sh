#!/usr/bin/env bash

cd ./build-untitled-Desktop_Qt_6_1_2_GCC_64bit-Release

# boostring очень точный, медленный.
# medianflow менее точный, быстрый.
for i in {0..7}
do
    sleep 1
    ./untitled $i
done

