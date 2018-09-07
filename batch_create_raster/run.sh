##! /bin/bash

g++ generator.cpp -lgdal -std=c++11 -o run
chmod 744 ./run
./run
rm -f ./run 
echo "program finished!"
