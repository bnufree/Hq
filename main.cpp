#include "dialog.h"
#include <QApplication>
#include <QTextCodec>
#include "qexchangedatamanage.h"
#include <QDebug>
#include <QDate>
//#include "qglobalapplication.h"
#include "qhttpget.h"
#include <QFile>
#include "utils/hqutils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
#if 0
    QString src = QStringLiteral("招商银行");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF8");
    qDebug()<<HqUtils::GetFirstLetter(utf8->toUnicode(src.toStdString().data()))<<"src:"<<src;
#else
    Dialog w;
    w.showMaximized();
#endif

    return a.exec();
}
