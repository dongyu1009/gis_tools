// compile command: g++ test.cpp -lgdal -std=c++11
#include <iostream>
#include <gdal_priv.h>

using namespace std;

typedef short DT_8;

int main()
{
    const char* outfilename = "/root/test/out";

    GDALAllRegister();

    GDALDriver *driver = (GDALDriver *)GDALGetDriverByName("ENVI"); // "GTiff"

    GDALDataset* dataset = driver->Create(outfilename, 10, 10, 1, GDT_Int16, NULL);

    dataset->SetProjection("GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"4326\"]]");

    GDALRasterBand *band = dataset->GetRasterBand(1);

    DT_8 *data = new DT_8[10 * 10];
    for(int i = 0; i < 10 * 10; i++)
    {
        data[i] = i;
    }
    band->RasterIO(GF_Write, 0, 0, 10, 10, data, 10, 10, GDT_Int16, 0, 0);
    delete[] data;

    if(dataset == NULL) {
        cout << "failure!" << endl;
    } else {
        cout << "success!" << endl;
    }

    return 1;
}
