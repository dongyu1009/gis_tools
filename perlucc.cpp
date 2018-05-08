#include <iostream>
#include <stdio.h>
#include <string.h>
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "math.h"

using namespace std;

typedef unsigned char DT_8U;
typedef unsigned short DT_16U;

int roundm(double value)
{
    return (value > .0) ? (value + .5) : (value - .5);
}

void process(int year, char filename[]){

    int pixelsize = 1000;

    GDALAllRegister();

    // read data
    GDALDataset *pDataset = (GDALDataset *)GDALOpen(filename, GA_ReadOnly);
    double adfGeoTransform[6];
    pDataset->GetGeoTransform(adfGeoTransform);
    int xsize = pDataset->GetRasterXSize();
    int ysize = pDataset->GetRasterYSize();
    const char* sr_wkt  = pDataset->GetProjectionRef();
    GDALRasterBand *pBand = pDataset->GetRasterBand(1);
    double nodata = pBand->GetNoDataValue(NULL);

    int pixelx = adfGeoTransform[1];
    int pixely = adfGeoTransform[5] * -1;
    adfGeoTransform[1] = 1000; 
    adfGeoTransform[5] = -1000; 

    // create new data
    int xlength = ceil(pixelx * xsize * 1.0 / pixelsize);
    int ylength = ceil(pixely * ysize * 1.0 / pixelsize);
    cout << "xlength : " << xlength << " ylength : " << ylength<< endl;
    int *resultdata21 = new int[xlength * ylength];
    int *resultdata22 = new int[xlength * ylength];
    int *resultdata23 = new int[xlength * ylength];
    int *resultdata24 = new int[xlength * ylength];
    int *resultdata25 = new int[xlength * ylength];
    int *resultdata26 = new int[xlength * ylength];
    int *resultdata3x = new int[xlength * ylength];
    for(int x = 0; x < xlength; x++)
    {
        // cout << "x : " << x << endl;
        for(int y = 0; y < ylength; y++)
        {
            // cout << "x : " << x << " y : " << y << endl;
            int xx_min = roundm(x * pixelsize * 1.0 / pixelx);
            int yy_min = roundm(y * pixelsize * 1.0 / pixely);
            int xx_max = min(roundm((x + 1) * pixelsize * 1.0 / pixelx), xsize);
            int yy_max = min(roundm((y + 1) * pixelsize * 1.0 / pixely), ysize);
            // cout << xx_min << " " << yy_min << " " << xx_max << " " << yy_max << " " << endl;
            int xxlength = xx_max - xx_min;
            int yylength = yy_max - yy_min;
            int allcount = xxlength * yylength;
            DT_8U *data = new DT_8U[allcount];
            // cout << allcount << endl;
            pBand->RasterIO(GF_Read, xx_min, yy_min, xxlength, yylength, data, xxlength, yylength, GDT_Byte, 0, 0);

            int isNodata = 1;
            int value21count = 0;
            int value22count = 0;
            int value23count = 0;
            int value24count = 0;
            int value25count = 0;
            int value26count = 0;
            int value3xcount = 0;
            for(int xx = 0; xx < xxlength; xx ++)
            {
                for(int yy = 0; yy < yylength; yy ++)
                {
                    int value = data[yy * xxlength + xx];
                    if(value != nodata)
                    {
                        isNodata = 0;
                    }
                    if(21 == value) value21count ++;
                    if(22 == value) value22count ++;
                    if(23 == value) value23count ++;
                    if(24 == value) value24count ++;
                    if(25 == value) value25count ++;
                    if(26 == value) value26count ++;
                    if(value > 30 && value < 40) value3xcount ++;
                }
            }
            if(isNodata)
            {
                resultdata21[y * xlength + x] = nodata;
                resultdata22[y * xlength + x] = nodata;
                resultdata23[y * xlength + x] = nodata;
                resultdata24[y * xlength + x] = nodata;
                resultdata25[y * xlength + x] = nodata;
                resultdata26[y * xlength + x] = nodata;
                resultdata3x[y * xlength + x] = nodata;
            }
            else {
                resultdata21[y * xlength + x] = value21count * 1.0 / allcount * 10000;
                resultdata22[y * xlength + x] = value22count * 1.0 / allcount * 10000;
                resultdata23[y * xlength + x] = value23count * 1.0 / allcount * 10000;
                resultdata24[y * xlength + x] = value24count * 1.0 / allcount * 10000;
                resultdata25[y * xlength + x] = value25count * 1.0 / allcount * 10000;
                resultdata26[y * xlength + x] = value26count * 1.0 / allcount * 10000;
                resultdata3x[y * xlength + x] = value3xcount * 1.0 / allcount * 10000;
            }

            delete []data;
        }
    }
    const char* pszFormat = "GTiff";
    GDALDriver *pDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
    
    GDALDataset *poDataset;
    GDALRasterBand *poBand;

    char prefix[10];
    char outfilename21[100] = "/root/lucc_combine/per/per_";
    sprintf(prefix, "%d_21.tif", year);
    cout << outfilename21 << endl;
    cout << prefix << endl;
    strcat(outfilename21, prefix);
    poDataset = pDriver->Create(outfilename21, xlength, ylength, 1, GDT_Int32, NULL);
    poDataset->SetGeoTransform(adfGeoTransform);
    poDataset->SetProjection(sr_wkt);
    poBand = poDataset->GetRasterBand(1);
    poBand->SetNoDataValue(nodata);
    poBand->RasterIO(GF_Write, 0, 0, xlength, ylength, resultdata21, xlength, ylength, GDT_Int32, 0, 0);
    GDALClose((GDALDatasetH)poDataset);

    char outfilename22[100] = "/root/lucc_combine/per/per_";
    sprintf(prefix, "%d_22.tif", year);
    strcat(outfilename22, prefix);
    poDataset = pDriver->Create(outfilename22, xlength, ylength, 1, GDT_Int32, NULL);
    poDataset->SetGeoTransform(adfGeoTransform);
    poDataset->SetProjection(sr_wkt);
    poBand = poDataset->GetRasterBand(1);
    poBand->SetNoDataValue(nodata);
    poBand->RasterIO(GF_Write, 0, 0, xlength, ylength, resultdata22, xlength, ylength, GDT_Int32, 0, 0);
    GDALClose((GDALDatasetH)poDataset);

    char outfilename23[100] = "/root/lucc_combine/per/per_";
    sprintf(prefix, "%d_23.tif", year);
    strcat(outfilename23, prefix);
    poDataset = pDriver->Create(outfilename23, xlength, ylength, 1, GDT_Int32, NULL);
    poDataset->SetGeoTransform(adfGeoTransform);
    poDataset->SetProjection(sr_wkt);
    poBand = poDataset->GetRasterBand(1);
    poBand->SetNoDataValue(nodata);
    poBand->RasterIO(GF_Write, 0, 0, xlength, ylength, resultdata23, xlength, ylength, GDT_Int32, 0, 0);
    GDALClose((GDALDatasetH)poDataset);

    char outfilename24[100] = "/root/lucc_combine/per/per_";
    sprintf(prefix, "%d_24.tif", year);
    strcat(outfilename24, prefix);
    poDataset = pDriver->Create(outfilename24, xlength, ylength, 1, GDT_Int32, NULL);
    poDataset->SetGeoTransform(adfGeoTransform);
    poDataset->SetProjection(sr_wkt);
    poBand = poDataset->GetRasterBand(1);
    poBand->SetNoDataValue(nodata);
    poBand->RasterIO(GF_Write, 0, 0, xlength, ylength, resultdata24, xlength, ylength, GDT_Int32, 0, 0);
    GDALClose((GDALDatasetH)poDataset);

    char outfilename25[100] = "/root/lucc_combine/per/per_";
    sprintf(prefix, "%d_25.tif", year);
    strcat(outfilename25, prefix);
    poDataset = pDriver->Create(outfilename25, xlength, ylength, 1, GDT_Int32, NULL);
    poDataset->SetGeoTransform(adfGeoTransform);
    poDataset->SetProjection(sr_wkt);
    poBand = poDataset->GetRasterBand(1);
    poBand->SetNoDataValue(nodata);
    poBand->RasterIO(GF_Write, 0, 0, xlength, ylength, resultdata25, xlength, ylength, GDT_Int32, 0, 0);
    GDALClose((GDALDatasetH)poDataset);

    char outfilename26[100] = "/root/lucc_combine/per/per_";
    sprintf(prefix, "%d_26.tif", year);
    strcat(outfilename26, prefix);
    poDataset = pDriver->Create(outfilename26, xlength, ylength, 1, GDT_Int32, NULL);
    poDataset->SetGeoTransform(adfGeoTransform);
    poDataset->SetProjection(sr_wkt);
    poBand = poDataset->GetRasterBand(1);
    poBand->SetNoDataValue(nodata);
    poBand->RasterIO(GF_Write, 0, 0, xlength, ylength, resultdata26, xlength, ylength, GDT_Int32, 0, 0);
    GDALClose((GDALDatasetH)poDataset);
    
    char outfilename3x[100] = "/root/lucc_combine/per/per_";
    sprintf(prefix, "%d_3x.tif", year);
    strcat(outfilename3x, prefix);
    poDataset = pDriver->Create(outfilename3x, xlength, ylength, 1, GDT_Int32, NULL);
    poDataset->SetGeoTransform(adfGeoTransform);
    poDataset->SetProjection(sr_wkt);
    poBand = poDataset->GetRasterBand(1);
    poBand->SetNoDataValue(nodata);
    poBand->RasterIO(GF_Write, 0, 0, xlength, ylength, resultdata3x, xlength, ylength, GDT_Int32, 0, 0);
    GDALClose((GDALDatasetH)poDataset);
    
    delete []resultdata21;
    delete []resultdata22;
    delete []resultdata23;
    delete []resultdata24;
    delete []resultdata25;
    delete []resultdata26;
    delete []resultdata3x;
    GDALClose((GDALDatasetH)pDataset);
}

int main(void)
{
    int* years = new int[9]{1980, 1990, 1995, 2000, 2005, 2008, 2010, 2013, 2015};
    char filenames[][100] = {"/root/lucc_combine/lucc1980.tif", 
                            "/root/lucc_combine/lucc1990.tif",
                            "/root/lucc_combine/lucc1995.tif",
                            "/root/lucc_combine/lucc2000.tif",
                            "/root/lucc_combine/lucc2005.tif",
                            "/root/lucc_combine/lucc2008.tif",
                            "/root/lucc_combine/lucc2010.tif",
                            "/root/lucc_combine/lucc2013.tif",
                            "/root/lucc_combine/lucc2015.tif"};
    for(int i = 0; i < 9; i++)
    {
        cout << years[i] << " " << filenames[i] << endl;
        process(years[i], filenames[i]);
    }
    delete []years;
    return 0;
}
