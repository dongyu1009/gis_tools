#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
#include "gdal_priv.h"
#include "cpl_conv.h"

using namespace std;

typedef short DT_16I;
typedef float DT_32F;

int main()
{
    // cout << sizeof(short) << endl;
    // cout << sizeof(float) << endl;
    //

    GDALAllRegister();

    ofstream fout;
    fout.open("output.csv");
    fout << "dem,slope\r\n";

    // read dem data
    GDALDataset *demDataset= (GDALDataset *) GDALOpen("./dem.tif", GA_ReadOnly);
    int xsize = demDataset->GetRasterXSize();
    int ysize = demDataset->GetRasterYSize();
    // cout << xsize << "," << ysize << endl;
    GDALRasterBand *demBand = demDataset->GetRasterBand(1);
    DT_16I *demData = new DT_16I[xsize * ysize];
    demBand->RasterIO(GF_Read, 0, 0, xsize, ysize, demData, xsize, ysize, GDT_Int16, 0, 0);

    // read slope data
    GDALDataset *slopeDataset= (GDALDataset *) GDALOpen("./slope.tif", GA_ReadOnly);
    GDALRasterBand *slopeBand = slopeDataset->GetRasterBand(1);
    DT_32F *slopeData = new DT_32F[xsize * ysize];
    slopeBand->RasterIO(GF_Read, 0, 0, xsize, ysize, slopeData, xsize, ysize, GDT_Float32, 0, 0);

    // read year data
    GDALDataset *dtbDataset= (GDALDataset *) GDALOpen("./disturbance.tif", GA_ReadOnly);
    GDALRasterBand *dtbBand = dtbDataset->GetRasterBand(1);
    DT_32F *dtbData = new DT_32F[xsize * ysize];
    dtbBand->RasterIO(GF_Read, 0, 0, xsize, ysize, dtbData, xsize, ysize, GDT_Float32, 0, 0);
    int nodatavalue = dtbBand->GetNoDataValue();

    cout << nodatavalue << endl;


    for(int i = 0; i < xsize * ysize; i ++)
    {
        short demvalue = demData[i];
        float slopevalue = slopeData[i];
        float dtbvalue = dtbData[i];

        if(dtbvalue > 0)
        {
           fout << demvalue << "," << slopevalue << "\r\n";
        }
    }



    delete []demData;
    delete []slopeData;
    delete []dtbData;

    GDALClose((GDALDatasetH)demDataset);
    GDALClose((GDALDatasetH)slopeDataset);
    GDALClose((GDALDatasetH)dtbDataset);

    fout.close();


    return 0;
}
