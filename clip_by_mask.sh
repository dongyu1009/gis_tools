#!/bin/bash

gdalwarp -ot Byte -of GTiff -cutline ./bound/MongoliaPlateau_Overall.shp -crop_to_cutline -co COMPRESS=LZW ./merged.tif ./mongolia.tif
