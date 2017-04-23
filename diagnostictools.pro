
DEFINES += DIAGNOSTICTOOLS_LIBRARY

# DiagnosticTools files

INCLUDEPATH += datavisualiser

SOURCES += diagnostictoolsplugin.cpp \
    diagnostictoolsfactory.cpp \
    dataqueryengine/dataqueryengine.cpp \
    datavisualiser/datavisualiser.cpp \
    datavisualiser/plotwidget.cpp

HEADERS += diagnostictoolsplugin.h \
    diagnostictools_global.h \
    diagnostictoolsconstants.h \
    diagnostictoolsfactory.h \
    dataqueryengine/dataqueryengine.h \
    datavisualiser/datavisualiser.h \
    datavisualiser/plotwidget.h

FORMS += \
    datavisualiser/datavisualiser.ui

# Qt Creator linking

## Either set the IDE_SOURCE_TREE when running qmake,
## or set the QTC_SOURCE environment variable, to override the default setting
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = $$(QTC_SOURCE)
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = "D:/Solomin/plugin_develop/qt-creator-opensource-src-4.2.1"
# /home/solomin/plugin_develop/qt-creator-opensource-src-4.2.1
#"D:/Solomin/plugin_develop/qt-creator-opensource-src-4.2.1"

## Either set the IDE_BUILD_TREE when running qmake,
## or set the QTC_BUILD environment variable, to override the default setting
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = "D:/Solomin/plugin_develop/build-qtcreator-Qt_5_8_0_vc_2015_x32-Release"
# /home/solomin/plugin_develop/qtcreator-4.2.1
#"D:/Solomin/plugin_develop/build-qtcreator-Qt_5_8_0_vc_2015_x32-Release"

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on OS X
# USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = diagnostictools
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$IDE_SOURCE_TREE/src/qtcreatorplugin.pri)

## path where build is located
#PUBLIC_HEADERS = $$HEADERS
#exportHeaders.input = PUBLIC_HEADERS
#QMAKE_EXTRA_COMPILERS += exportHeaders

#PUBLIC_FORMS = $$FORMS
#exportForms.input = PUBLIC_FORMS
#QMAKE_EXTRA_COMPILERS += exportForms

