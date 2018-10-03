#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include "gdal_priv.h"

using namespace std;

void compute(int year, string localu){
    const char *local = localu.c_str(); 
    char filename[100];
    sprintf(filename, "./gpp/%d/%s/GPP.%d%03d.%s.tif", year, local, year, 1, local);
    GDALDataset *dataset = (GDALDataset *)GDALOpen(filename, GA_ReadOnly);
    GDALRasterBand *band = dataset->GetRasterBand(1);
    double nodatavalue = band->GetNoDataValue(NULL);
    int xsize = dataset->GetRasterXSize();
    int ysize = dataset->GetRasterYSize();
    
    double adfGeoTransform[6];
    dataset->GetGeoTransform(adfGeoTransform);
    const char* sr_wkt  = dataset->GetProjectionRef();

    int *data = new int[xsize * ysize];
    for(int i = 0; i < xsize * ysize; i++)
        data[i] = 0;

    GDALClose((GDALDatasetH)dataset);
    for(int internal = 1; internal < 365; internal = internal + 8)
    {
        sprintf(filename, "./gpp/%d/%s/GPP.%d%03d.%s.tif", year, local, year, internal, local);
        GDALDataset *dataset = (GDALDataset *)GDALOpen(filename, GA_ReadOnly);
        GDALRasterBand *band = dataset->GetRasterBand(1);
        unsigned short *tdata = new unsigned short[xsize * ysize];
        band->RasterIO(GF_Read, 0, 0, xsize, ysize, tdata, xsize, ysize, GDT_UInt16, 0, 0);

        for(int i = 0; i < xsize * ysize; i++){
            if(tdata[i] == nodatavalue)
                data[i] == nodatavalue;
            else if (internal == 361)
                data[i] += tdata[i] * 5;
            else
                data[i] += tdata[i] * 8;
        }
        delete[] tdata;
        GDALClose((GDALDatasetH)dataset);
    }

    char outputfilename[100];

    sprintf(outputfilename, "./res/gpp_%d_%s.tif", year, local);

    GDALDriver *driver = GetGDALDriverManager()->GetDriverByName("GTiff");
    GDALDataset *poDataset = driver->Create(outputfilename, xsize, ysize, 1, GDT_Int32, NULL);
    poDataset->SetGeoTransform(adfGeoTransform);
    poDataset->SetProjection(sr_wkt);
    GDALRasterBand *poBand = poDataset->GetRasterBand(1);
    poBand->SetNoDataValue(nodatavalue);
    poBand->RasterIO(GF_Write, 0, 0, xsize, ysize, data, xsize, ysize, GDT_Int32, 0, 0);
    

    GDALClose((GDALDatasetH)poDataset);
    delete[] data;
}

int main(){

    GDALAllRegister();

    string locals[11] = {"h23v03", "h23v04", "h24v03", "h24v04", "h25v03", "h25v04", "h25v05", "h26v03", "h26v04", "h26v05", "h27v04"};

    for(int year = 2000; year < 2017; year++)
    {
        for(int i = 0; i < 11; i++)
        {
            cout << "year : " << year << " local: " << locals[i] << endl;
            compute(year, locals[i]); 
        }
    }
    return 0;
}
