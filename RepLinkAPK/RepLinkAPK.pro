#-------------------------------------------------
#
# Project created by QtCreator 2016-02-03T09:31:52
#
#-------------------------------------------------

QT       += core gui xml winextras sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RepLinkAPK
TEMPLATE = app

RC_ICONS+=replinkAPKico.ico \
          apkfile.ico

SOURCES += main.cpp \
           mainwindow.cpp \
    cxmlmethods.cpp \
    caaptparser.cpp \
    manualdescription.cpp \
    procHilo.cpp \
    appsview.cpp \
    appinfo.cpp \
    countHilo.cpp \
    listHilo.cpp \
    jsonHilo.cpp \
    backupHilo.cpp \
    updateHilo.cpp \
    previewHilo.cpp \
    reportHilo.cpp \
    tohtml.cpp \
    apkreport.cpp \
    cabout.cpp \
    apkinfoview.cpp \
    pkglistview.cpp \
    pkginfo.cpp \
    processing.cpp \
    catchooser.cpp \
    sortHilo.cpp \
    crepomanager.cpp \
    repoeditorview.cpp \
    cateditor.cpp \
    cconfig.cpp \
    apkfilesviewer.cpp \
    csecurity.cpp

HEADERS  += mainwindow.h \
          listaSE.h \
    nodoSE.h \
    cxmlapp.h \
    cxmlpackage.h \
    cxmlmethods.h \
    caaptparser.h \
    manualdescription.h\
    procHilo.h \
    appsview.h \
    appinfo.h \
    countHilo.h \
    tohtml.h \
    listHilo.h \
    jsonHilo.h \
    backupHilo.h \
    updateHilo.h \
    previewHilo.h \
    reportHilo.h \
    apkreport.h \
    cabout.h \
    apkinfoview.h \
    pkglistview.h \
    pkginfo.h \
    processing.h \
    newapkfile.h \
    catchooser.h \
    sortHilo.h \
    crepomanager.h \
    dataccess.h \
    repoinfo.h \
    repoeditorview.h \
    cxmlrepo.h \
    cateditor.h \
    keystoreinfo.h \
    cconfig.h \
    apkpreview.h \
    apkfilesviewer.h \
    csecurity.h

FORMS    += mainwindow.ui \
    manualdescription.ui \
    appsview.ui \
    apkreport.ui \
    cabout.ui \
    apkinfoview.ui \
    pkglistview.ui \
    processing.ui \
    catchooser.ui \
    crepomanager.ui \
    repoeditorview.ui \
    cateditor.ui \
    cconfig.ui \
    apkfilesviewer.ui

RESOURCES += \
    check.qrc
