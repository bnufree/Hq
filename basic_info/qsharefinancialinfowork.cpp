#include "qsharefinancialinfowork.h"
#include "qhttpget.h"
#include "utils/sharedata.h"

QShareFinancialInfoWork::QShareFinancialInfoWork(const QStringList& list, QObject* parent)
    :mCodesList(list),mParent(parent),QRunnable()
{

}

QShareFinancialInfoWork::~QShareFinancialInfoWork()
{

}

void QShareFinancialInfoWork::run()
{
    ShareBaseDataList dataList;
    QStringList wklist;
    foreach (QString code, mCodesList) {
        code = code.right(6);
        wklist.append(ShareBaseData::prefixCode(code) + code+ "_i");
    }
    QString url = QString("http://hq.sinajs.cn/?list=%1").arg(wklist.join(","));
    QString result = QString::fromUtf8(QHttpGet::getContentOfURL(url));
    //按行进行分割
    QStringList rows = result.split(QRegExp("[\\r\\n]"));
    foreach (QString row, rows) {
        QStringList list = row.split(QRegExp("[,\" ;]"));
        if(list.length() > 20)
        {
            ShareBaseData data;
            data.setCode(ShareBaseData::fullCode(list[1].mid(9,6)));
            data.setPY(list[3].toUpper());
            data.mMGSY = list[6].toDouble();
            data.mMGJZC = list[7].toDouble();
            data.mTotalShare = qint64(list[9].toDouble() * 10000);
            data.mMutalShare = qint64(list[10].toDouble() * 10000);
            data.mJZCSYL = list[18].toDouble();
            dataList.append(data);

        }
    }
    if(dataList.size() > 0 && mParent){
        QMetaObject::invokeMethod(mParent, "slotUpdateShareFinancialList", Qt::DirectConnection, Q_ARG(ShareBaseDataList,dataList ));
    }

}
