#include "dialog.h"
#include <QApplication>
#include <QTextCodec>
#include "qexchangedatamanage.h"
#include <QDebug>
#include <QDate>
//#include "qglobalapplication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
    Dialog w;
    w.show();

//    qDebug()<<QExchangeDataManage::instance()->GetUnExchangeDays();
//    qDebug()<<QExchangeDataManage::instance()->IsActiveDay(QDate::currentDate().addDays(2));


    return a.exec();
}
