#*****************************************************************
# Filename    : gimms_define_projection.py
# Author      : Dong Yu
# E-mail      : dongyu1009@163.com
# Date        : 20170315
# Description : convert monthly or annual ndvi dataset of gimms by using MVC(Maximum Value Composite) method
# Version     : 0.1
# Build       : 1
# Copyright (c) 2017 Dong Yu
#*****************************************************************
import arcpy  
arcpy.CheckOutExtension("spatial")  
source_location = ".\\GIMMS_TIF_WGS1984\\"  
month_location = ".\\GIMMS_MVC_MONTH_TIF\\"
year_location = ".\\GIMMS_MVC_YEAR_TIF\\"
print("begin to process the mvc!")  
for year in range(1981,2016):
	for month in range(1,13):
		if year == 1981 and month < 7:
			continue
		a1 = source_location + "GIMMS_" + str(year)+ "_"  + str(month) + "_1.tif"
		a2 = source_location + "GIMMS_" + str(year)+ "_"  + str(month) + "_2.tif"
		out = month_location + "GIMMS_" + str(year) + "_" + str(month) + ".tif"
		arcpy.gp.CellStatistics_sa((a1,a2), out, "MAXIMUM", "DATA")  
		print(str(year) + "_" + str(month) +" has done!")  
	a1 = month_location + "GIMMS_" + str(year) + "_1.tif"
	a2 = month_location + "GIMMS_" + str(year) + "_2.tif"
	a3 = month_location + "GIMMS_" + str(year) + "_3.tif"
	a4 = month_location + "GIMMS_" + str(year) + "_4.tif"
	a5 = month_location + "GIMMS_" + str(year) + "_5.tif"
	a6 = month_location + "GIMMS_" + str(year) + "_6.tif"
	a7 = month_location + "GIMMS_" + str(year) + "_7.tif"
	a8 = month_location + "GIMMS_" + str(year) + "_8.tif"
	a9 = month_location + "GIMMS_" + str(year) + "_9.tif"
	a10 = month_location + "GIMMS_" + str(year) + "_10.tif"
	a11 = month_location + "GIMMS_" + str(year) + "_11.tif"
	a12 = month_location + "GIMMS_" + str(year) + "_12.tif"
	out = year_location + "GIMMS_" + str(year) + ".tif"
	if year == 1981:
		arcpy.gp.CellStatistics_sa((a7,a8,a9,a10,a11,a12), out, "MAXIMUM", "DATA")  
	else:
		arcpy.gp.CellStatistics_sa((a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12), out, "MAXIMUM", "DATA")  
	print(str(year) + " has done!")  
print("All done,please close!")  