QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += C:\msys64\mingw64\include\vtk-8.2 \
    funcNode/otherNode \
    funcNode/funcCaculate \
    funcNode/funcGraph \
    funcNode \
    childPage \
    operationalLayer \
    logic \
    Global
LIBS += -L C:\msys64\mingw64\lib \
    -lvtkChartsCore-8.2  \
    -lvtkCommonColor-8.2 \
    -lvtkCommonComputationalGeometry-8.2 \
    -lvtkCommonCore-8.2 \
    -lvtkCommonDataModel-8.2 \
    -lvtkCommonExecutionModel-8.2 \
    -lvtkCommonMath-8.2 \
    -lvtkCommonMisc-8.2 \
    -lvtkCommonSystem-8.2 \
    -lvtkCommonTransforms-8.2 \
    -lvtkDICOMParser-8.2 \
    -lvtkDomainsChemistry-8.2 \
    -lvtkexodusII-8.2 \
    -lvtkFiltersAMR-8.2 \
    -lvtkFiltersCore-8.2 \
    -lvtkFiltersExtraction-8.2 \
    -lvtkFiltersFlowPaths-8.2 \
    -lvtkFiltersGeneral-8.2 \
    -lvtkFiltersGeneric-8.2 \
    -lvtkFiltersGeometry-8.2 \
    -lvtkFiltersHybrid-8.2 \
    -lvtkFiltersHyperTree-8.2 \
    -lvtkFiltersImaging-8.2 \
    -lvtkFiltersModeling-8.2 \
    -lvtkFiltersParallel-8.2 \
    -lvtkFiltersParallelImaging-8.2 \
    -lvtkFiltersPoints-8.2 \
    -lvtkFiltersProgrammable-8.2 \
    -lvtkFiltersSelection-8.2 \
    -lvtkFiltersSMP-8.2 \
    -lvtkFiltersSources-8.2 \
    -lvtkFiltersStatistics-8.2 \
    -lvtkFiltersTexture-8.2 \
    -lvtkFiltersTopology-8.2 \
    -lvtkFiltersVerdict-8.2 \
    -lvtkGeovisCore-8.2 \
    -lvtkgl2ps-8.2 \
    -lvtkglew-8.2 \
    -lvtkGUISupportQt-8.2 \
    -lvtkGUISupportQtSQL-8.2 \
    -lvtkImagingColor-8.2 \
    -lvtkImagingCore-8.2 \
    -lvtkImagingFourier-8.2 \
    -lvtkImagingGeneral-8.2 \
    -lvtkImagingHybrid-8.2 \
    -lvtkImagingMath-8.2 \
    -lvtkImagingMorphological-8.2 \
    -lvtkImagingSources-8.2 \
    -lvtkImagingStatistics-8.2 \
    -lvtkImagingStencil-8.2 \
    -lvtkInfovisCore-8.2 \
    -lvtkInfovisLayout-8.2 \
    -lvtkInteractionImage-8.2 \
    -lvtkInteractionStyle-8.2 \
    -lvtkInteractionWidgets-8.2 \
    -lvtkIOAMR-8.2 \
    -lvtkIOAsynchronous-8.2 \
    -lvtkIOCityGML-8.2 \
    -lvtkIOCore-8.2 \
    -lvtkIOEnSight-8.2 \
    -lvtkIOExodus-8.2 \
    -lvtkIOExport-8.2 \
    -lvtkIOExportPDF-8.2 \
    -lvtkIOGeometry-8.2 \
    -lvtkIOImage-8.2 \
    -lvtkIOImport-8.2 \
    -lvtkIOInfovis-8.2 \
    -lvtkIOLegacy-8.2 \
    -lvtkIOLSDyna-8.2 \
    -lvtkIOMINC-8.2 \
    -lvtkIOMovie-8.2 \
    -lvtkIONetCDF-8.2 \
    -lvtkIOParallel-8.2 \
    -lvtkIOParallelXML-8.2 \
    -lvtkIOPLY-8.2 \
    -lvtkIOSegY-8.2 \
    -lvtkIOSQL-8.2 \
    -lvtkIOTecplotTable-8.2 \
    -lvtkIOVeraOut-8.2 \
    -lvtkIOVideo-8.2 \
    -lvtkIOXML-8.2 \
    -lvtkIOXMLParser-8.2 \
    -lvtklibharu-8.2 \
    -lvtklzma-8.2 \
    -lvtkmetaio-8.2 \
    -lvtknetcdf-8.2 \
    -lvtkParallelCore-8.2 \
    -lvtkpugixml-8.2 \
    -lvtkRenderingAnnotation-8.2 \
    -lvtkRenderingContext2D-8.2 \
    -lvtkRenderingCore-8.2 \
    -lvtkRenderingFreeType-8.2 \
    -lvtkRenderingGL2PSOpenGL2-8.2 \
    -lvtkRenderingImage-8.2 \
    -lvtkRenderingLabel-8.2 \
    -lvtkRenderingLOD-8.2 \
    -lvtkRenderingOpenGL2-8.2 \
    -lvtkRenderingQt-8.2 \
    -lvtkRenderingVolume-8.2 \
    -lvtkRenderingVolumeOpenGL2-8.2 \
    -lvtksqlite-8.2 \
    -lvtksys-8.2 \
    -lvtkverdict-8.2 \
    -lvtkViewsContext2D-8.2 \
    -lvtkViewsCore-8.2 \
    -lvtkViewsInfovis-8.2 \
    -lvtkViewsQt-8.2 \
    -lvtkWrappingTools-8.2 \
    -lvtkDomainsChemistryOpenGL2-8.2 \
    -lvtkRenderingContextOpenGL2-8.2

SOURCES += \
    Global/Interpolation.cpp \
    Global/KDtree.cpp \
    childPage/My_QTreeWidgetItem.cpp \
    childPage/annotation_property_widget.cpp \
    childPage/axis_property_widget.cpp \
    childPage/base_property_widget.cpp \
    childPage/border_property_widget.cpp \
    childPage/color_scale_property.cpp \
    childPage/custom_button_left.cpp \
    childPage/custom_button_right.cpp \
    childPage/cut_plane_property.cpp \
    childPage/exclusion_filter_property_widget.cpp \
    childPage/gridding_property_widget.cpp \
    childPage/highlowmap_property_widget.cpp \
    childPage/isoline_property_widget.cpp \
    childPage/isosurface_property_widget.cpp \
    childPage/myqwidget.cpp \
    childPage/mytable.cpp \
    childPage/new_project.cpp \
    childPage/orthogonal_property_widget.cpp \
    childPage/repeat_filter_property_widget.cpp \
    childPage/scatter_property_widget.cpp \
    childPage/shape_render_property_widget.cpp \
    childPage/slice_property_widget.cpp \
    childPage/source_property_widget.cpp \
    childPage/subset_property_widget.cpp \
    childPage/surface_render_property_widget.cpp \
    childPage/text_property_widget.cpp \
    childPage/transform_property_widget.cpp \
    childPage/vector_property_widget.cpp \
    funcNode/SurperNode.cpp \
    funcNode/funcCaculate/CConversionNode.cpp \
    funcNode/funcCaculate/CMeshNode.cpp \
    funcNode/funcCaculate/CaculateNode.cpp \
    funcNode/funcGraph/GAxisNode.cpp \
    funcNode/funcGraph/GBoundingBoxNode.cpp \
    funcNode/funcGraph/GCommentNode.cpp \
    funcNode/funcGraph/GHighLowMapNode.cpp \
    funcNode/funcGraph/GIsoLineNode.cpp \
    funcNode/funcGraph/GIsoSurfaceNode.cpp \
    funcNode/funcGraph/GOrthogonal_factor.cpp \
    funcNode/funcGraph/GScatterMapNode.cpp \
    funcNode/funcGraph/GSliceMapNode.cpp \
    funcNode/funcGraph/GSurfaceNode.cpp \
    funcNode/funcGraph/GTextNode.cpp \
    funcNode/funcGraph/GVolumeRenderNode.cpp \
    funcNode/funcGraph/GraphNode.cpp \
    funcNode/otherNode/DDataNode.cpp \
    funcNode/otherNode/GlobalInclude.cpp \
    funcNode/otherNode/OColorBar.cpp \
    funcNode/otherNode/OCuttingPlane.cpp \
    logic/OperationLogic.cpp \
    logic/XmlProject.cpp \
    main.cpp \
    operationalLayer/OperationalLayer.cpp \
    test/TxmlProject.cpp \
    threeDdetectSystem.cpp \
    xmlpaser/tinystr.cpp \
    xmlpaser/tinyxml.cpp \
    xmlpaser/tinyxmlerror.cpp \
    xmlpaser/tinyxmlparser.cpp

HEADERS += \
    Global/Interpolation.h \
    Global/KDtree.h \
    childPage/My_QTreeWidgetItem.h \
    childPage/annotation_property_widget.h \
    childPage/axis_property_widget.h \
    childPage/base_property_widget.h \
    childPage/border_property_widget.h \
    childPage/color_scale_property.h \
    childPage/custom_button_left.h \
    childPage/custom_button_right.h \
    childPage/cut_plane_property.h \
    childPage/exclusion_filter_property_widget.h \
    childPage/gridding_property_widget.h \
    childPage/highlowmap_property_widget.h \
    childPage/isoline_property_widget.h \
    childPage/isosurface_property_widget.h \
    childPage/myqwidget.h \
    childPage/mytable.h \
    childPage/new_project.h \
    childPage/orthogonal_property_widget.h \
    childPage/repeat_filter_property_widget.h \
    childPage/scatter_property_widget.h \
    childPage/shape_render_property_widget.h \
    childPage/slice_property_widget.h \
    childPage/source_property_widget.h \
    childPage/subset_property_widget.h \
    childPage/surface_render_property_widget.h \
    childPage/text_property_widget.h \
    childPage/transform_property_widget.h \
    childPage/vector_property_widget.h \
    funcNode/SurperNode.h \
    funcNode/funcCaculate/CConversionNode.h \
    funcNode/funcCaculate/CMeshNode.h \
    funcNode/funcCaculate/CaculateNode.h \
    funcNode/funcGraph/GAxisNode.h \
    funcNode/funcGraph/GBoundingBoxNode.h \
    funcNode/funcGraph/GCommentNode.h \
    funcNode/funcGraph/GHighLowMapNode.h \
    funcNode/funcGraph/GIsoLineNode.h \
    funcNode/funcGraph/GIsoSurfaceNode.h \
    funcNode/funcGraph/GOrthogonal_factor.h \
    funcNode/funcGraph/GScatterMapNode.h \
    funcNode/funcGraph/GSliceMapNode.h \
    funcNode/funcGraph/GSurfaceNode.h \
    funcNode/funcGraph/GTextNode.h \
    funcNode/funcGraph/GVolumeRenderNode.h \
    funcNode/funcGraph/GraphNode.h \
    funcNode/otherNode/DDataNode.h \
    funcNode/otherNode/GlobalInclude.h \
    funcNode/otherNode/OColorBar.h \
    funcNode/otherNode/OCuttingPlane.h \
    logic/OperationLogic.h \
    logic/XmlProject.h \
    operationalLayer/OperationalLayer.h \
    test/TxmlProject.h \
    threeDdetectSystem.h \
    xmlpaser/tinystr.h \
    xmlpaser/tinyxml.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Global/xmlStruct.xml
