#include "qsharefinancialinfowork.h"
#include "utils/qhttpget.h"
#include "dbservices/dbservices.h"
#include "utils/profiles.h"
#include "qetfscalethread.h"
#include <QEventLoop>

QShareFinancialInfoWork::QShareFinancialInfoWork(const QStringList& list, QObject* parent)
    : QThread(parent)
{
    //只更新股票数据，基金数据另外更新
    foreach (QString code, list) {
        int share_type = ShareData::shareType(code);
        if(share_type & SHARE_SHARE_ONLY )
        {
            mShareCodeList.append(code);
        }
    }
}

QShareFinancialInfoWork::~QShareFinancialInfoWork()
{

}

void QShareFinancialInfoWork::run()
{
    QDate last_update_date = DATA_SERVICE->getLastUpdateDateOfFinanceInfo();
    FinancialDataList dataList;

//    if(last_update_date.isNull() || last_update_date < QDate::currentDate())
    {
        //先更新基金规模数据
        QEventLoop loop;
        QEtfScaleThread *etf = new QEtfScaleThread;
        connect(etf,  SIGNAL(finished()), &loop, SLOT(quit()));
        connect(etf,  SIGNAL(finished()), etf, SLOT(deleteLater()));
        etf->start();
        loop.exec();
        //检索保存目录的
        QDir dir(HQ_ETF_DIR);
        QFileInfoList fileList = dir.entryInfoList(QDir::Files, QDir::Name);
        if(fileList.size() >0 )
        {
            QString fileName = fileList.last().filePath();
            QFile file(fileName);
            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                while (!file.atEnd()) {
                    QString line =  QString::fromUtf8(file.readLine());
                    QStringList list = line.split(",");
                    if(line.size() >= 4)
                    {
                        FinancialData data;
                        data.mCode = list[0];
                        data.mEPS = 0;
                        data.mBVPS = 0;
                        data.mTotalShare = list[3].toDouble() * 10000;
                        data.mMutalShare = list[3].toDouble() * 10000;
                        data.mROE = 0;
                        dataList.append(data);
                    }
                }
                file.close();
            }
        }

        int pos = 0;
        int section = 200;
        while(pos < mShareCodeList.length())
        {
            QStringList sublist = mShareCodeList.mid(pos, section);
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
    }
    DATA_SERVICE->signalUpdateShareFinanceInfo(dataList);
}
