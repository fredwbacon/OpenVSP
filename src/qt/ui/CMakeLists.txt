CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} ${QT_INCLUDE_DIR} )

SET(SCREENS_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE PATH "Path to moc generated *.h files")

QT_UI( screens
aboutDialog.ui
awaveDialog.ui
curveEditDialog.ui
feaStructDialog.ui
labelDialog.ui
massPropertiesDialog.ui
parmDialog.ui
pickSetDialog.ui
planarSlicingDialog.ui
selectFileDialog.ui
setEditorDialog.ui
sliceDialog.ui
textureMgrDialog.ui
typeEditorDialog.ui
)

ADD_LIBRARY(screens ${screens_QT_UI_SRCS} )
