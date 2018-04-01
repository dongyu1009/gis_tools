# -*- encoding:utf-8 -*-
import osgeo.gdal as gdal
import numpy as np
import os
import copy
import math

if __name__ == '__main__':
    # config the parameters
    internel = 8000
    
    # read the image
    ds_in1 = gdal.Open(".\\test\\china.tif", gdal.GA_Update) # GA_Update  GA_ReadOnly 
    im_width1 = ds_in1.RasterXSize
    im_height1 = ds_in1.RasterYSize
    print im_width1, " ", im_height1
    # im_bands1 = ds_in1.RasterCount
    im_geotrans1 = ds_in1.GetGeoTransform()
    im_proj1 = ds_in1.GetProjection()
    nodatavalue = ds_in1.GetRasterBand(1).GetNoDataValue()
    
    # ds_model = gdal.Open(".\\test\\china.tif") # GA_Update  GA_ReadOnly 
    
    x_length = int(math.ceil(im_width1 / float(internel)))
    y_length = int(math.ceil(im_height1 / float(internel)))
    # specific operation
    for i in range(x_length):
        for j in range(y_length):
            x_min = i * internel
            y_min = j * internel
            x_max = min((i + 1) * internel, im_width1)
            y_max = min((j + 1) * internel, im_height1)
            im_data1 = ds_in1.ReadAsArray(x_min, y_min, x_max - x_min, y_max - y_min)
            print "xmin", x_min, "y_min", y_min, "xmax", x_max, "ymax", y_max
            print im_data1.shape
            for x in range(x_max - x_min):
                for y in range(y_max - y_min):
                    im_data1[y][x] = i*j
            # res0 = copy.deepcopy(im_data1).astype(np.float32)

            # print im_data1.shape
            print "finished!"
            band = ds_in1.GetRasterBand(1)
            band.WriteArray(im_data1, x_min, y_min) # .astype(np.float32)
    
    # clean the variables
    del ds_in1
    del im_data1
