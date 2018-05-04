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
#include "utils/hqutils.h"
#include "utils/sharedata.h"
#include <QEventLoop>

QShareCodesWork::QShareCodesWork(QObject *parent) : mParent(parent),QRunnable()
{
}

QShareCodesWork::~QShareCodesWork()
{
}

void QShareCodesWork::run()
{
    ShareBaseDataList list;
    QByteArray http = QHttpGet::getContentOfURL("http://quote.eastmoney.com/stocklist.html");
    int r = http.indexOf("(600036)");
    //qDebug()<<"mid:"<<http.mid(r - 10, 20);
    QTextCodec *codes = QTextCodec::codecForHtml(http);
    QTextCodec *UTF8 = QTextCodec::codecForName("UTF8");
    QString result = codes->toUnicode(http);
#if 0
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
            ShareBaseData data;
            data.setCode(ShareBaseData::fullCode(code));
            data.setName(name);
            data.setShareType(ShareBaseData::shareType(code));            
            //data.setPY(HqUtils::GetFirstLetter(UTF8->toUnicode(name.toUtf8())));
            qDebug()<<data.mCode<<data.mName;
            list.append(data);
        }
        index += reg.matchedLength();
    }
#else

    QRegExp reg_code("\\((60[013][0-9]{3}|300[0-9]{3}|00[012][0-9]{3}|510[0-9]{3}|1599[0-9]{2})\\)");
    int index = 0;
    while((index = reg_code.indexIn(result, index)) >= 0)
    {
        QString code = reg_code.cap(1);
        ShareBaseData data;
        data.setCode(ShareBaseData::fullCode(code));
        data.setShareType(ShareBaseData::shareType(code));
        qDebug()<<data.mCode;
        list.append(data);
        index += reg_code.matchedLength();
    }
#endif
    if(list.length() > 0 && mParent)
    {
        QMetaObject::invokeMethod(mParent, "slotUpdateShareCodesList", Qt::DirectConnection, Q_ARG(ShareBaseDataList,list ));
    }
    return;
}


