#include "qsinasharevolinfothread.h"
#include "qhttpget.h"
#include "dbservices/dbservices.h"
#include "utils/hqutils.h"

QSinaShareVolInfoThread::QSinaShareVolInfoThread(const QStringList& codes,QObject *parent) : QThread(parent)
{
    mShareCodesList = codes;
}

void QSinaShareVolInfoThread::run()
{
    StockDataList sharelist;
    foreach (QString code, mShareCodesList) {
        code = code.right(6);
        QString prefix = HqUtils::prefixCode(code);
        QString url = QString("http://hq.sinajs.cn/?list=%1%2_i").arg(prefix).arg(code);
        QString result = QString::fromUtf8(QHttpGet::getContentOfURL(url));
        QStringList list = result.split(QRegExp("[,\" ;]"));
        if(list.length() > 20)
        {
            QString code = list[1].mid(9,6);
            StockData data;
            data.mCode = code;
            data.mMGSY = list[6].toDouble();
            data.mMGJZC = list[7].toDouble();
            data.mTotalShare = qint64(list[9].toDouble() * 10000);
            data.mMutableShare = qint64(list[10].toDouble() * 10000);
            data.mJZCSYL = list[18].toDouble();
            sharelist.append(data);
        }
    }
    emit DATA_SERVICE->signalUpdateShareFinanceInfo(sharelist);
}
