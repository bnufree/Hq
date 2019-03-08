#include "qsharecodeswork.h"
#include <QDebug>
#include <QDateTime>
#include <QRegularExpression>
#include <QRegExp>
#include <QFile>
#include <QDir>
#include "dbservices/hqdatadefines.h"
#include "dbservices/dbservices.h"
#include <QTextCodec>
#include "utils/qhttpget.h"
#include "data_structure/hqutils.h"
#include "data_structure/sharedata.h"
#include <QEventLoop>

QShareCodesWork::QShareCodesWork(QObject *parent) : mParent(parent),QRunnable()
{
}

QShareCodesWork::~QShareCodesWork()
{
}

void QShareCodesWork::run()
{
    //首先获取数据库的更新日期
    ShareDataList list;
    ShareDate update_date = DATA_SERVICE->getLastUpdateDateOfBasicInfo();
    qDebug()<<update_date.date();
    if(update_date != ShareDate::latestActiveDay())
    {
        QByteArray http = QHttpGet::getContentOfURL("http://quote.eastmoney.com/stocklist.html");
        QTextCodec *codes = QTextCodec::codecForHtml(http);
        //qDebug()<<"code:"<<codes->name();
        QTextCodec *UTF8 = QTextCodec::codecForName("UTF8");
        QString result = codes->toUnicode(http);
        //QRegExp reg(">([\u4e00-\u9fa5A-Z0-9\*]{1,})\\(([0-9]{6})\\)<");
        QRegExp reg(">([\u4e00-\u9fffA-Z0-9\*]{1,})\\(([0-9]{6})\\)<");
        QString utf8_result = QString::fromUtf8(UTF8->fromUnicode(result));
        QRegExp reg_code("60[013][0-9]{3}|300[0-9]{3}|00[012][0-9]{3}|510[0-9]{3}|1599[0-9]{2}");
        int index = 0;
        while((index = reg.indexIn(utf8_result, index)) >= 0)
        {
            QString name = reg.cap(1);
            QString code = reg.cap(2);
            if(reg_code.exactMatch(code))
            {
                ShareData data;
                data.mCode = code;
                data.mName = name;
                data.mShareType = ShareData::shareType(code);
                //qDebug()<<data.mCode<<data.mName<<name.toUtf8().toHex()<<name.toUtf8().size();
                data.mPY = HqUtils::GetFirstLetter(UTF8->toUnicode(name.toUtf8()));
                list.append(data);
            }
            index += reg.matchedLength();
        }
        //qDebug()<<"code length:"<<list.length();
    }
    DATA_SERVICE->signalUpdateShareBasicInfo(list);
    return;
}


