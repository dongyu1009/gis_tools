#include <iostream>
#include "gdal_priv.h"
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>

#define START_YEAR 1981 
#define END_YEAR 2000  
#define OUTPUT_FORMAT "GTiff"
#define OUTPUT_NAME "./3std.tif"

using namespace std;

typedef float DT_F32;

void createraster(const char *filename, DT_F32 *resdata, int xsize, int ysize, double *adfGeoTransform, const char* sr_wkt, DT_F32 nodatavalue);

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
    DT_F32 *n3std = new DT_F32[xsize * ysize]; // μ - 3σ
    DT_F32 *n2std = new DT_F32[xsize * ysize]; // μ - 2σ
    DT_F32 *nstd = new DT_F32[xsize * ysize]; // μ - σ
    
    DT_F32 *n3stdgt0 = new DT_F32[xsize * ysize]; // following is greater than 0
    DT_F32 *n2stdgt0 = new DT_F32[xsize * ysize];
    DT_F32 *nstdgt0 = new DT_F32[xsize * ysize];

    DT_F32 *pstd = new DT_F32[xsize * ysize]; // population standard variation


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

        double accum = .0;

        for_each(begin(values), end(values), [&](int value) {
            accum += (value - mean) * (value - mean);
        });

        double std = sqrt(accum / values.size());
        n3std[i] = mean - 3 * std;
        n2std[i] = mean - 2 * std;
        nstd[i] = mean -  std;

        if (values[0] != nodatavalue){
            n3stdgt0[i] = ((mean - 3 * std) < 0) ? 0 : (mean - 3 * std);
            n2stdgt0[i] = ((mean - 2 * std) < 0) ? 0 : (mean - 2 * std);
            nstdgt0[i] = ((mean - std) < 0) ? 0 : (mean - std);
            pstd[i] = std;
        } else {
            n3stdgt0[i] = nodatavalue;
            n2stdgt0[i] = nodatavalue;
            nstdgt0[i] = nodatavalue;
            pstd[i] = nodatavalue;
        }
    }

    // create the new file 
    // void createraster(const char *filename, DT_F32 *resdata, xsize, ysize, double *adfGeoTransform, const char* sr_wkt, DT_F32 nodatavalue){
    createraster("./std.tif", pstd, xsize, ysize, adfGeoTransform, sr_wkt, nodatavalue);
    createraster("./nstd.tif", nstd, xsize, ysize, adfGeoTransform, sr_wkt, nodatavalue);
    createraster("./n2std.tif", n2std, xsize, ysize, adfGeoTransform, sr_wkt, nodatavalue);
    createraster("./n3std.tif", n3std, xsize, ysize, adfGeoTransform, sr_wkt, nodatavalue);
    createraster("./nstdgt0.tif", nstdgt0, xsize, ysize, adfGeoTransform, sr_wkt, nodatavalue);
    createraster("./n2stdgt0.tif", n2stdgt0, xsize, ysize, adfGeoTransform, sr_wkt, nodatavalue);
    createraster("./n3stdgt0.tif", n3stdgt0, xsize, ysize, adfGeoTransform, sr_wkt, nodatavalue);
    delete []pstd;
    delete []nstd;
    delete []n2std;
    delete []n3std;
    delete []nstdgt0;
    delete []n2stdgt0;
    delete []n3stdgt0;

    // free the data in heap
    for(int year = START_YEAR; year < END_YEAR + 1; year ++) {
        DT_F32 *data = datas[year - START_YEAR];
        delete []data;
    }
    return 0;

}

void createraster(const char *filename, DT_F32 *resdata, int xsize, int ysize, double *adfGeoTransform, const char* sr_wkt, DT_F32 nodatavalue){
    
    GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(OUTPUT_FORMAT);
    GDALDataset *rd = driver->Create(filename, xsize, ysize, 1, GDT_Float32, NULL);
    rd->SetGeoTransform(adfGeoTransform);
    // rd->SetProjection(sr_wkt);
    GDALRasterBand *band = rd->GetRasterBand(1);
    band->SetNoDataValue(nodatavalue);
    band->RasterIO(GF_Write, 0, 0, xsize, ysize, resdata, xsize, ysize, GDT_Float32, 0, 0);

    GDALClose(rd);
}
