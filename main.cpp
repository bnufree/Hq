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
    Dialog w;
    w.show();
#endif

    return a.exec();
}
