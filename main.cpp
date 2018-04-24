#include "dialog.h"
#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QDate>
//#include "qglobalapplication.h"
#include <QFile>
#include <QDesktopWidget>
#include <QDir>
#include "utils/hqutils.h"
#include "basic_info/qsharecodeswork.h"
#include <QThreadPool>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    //make size as 9:16
//    Dialog w;
//    w.showMaximized();
    QThreadPool pool;
    pool.start(new QShareCodesWork);
    pool.waitForDone();

    return a.exec();
}
