#include <iostream>
#include "gdal_priv.h"

using namespace std;

int main(void)
{
    GDALAllRegister();

    GDALDataset *dataset = (GDALDataset *)GDALOpen("mongolia.tif", GA_ReadOnly);

    GDALRasterBand *band = dataset->GetRasterBand(1);

    int xsize = dataset->GetRasterXSize();
    int ysize = dataset->GetRasterYSize();

    float *data = new float[xsize * ysize];

    band->RasterIO(GF_Read, 0, 0, xsize, ysize, data, xsize, ysize, GDT_Float32, 0, 0);

    int count = 0;

    for(int i = 0; i < xsize * ysize; i++)
    {
        float value = data[i];
        if (value == 110 || value == 130)
        {
            count ++;
        }
    }


    GDALClose(dataset);

    delete[] data;

    // cout << sizeof(char)  << endl;
    cout << sizeof(float)  << endl;
    cout << count  << endl;

    return 0;

}

