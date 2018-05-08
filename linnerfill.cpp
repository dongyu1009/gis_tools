#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include "gdal_priv.h"

using namespace std;

void process(int year1, int year2){
    
    GDALAllRegister();

    const char* pszFormat = "GTiff";
    GDALDriver *pDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);

    string types[7] = {"21", "22", "23", "24", "25", "26", "3x"};
    for(int i = 0; i < 7; i ++) {

        char year1filename[100];
        sprintf(year1filename, "/root/lucc_combine/per/per_%d_%s.tif", year1, types[i].c_str());
        GDALDataset *year1Dataset = (GDALDataset *)GDALOpen(year1filename, GA_ReadOnly);
        GDALRasterBand *y1Band = year1Dataset->GetRasterBand(1);
        double nodata = y1Band->GetNoDataValue(NULL);
        double adfGeoTransform[6];
        year1Dataset->GetGeoTransform(adfGeoTransform);
        const char* sr_wkt  = year1Dataset->GetProjectionRef();
        int xsize = year1Dataset->GetRasterXSize();
        int ysize = year1Dataset->GetRasterYSize();
        int *y1data = new int[xsize * ysize];
        y1Band->RasterIO(GF_Read, 0, 0, xsize, ysize, y1data, xsize, ysize, GDT_Int32, 0, 0);
    
        char year2filename[100];
        sprintf(year2filename, "/root/lucc_combine/per/per_%d_%s.tif", year2, types[i].c_str());
        GDALDataset *year2Dataset = (GDALDataset *)GDALOpen(year2filename, GA_ReadOnly);
        GDALRasterBand *y2Band = year2Dataset->GetRasterBand(1);
        int *y2data = new int[xsize * ysize];
        y2Band->RasterIO(GF_Read, 0, 0, xsize, ysize, y2data, xsize, ysize, GDT_Int32, 0, 0);

        for(int year = year1 + 1; year < year2; year ++){

            int *data = new int[xsize * ysize];
            for(int i = 0; i < xsize * ysize; i ++)
            {
                double k = (y2data[i] - y1data[i]) * 1.0 / (year2 - year1);
                data[i] = y1data[i] + k * (year - year1);
            }
            char outfilename[100];
            sprintf(outfilename, "/root/lucc_combine/per/per_%d_%s.tif", year, types[i].c_str());
            cout << outfilename << endl;

            GDALDataset *poDataset = pDriver->Create(outfilename, xsize, ysize, 1, GDT_Int32, NULL);
            poDataset->SetGeoTransform(adfGeoTransform);
            poDataset->SetProjection(sr_wkt);
            GDALRasterBand *poBand = poDataset->GetRasterBand(1);
            poBand->SetNoDataValue(nodata);
            poBand->RasterIO(GF_Write, 0, 0, xsize, ysize, data, xsize, ysize, GDT_Int32, 0, 0);

            GDALClose((GDALDatasetH)poDataset);
            delete []data;
        
        }

        GDALClose((GDALDatasetH)year1Dataset);
        GDALClose((GDALDatasetH)year2Dataset);
        delete []y1data;
        delete []y2data;
    }
}

int main(void)
{
    int* years = new int[9]{1980, 1990, 1995, 2000, 2005, 2008, 2010, 2013, 2015};
    for(int i = 0; i < 8; i++)
    {
        int year1 = years[i];
        int year2 = years[i + 1];
        process(year1, year2);
    }
    delete []years;
    return 0;
}
