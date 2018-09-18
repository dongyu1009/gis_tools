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


luccfilename = r'G:\mongolia_grass_capacity\data\lucc_esacci\grass.tif'
luccraster = OpenRaster(luccfilename)
print("Lucc : " + str(luccraster.isValid()))

nppfilename = r'G:\mongolia_grass_capacity\data\npp_avhrr\output\n3stdgt0.tif'
nppraster = OpenRaster(nppfilename)
print("NPP : " + str(nppraster.isValid()))

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
                        'G:/abc.tif', 
                        'GTiff',
                        luccraster.extent(), 
                        luccraster.width(), 
                        luccraster.height(), 
                        entries )

calc.processCalculation()