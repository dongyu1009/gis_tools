#*****************************************************************
# Filename    : batch_statistics_reader.py
# Author      : Dong Yu
# E-mail      : dongyu1009@163.com
# Date        : 20180501
# Description : read a statistics of raster and to write to a file
# Version     : 0.1
# Build       : 1
# 2018 Dong Yu. MIT License.
#*****************************************************************
import arcpy
import arcpy.sa
import os
from arcpy.sa import *

if __name__ == "__main__":

	arcpy.CheckOutExtension("Spatial")
	arcpy.env.workspace = ".\\GIMMS\\GIMMS_GRASS\\"
	rasterlist = arcpy.ListRasters()
	
	file_object = open('stat_grass.csv', 'w')
	# file_object = open('stat_grass_out.csv', 'w')
	
	strCurrentLine = "name,max,min,mean,std\n"
	file_object.writelines(strCurrentLine)
	for fc in rasterlist:
		print fc
		fc_raster = Raster(fc)
		max = fc_raster.maximum
		min = fc_raster.minimum
		mean = fc_raster.mean
		std = fc_raster.standardDeviation
		strCurrentLine = "" + fc + "," + str(max) + "," + str(min) + "," + str(mean) + "," + str(std) + "\n"
		file_object.writelines(strCurrentLine)
    
	file_object.close()
    
