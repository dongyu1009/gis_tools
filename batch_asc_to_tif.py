#*****************************************************************
# Filename    : batch_asc_to_tif.py
# Author      : Dong Yu
# E-mail      : dongyu1009@163.com
# Date        : 20180501
# Description : batch program to convert ascii file to tif file
# Version     : 0.1
# Build       : 1
# MIT License. Programed by Dong Yu 2018
#*****************************************************************
import arcpy
import arcpy.sa
import os
from arcpy.sa import *

if __name__ == "__main__":

	arcpy.CheckOutExtension("Spatial")

	for year in range(1948, 2011):
		inputfilename = "E:\\npp_p\\npp\\origin\\" + str(year) + "\\tnpy_" + str(year) + ".asc"
		outputfilename = "E:\\npp_p\\npp\\res\\npp" + str(year) + ".tif"
		print inputfilename
		# in_raster = Raster(inputfilename)
		arcpy.ASCIIToRaster_conversion(inputfilename, outputfilename, "FLOAT")

    