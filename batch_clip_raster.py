#*****************************************************************
# Filename    : batch_clip_raster.py
# Author      : Dong Yu
# E-mail      : dongyu1009@163.com
# Date        : 20180501
# Description : batch program to clip raster file
# Version     : 0.1
# Build       : 1
# License MIT. 
#*****************************************************************
import arcpy
import arcpy.sa
import os

if __name__ == "__main__":
    # target spatial reference
    arcpy.env.workspace = ".\\res\\"
    out_location = ".\\cliped\\"
    
    # Use Python's built-in function len to reveal the number of feature classes
    # in the workspace
    rasterlist = arcpy.ListRasters()

    # fcCount = len(rasterlist)
    for fc in rasterlist:
        print fc
        arcpy.Clip_management(fc, "#", out_location + fc, "..\\border\\clip.shp", 32767, "ClippingGeometry")
