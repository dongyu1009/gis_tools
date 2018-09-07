// compile command: g++ test.cpp -lgdal -std=c++11
#include <iostream>
#include <gdal_priv.h>
#include <sys/stat.h>
#include <errno.h>
#include <string>
#include <ctime>
#include <math.h>

#define random(a,b) (rand()%(b-a+1)+a)
using namespace std;

typedef short DT_8;

int createfile(char filename[], int jday)
{
    GDALDriver *driver = (GDALDriver *)GDALGetDriverByName("ENVI"); // "GTiff"

    // make directory
    char directoryname[100];
    sprintf(directoryname, "./LC8024036%sLGN01", filename);
    mkdir(directoryname, 0777);

    // data filename
    char outfilename[100];
    sprintf(outfilename, "./LC8024036%sLGN01/LC8024036%sLGN01_MTLstack", filename, filename);
    GDALDataset* dataset = driver->Create(outfilename, 10, 10, 8, GDT_Int16, NULL);

    dataset->SetProjection("GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"4326\"]]");

    for(int i = 1; i < 9; i++)
    {
        GDALRasterBand *band = dataset->GetRasterBand(i);
        DT_8 *data = new DT_8[10 * 10];
        for(int j = 0; j < 10 * 10; j++)
        {
            // int randomnumber = random(0, 10000);
            //
            int base = (sin(jday * 3.14 / 180)+1) * 1000;
            int randomnumber = random(0, 100);
            data[j] = base + randomnumber;
            if(i == 8) data[j] = 0;
        }
        band->RasterIO(GF_Write, 0, 0, 10, 10, data, 10, 10, GDT_Int16, 0, 0);
        delete[] data;
    }

    if(dataset == NULL) {
        cout << "failure!" << endl;
    } else {
        cout << "success!" << endl;
    }

    GDALClose(dataset);
    return 1;
}

int main()
{
    GDALAllRegister();

    for(int i = 2000; i < 2016; i++)
    {
        for(int j = 1; j < 365; j+=7)
        {
            char filename[100];
            sprintf(filename, "%04d%03d", i, j);
            createfile(filename, j);
        }
    }
    return 0;
}
