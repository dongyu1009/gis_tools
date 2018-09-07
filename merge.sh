#!/bin/bash

# python /usr/bin/gdal_merge.py -o merged.tif -n 0 `find ./ -name *.tif`
python /usr/bin/gdal_merge.py -o merged.tif -n 0 `ls |grep tif`
