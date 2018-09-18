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
        return null

# to disable 'coordinate reference system selector' dialog using QGIS 3 only need this
settings = QSettings()
settings.setValue('/Projections/defaultBehavior', '')

filename = 'G:/mongolia_grass_capacity/data/npp_mod17a3h/output/n3std.tif'

raster = OpenRaster(filename)

print(raster.isValid())