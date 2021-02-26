#include "dialog.h"
#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QDate>
//#include "qglobalapplication.h"
#include <QFile>
#include <QDesktopWidget>
#include <QDir>
#include <iostream>
#include <new>
#include <zchxmainwindow.h>
#include "exchange/qexchangerecordworker.h"
#include "date/annualcloseddaydisclosure.h"

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QtAndroid>

bool requestPermission() {
#if QT_VERSION >= QT_VERSION_CHECK(5,12,0)
    QStringList list;
    list.append("android.permission.WRITE_EXTERNAL_STORAGE");
    list.append("android.permission.READ_EXTERNAL_STORAGE");
    QtAndroid::PermissionResultMap result = QtAndroid::requestPermissionsSync(list);
    QtAndroid::PermissionResultMap::const_iterator it = result.begin();
    for(; it != result.end(); it++)
    {
        if(it.value() == QtAndroid::PermissionResult::Denied) return false;
    }
#endif
   return true;
}
#endif

void no_memory () {
  std::cout << "Failed to allocate memory!\n";
  std::exit (1);
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::set_new_handler(no_memory);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    int height = QApplication::desktop()->availableGeometry().height();
    int width = QApplication::desktop()->availableGeometry().width();
    qDebug()<<"device size:"<<height<<width;
    zchxMainWindow w;
#ifdef Q_OS_WIN
//    w.setFixedSize(0.4*height, height*0.9);
    w.showMaximized();
#else
    qDebug()<<"device size:"<<height<<width;
    if(requestPermission())
    {
        w.showMaximized();
    }
#endif
    return a.exec();
}
