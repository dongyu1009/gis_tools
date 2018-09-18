from qgis import *
from qgis.analysis import QgsRasterCalculator, QgsRasterCalculatorEntry
from qgis.core import *

def OpenRaster(raster):
    # Check if string is provided

    fileInfo = QFileInfo(raster)
    path = fileInfo.filePath()
    baseName = fileInfo.baseName()

    layer = QgsRasterLayer(path, baseName)
    # QgsMapLayerRegistry.instance().addMapLayer(layer)

    if layer.isValid() is True:
        # print("Layer was loaded successfully!")
        return layer

    else:
        # print("Unable to read basename and file path - Your string is probably invalid")
        return NULL


def RasterAddition(luccraster, nppraster, outputfilename):
    luccraster = OpenRaster(luccfilename)
    # print("Lucc : " + str(luccraster.isValid()))

    nppraster = OpenRaster(nppfilename)
    # print("NPP : " + str(nppraster.isValid()))

    entries = []
    boh1 = QgsRasterCalculatorEntry()
    boh1.ref = 'lucc@1'
    boh1.raster = luccraster
    boh1.bandNumber = 1
    entries.append( boh1 )

    boh2 = QgsRasterCalculatorEntry()
    boh2.ref = 'npp@1'
    boh2.raster = nppraster
    boh2.bandNumber = 1
    entries.append( boh2 )

    # Process calculation with input extent and resolution
    calc = QgsRasterCalculator( 'lucc@1 + npp@1', 
                            outputfilename, 
                            'GTiff',
                            luccraster.extent(), 
                            luccraster.width(), 
                            luccraster.height(), 
                            entries )

    calc.processCalculation()


# to disable 'coordinate reference system selector' dialog using QGIS 3 only need this
settings = QSettings()
settings.setValue('/Projections/defaultBehavior', '')

lnames = ["esacci", "glc", "globcover", "globleland30"]
nnames = ["avhrr", "mod17a3h"]
ntypes = ["npp_mean", "npp_min", "nstdgt0", "n2stdgt0", "n3stdgt0"]

luccpath = 'G:/mongolia_grass_capacity/data/lucc_%(lname)s/grass.tif'
npppath = 'G:/mongolia_grass_capacity/data/npp_%(nname)s/output/%(ntype)s.tif'
outputpath = 'G:/mongolia_grass_capacity/data/npp_lucc_combine/%(lname)s_%(nname)s_%(ntype)s.tif'

for lname in lnames:
    for nname in nnames:
        for ntype in ntypes:
            luccfilename = luccpath % {'lname': lname}
            nppfilename = npppath % {'nname': nname, 'ntype': ntype}
            outputfilename = outputpath % {'lname': lname, 'nname': nname,'ntype': ntype}
            print("luccfilename : ", luccfilename)
            print("nppfilename : ", nppfilename)
            print("outputfilename : ", outputfilename)
            RasterAddition(OpenRaster(luccfilename), OpenRaster(nppfilename), outputfilename)