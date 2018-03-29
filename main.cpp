#include "dialog.h"
#include <QApplication>
#include <QTextCodec>
#include "qexchangedatamanage.h"
#include <QDebug>
#include <QDate>
//#include "qglobalapplication.h"
#include <QFile>
#include "basic_info/qsharecodeswork.h"
#include "QThreadPool"
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
#if 0
    QThreadPool pool;
    pool.setExpiryTimeout(-1);
    pool.start(new QShareCodesWork);
    pool.waitForDone();
#else
    qDebug()<<QApplication::desktop()->screenGeometry();
    //make size as 9:16
    Dialog w;
    w.resize(1080*0.3, 1920*0.3);
    w.show();
#endif

    return a.exec();
}
