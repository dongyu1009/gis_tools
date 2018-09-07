#!/bin/bash

mkdir -p ./output
filenames=`ls |grep .tif`
for filename in $filenames
do
    echo $filename
    gdalwarp -of GTiff -dstnodata -999 -cutline ../lucc_data_merge/output/bound/MongoliaPlateau_Overall.shp -crop_to_cutline -co COMPRESS=LZW $filename ./output/$filename
done
