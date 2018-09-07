#!/bin/bash

mkdir -r ./output

filenames=`find ./ -name *.tif`
for i in $filenames
do
    # srs_origin=`gdalsrsinfo -o wkt $i`
    filename=${i##*/}
    echo `processing:$filename`
    gdalwarp -overwrite $i ./output/$filename -t_srs EPSG:4326
done


