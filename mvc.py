#*****************************************************************
# Filename    : mvc.py
# Author      : Dong Yu
# E-mail      : dongyu1009@163.com
# Date        : 20180506
# Description : MVC(Maximum Value Composite) method
# Version     : 0.1
# Build       : 1
# 2018 Dong Yu
#*****************************************************************
import arcpy  

if __name__ == "__main__":


    arcpy.CheckOutExtension("Spatial")

    # target spatial reference
    arcpy.env.workspace = ".\\"
    outfilename = ".\\evimax2017.tif"
    rasterlist = arcpy.ListRasters()
    
    # print rasterlist
    filenames = []
    for fc in rasterlist:
        # print fc
        filename = ".\\" + fc
        filenames.append(filename)
    print filenames
    arcpy.gp.CellStatistics_sa(filenames, outfilename, "MAXIMUM", "DATA")  
    print("All done,please close!")  
    
