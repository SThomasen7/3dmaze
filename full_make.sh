#!/bin/bash

rm -f test maze
rm -rf test_output
rm -rf build
rm -rf __pycache__
mkdir -p build
cmake -S . -B build
cmake --build build
cmake --build build --target tags
cp build/test .
cp build/maze .

