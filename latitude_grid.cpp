#include <iostream>
#include "gdal_priv.h"

using namespace std;

int main(void)
{
    
    GDALAllRegister();
    
    const char *pszFormat = "GTiff";

    GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(pszFormat);

    GDALDataset *dataset = (GDALDataset *)GDALOpen("./model.tif", GA_Update);
    GDALRasterBand *rasterband = dataset->GetRasterBand(1);
    int xsize = dataset->GetRasterXSize();
    int ysize = dataset->GetRasterYSize();
    int *data = new int[xsize * ysize];
    rasterband->RasterIO(GF_Read, 0, 0, xsize, ysize, data, xsize, ysize, GDT_Int32, 0, 0);

    int basevalue = 2225;
    for(int i = 0; i < xsize; i ++)
    {
        int value = basevalue - i / 1250 * 25;
        for(int j = 0; j < ysize; j ++)
        {
           data[i * xsize + j] = value;
        }
    }

    rasterband->RasterIO(GF_Write, 0, 0, xsize, ysize, data, xsize, ysize, GDT_Int32, 0, 0);

    GDALClose((GDALDatasetH)dataset);
    delete[] data;
    return 0;
}
