#include "qsharefinancialinfowork.h"
#include "utils/qhttpget.h"
#include "dbservices/dbservices.h"

QShareFinancialInfoWork::QShareFinancialInfoWork(QObject* parent)
    :mParent(parent),QRunnable()
{

}

QShareFinancialInfoWork::~QShareFinancialInfoWork()
{

}

void QShareFinancialInfoWork::run()
{
    ShareDate last_update_date = DATA_SERVICE->getLastUpdateDateOfFinanceInfo();
    if(!last_update_date.isNull() && last_update_date >= ShareDate::latestActiveDay()) return;
    FinancialDataList dataList;

    QStringList allCodes = DATA_SERVICE->getAllShareCodes();
    int pos = 0;
    int section = 200;
    while(pos < allCodes.length())
    {
        QStringList sublist = allCodes.mid(pos, section);
        pos += section;
        QStringList wklist;
        foreach (QString code, sublist)
        {
            code = code.right(6);
            wklist.append(ShareData::prefixCode(code) + code+ "_i");
        }
        QString url = QString("http://hq.sinajs.cn/?list=%1").arg(wklist.join(","));
        QString result = QString::fromUtf8(QHttpGet::getContentOfURL(url));
        //按行进行分割
        QStringList rows = result.split(QRegExp("[\\r\\n]"));
        foreach (QString row, rows) {
            QStringList list = row.split(QRegExp("[,\" ;]"));
            if(list.length() > 20)
            {
                FinancialData data;
                data.mCode = list[1].mid(9,6);
                data.mEPS = list[6].toDouble();
                data.mBVPS = list[7].toDouble();
                data.mTotalShare = list[9].toDouble() * 10000;
                data.mMutalShare = list[10].toDouble() * 10000;
                data.mROE = list[18].toDouble();
                dataList.append(data);
            }
        }
    }
    if(dataList.size() > 0)
    {
        DATA_SERVICE->signalUpdateShareFinanceInfo(dataList);
    }

}
