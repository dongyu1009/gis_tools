#! /bin/bash

g++ $1 -lgdal -std=c++11 -o out
./out
rm -f out
