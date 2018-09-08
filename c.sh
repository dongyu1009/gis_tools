#!/bin/bash

gcc $1 -o out
./out
rm -f out
