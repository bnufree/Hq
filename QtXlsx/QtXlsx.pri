QTXLSX_NAME        = QtXlsx
MINGW_QWT_VERSION     = 1.0.0
######################################################################
# 3rdparty path
######################################################################
QTXLSX_PATH = $$PWD

exists( $${QTXLSX_PATH} ) {
    message($${QTXLSX_PATH})
    ######################################################################
    # Include library
    ######################################################################
    QTXLSX_HEADERS   = $${QTXLSX_PATH}/include
    QTXLSX_LIBS      = $${QTXLSX_PATH}/lib

    INCLUDEPATH += $${QTXLSX_HEADERS}

    CONFIG(release, debug|release) {
        LIBS +=-L$${QTXLSX_LIBS}/ -llibQt5Xlsx
        QTXLSX_install.files += $${QTXLSX_PATH}/bin/Qt5Xlsx.dll
    }else{
        LIBS +=-L$${QTXLSX_LIBS}/ -llibQt5Xlsxd
        QTXLSX_install.files += $${QTXLSX_PATH}/bin/Qt5Xlsxd.dll
    }

    QTXLSX_install.path = $${IDE_APP_PATH}/
    INSTALLS += QTXLSX_install
}

!exists( $${QTXLSX_PATH} ) {
    warning("Cann't find QTXLSX_PATH DIR =======================")
}
