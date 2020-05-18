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

void no_memory () {
  std::cout << "Failed to allocate memory!\n";
  std::exit (1);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::set_new_handler(no_memory);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

    QExchangeRecordWorker worker;
    worker.slotStartImport("D:/20200431.xlsx");
#if 0
    zchxMainWindow w;
#ifdef Q_OS_WIN
    w.showMaximized();
#else
    w.showMaximized();
#endif
#endif

    return a.exec();
}
