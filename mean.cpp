#include <iostream>
#include "gdal_priv.h"
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>

#define START_YEAR 1981 
#define END_YEAR 2000  
#define OUTPUT_FORMAT "GTiff"
#define OUTPUT_NAME "./res.tif"

using namespace std;

typedef float DT_F32;

int main()
{

    // the basic information of each image
    int xsize, ysize;
    DT_F32 nodatavalue;
    double adfGeoTransform[6];
    const char *sr_wkt;

    // the pointer array for the data
    DT_F32 *datas[END_YEAR - START_YEAR + 1];

    GDALAllRegister();

    // read data from  each image
    for(int year = START_YEAR; year < END_YEAR + 1; year ++)
    {
        char filename[100];
        sprintf(filename, "%d_npp_latlon.tif", year);
        GDALDataset *dataset = (GDALDataset *)GDALOpen(filename, GA_ReadOnly);
        GDALRasterBand *band = dataset->GetRasterBand(1);
        nodatavalue = band->GetNoDataValue(NULL);
        dataset->GetGeoTransform(adfGeoTransform);
        sr_wkt = dataset->GetProjectionRef();
        xsize = dataset->GetRasterXSize();
        ysize = dataset->GetRasterYSize();

        DT_F32 *data = new DT_F32[xsize * ysize];

        band->RasterIO(GF_Read, 0, 0, xsize, ysize, data, xsize, ysize, GDT_Float32, 0, 0);
        
        datas[year - START_YEAR] = data;

        GDALClose(dataset);
    }

    // the result data array
    DT_F32 *resdata = new DT_F32[xsize * ysize];
    
    // compute the minimun for every single grid
    for(int i = 0; i < xsize * ysize; i++)
    {

        vector<DT_F32> values;

        for(int j = 0; j < END_YEAR - START_YEAR + 1; j++){
            DT_F32 value = datas[j][i];
            values.push_back(value);
        }

        DT_F32 mean = accumulate(begin(values), end(values), 0);

        mean = mean / values.size();
        resdata[i] = mean;
    }

    // create the new file 
    GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(OUTPUT_FORMAT);
    GDALDataset *rd = driver->Create(OUTPUT_NAME, xsize, ysize, 1, GDT_Float32, NULL);
    rd->SetGeoTransform(adfGeoTransform);
    rd->SetProjection(sr_wkt);
    GDALRasterBand *band = rd->GetRasterBand(1);
    band->SetNoDataValue(nodatavalue);
    band->RasterIO(GF_Write, 0, 0, xsize, ysize, resdata, xsize, ysize, GDT_Float32, 0, 0);

    GDALClose(rd);

    // free the data in heap
    for(int year = START_YEAR; year < END_YEAR + 1; year ++) {
        DT_F32 *data = datas[year - START_YEAR];
        delete []data;
    }
    return 0;
}
