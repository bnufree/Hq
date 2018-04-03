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
#include <QDir>

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
    QRect rect = QApplication::desktop()->screenGeometry();
    qDebug()<<__func__<<rect<<QDir::currentPath();
    //make size as 9:16
    Dialog w;
    w.resize(rect.height() / 1.77 , rect.height());
    w.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    w.show();
#endif

    return a.exec();
}
