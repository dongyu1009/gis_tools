#include <iostream>
#include <stdio.h>
#include <string.h>
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "math.h"

using namespace std;

typedef unsigned char DT_8U;

int roundm(double value)
{
    return (value > .0) ? (value + .5) : (value - .5);
}

void process(){

    long count = 0;
    int pixelsize = 1000;

    GDALAllRegister();

    // read data
    GDALDataset *pDataset = (GDALDataset *)GDALOpen("counttest.tif", GA_ReadOnly);
    double adfGeoTransform[6];
    pDataset->GetGeoTransform(adfGeoTransform);
    int xsize = pDataset->GetRasterXSize();
    int ysize = pDataset->GetRasterYSize();
    const char* sr_wkt  = pDataset->GetProjectionRef();
    GDALRasterBand *pBand = pDataset->GetRasterBand(1);
    double nodata = pBand->GetNoDataValue(NULL);

    // int pixelx = adfGeoTransform[1];
    // int pixely = adfGeoTransform[5] * -1;
    // cout << "pixelx : " << xsize << " pixely: " << ysize << endl;
    // adfGeoTransform[1] = 1000; 
    // adfGeoTransform[5] = -1000; 

    // create new data
    int xlength = ceil(xsize * 1.0 / pixelsize);
    int ylength = ceil(ysize * 1.0 / pixelsize);
    cout << "xlength : " << xlength << " ylength : " << ylength<< endl;

    for(int x = 0; x < xlength; x++)
    {
        for(int y = 0; y < ylength; y++)
        {
            cout << "x : " << x << " y : " << y << endl;
            int xx_min = roundm(x * pixelsize * 1.0 );
            int yy_min = roundm(y * pixelsize * 1.0 );
            int xx_max = min(roundm((x + 1) * pixelsize * 1.0 ), xsize);
            int yy_max = min(roundm((y + 1) * pixelsize * 1.0 ), ysize);
            // cout << xx_min << " " << yy_min << " " << xx_max << " " << yy_max << " " << endl;
            int xxlength = xx_max - xx_min;
            int yylength = yy_max - yy_min;
            int allcount = xxlength * yylength;
            DT_8U *data = new DT_8U[allcount];
            // cout << allcount << endl;
            pBand->RasterIO(GF_Read, xx_min, yy_min, xxlength, yylength, data, xxlength, yylength, GDT_Byte, 0, 0);

            for(int xx = 0; xx < xxlength; xx ++)
            {
                for(int yy = 0; yy < yylength; yy ++)
                {
                    int value = data[yy * xxlength + xx];
                    if(value == 130 || value == 110)
                    {
                        count ++;
                    }
                }
            }

            delete []data;
        }
    }
    GDALClose((GDALDatasetH)pDataset);
    
    cout << count << endl;
}

int main(void)
{
    process();
}
