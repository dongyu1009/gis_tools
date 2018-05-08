#include <iostream>
#include <math.h>
#include <time.h>
#include "gdal_priv.h"
#include "cpl_conv.h"

using namespace std;

typedef unsigned char DT_8U;
typedef unsigned short DT_16U;

int internel = 8000;
int templatscale = 33;

int maxn(int *vs)
{
    int res = vs[0];
    if (vs[1] > res) res = vs[1];
    if (vs[2] > res) res = vs[2];
    if (vs[3] > res) res = vs[3];
    if (vs[4] > res) res = vs[4];
    return res;
}

void process(char *luccfilename, char *templatefilename)
{
    cout << luccfilename << templatefilename << endl;
    time_t starttime = time(NULL);
    GDALAllRegister();

    // open the lucc file
    GDALDataset *pDataset = (GDALDataset *)GDALOpen(luccfilename, GA_Update); // GA_ReadOnly
    int xsize = pDataset->GetRasterXSize();
    int ysize = pDataset->GetRasterYSize();
    // cout << "xsize: " << xsize << " ysize: " << ysize << endl;
    GDALRasterBand *pBand = pDataset->GetRasterBand(1);
    int nodatavalue = pBand->GetNoDataValue();
    
    // open the model file
    GDALDataset *mDataset = (GDALDataset *)GDALOpen(templatefilename, GA_ReadOnly); // GA_ReadOnly
    int m_xsize = mDataset->GetRasterXSize();
    int m_ysize = mDataset->GetRasterYSize();
    DT_8U *modeldata = new DT_8U[m_xsize * m_ysize];
    GDALRasterBand *mBand = mDataset->GetRasterBand(1);
    mBand->RasterIO(GF_Read, 0, 0, m_xsize, m_ysize, modeldata, m_xsize, m_ysize, GDT_Byte, 0, 0);

    // compute the segment count 
    int xlength = ceil(xsize / float(internel));
    int ylength = ceil(ysize / float(internel));
    cout << "xlength: " << xlength << " ylength: " << ylength << endl;
    for(int x = 0; x < xlength; x++)
    {
        for(int y = 0; y < ylength; y++)
        {
            cout << luccfilename << " "  << "x: " << x << " y: " << y << endl;
            time_t _time = time(NULL);
            int xx_min = x * internel;
            int yy_min = y * internel;
            int xx_max = min((x + 1) * internel, xsize);
            int yy_max = min((y + 1) * internel, ysize);
            int xxlength = xx_max - xx_min;
            int yylength = yy_max - yy_min;
            DT_8U *data = new DT_8U[xxlength * yylength];
            pBand->RasterIO(GF_Read, xx_min, yy_min, xxlength, yylength, data, xxlength, yylength, GDT_Byte, 0, 0);
            for(int xx = 0; xx < xxlength; xx++)
            {
                //cout << xx * 100.0 / xxlength << endl;
                for(int yy = 0; yy < yylength; yy++)
                {
                    int value = data[yy * xxlength + xx];
                    if(value == nodatavalue || value / 10 != 2)
                        continue;

                    int model_x = (xx + xx_min) / templatscale;
                    int model_y = (yy + yy_min) / templatscale;
                    int modelvalue = modeldata[model_y * m_xsize + model_x];
                    if(modelvalue > 0 && modelvalue < 7)
                    {
                        value = modelvalue + 20;
                    }
                    else {
                        int step = 1;
                        while(step)
                        {
                            // cout << step << endl;
                            int length = step * 2;
                            int *counts = new int[6]{0,0,0,0,0,0};
                            for(int i = 0; i < length; i++)
                            {
                                int mxx, myy;
                                mxx = model_x - step + i;
                                myy = model_y - step;
                                if(myy >=0 && mxx >=0 && mxx < m_xsize)
                                {
                                    modelvalue = modeldata[myy * m_xsize + mxx]; 
                                    if(modelvalue > 0 && modelvalue < 7)
                                        counts[modelvalue - 1] ++;
                                }
                                mxx = model_x + step;
                                myy = model_y - step + i;
                                if(myy >=0 && myy < m_ysize && mxx < m_xsize)
                                {
                                    modelvalue = modeldata[myy * m_xsize + mxx]; 
                                    if(modelvalue > 0 && modelvalue < 7)
                                        counts[modelvalue - 1] ++;
                                }
                                mxx = model_x - step;
                                myy = model_y - step + i + 1;
                                if(myy >=0 && myy < m_ysize && mxx >= 0)
                                {
                                    modelvalue = modeldata[myy * m_xsize + mxx]; 
                                    if(modelvalue > 0 && modelvalue < 7)
                                        counts[modelvalue - 1] ++;
                                }
                                mxx = model_x - step + i + 1;
                                myy = model_y + step;
                                if(mxx >= 0 && mxx < m_xsize && myy < m_ysize)
                                {
                                    modelvalue = modeldata[myy * m_xsize + mxx]; 
                                    if(modelvalue > 0 && modelvalue < 7)
                                        counts[modelvalue - 1] ++;
                                }
                            }
                            
                            if(counts[0] + counts[1] + counts[2] + counts[3] + counts[4] + counts[5])
                            {
                                step = 0;
                                int maxcount = maxn(counts);

                                if(maxcount == counts[0]) modelvalue = 21;
                                if(maxcount == counts[1]) modelvalue = 22;
                                if(maxcount == counts[2]) modelvalue = 23;
                                if(maxcount == counts[3]) modelvalue = 24;
                                if(maxcount == counts[4]) modelvalue = 25;
                                if(maxcount == counts[5]) modelvalue = 26;
                            }
                            else {
                                step++;
                            }
                            delete []counts;
                        }
                        value = modelvalue;
                    }
                    data[yy * xxlength + xx] = value;
                }
            }
            pBand->RasterIO(GF_Write, xx_min, yy_min, xxlength, yylength, data, xxlength, yylength, GDT_Byte, 0, 0);
            delete []data;
            time_t _time2 = time(NULL);
            // cout << "block time : " << difftime(_time2, _time) << endl;
        }
    }
    delete []modeldata;
    GDALClose((GDALDatasetH)pDataset);
    GDALClose((GDALDatasetH)mDataset);
    time_t endtime = time(NULL);
    cout << "Computing time : " << difftime(endtime, starttime) << endl;
}

int main(void)
{
    /*
    process("/root/lucc_combine/lucc1980.tif", "/root/lucc_combine/igbp1980.tif");
    process("/root/lucc_combine/lucc1990.tif", "/root/lucc_combine/igbp1995.tif");
    process("/root/lucc_combine/lucc1995.tif", "/root/lucc_combine/igbp1995.tif");
    process("/root/lucc_combine/lucc2000.tif", "/root/lucc_combine/igbp2000.tif");
    process("/root/lucc_combine/lucc2005.tif", "/root/lucc_combine/igbp2005.tif");
    process("/root/lucc_combine/lucc2008.tif", "/root/lucc_combine/igbp2005.tif");
    process("/root/lucc_combine/lucc2010.tif", "/root/lucc_combine/igbp2005.tif");
    */
    process("/root/lucc_combine/lucc2013.tif", "/root/lucc_combine/igbp2005.tif");
    // process("/root/lucc_combine/lucc2015.tif", "/root/lucc_combine/igbp2005.tif");
    return 0;
}
