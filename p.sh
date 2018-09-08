#!/bin/bash

g++ $1 -o run -lgdal -std=c++11
./run
rm -rf run



