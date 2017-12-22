#include "qsinasharevolinfothread.h"
#include "qhttpget.h"
#include "dbservices/dbservices.h"
#include "utils/hqutils.h"
#include <QFile>

#define STOCK_FINANCE_FILE  "stock-financial.data"

//文件结构更新时间+所有的信息

QSinaShareVolInfoThread::QSinaShareVolInfoThread(const QStringList& codes,QObject *parent) : QThread(parent)
{
    mShareCodesList = codes;
}


void QSinaShareVolInfoThread::run()
{
    FinDataList sharelist;
    //检查本地文件是否已经更新过
    bool updatedFromFile = false;
    if(QFile::exists(STOCK_FINANCE_FILE))
    {
        //读取文件
        QFile file(STOCK_FINANCE_FILE);
        if(file.open(QIODevice::ReadOnly))
        {
            int size = file.size();
            if(size >= sizeof(qint64) + sizeof(FINANCE_DATA))
            {
                qint64 lastupdate = 0;
                file.read((char*)(&lastupdate), sizeof(qint64));
                if(QDateTime::fromMSecsSinceEpoch(lastupdate).date() == QDate::currentDate())
                {
                    while (!file.atEnd() ) {
                        FINANCE_DATA data;
                        int res = file.read((char*)(&data), sizeof(FINANCE_DATA));
                        if(res<sizeof(FINANCE_DATA)) break;
                        sharelist.append(data);
                        //qDebug()<<data.mCode<<data.mMGSY<<data.mTotalShare<<data.mJZCSYL;
                    }
                    updatedFromFile = true;
                }
            }

            file.close();
        }
    }

    if(!updatedFromFile)
    {
        //联网更新
        int pos = 0;
        int section = 200;
        while(pos < mShareCodesList.length())
        {
            QStringList sublist = mShareCodesList.mid(pos, section);
            pos += section;
            if(sublist.length() > 0)
            {
                QStringList wklist;
                foreach (QString code, sublist) {
                    code = code.right(6);
                    wklist.append(HqUtils::prefixCode(code) + code+ "_i");
                }
                QString url = QString("http://hq.sinajs.cn/?list=%1").arg(wklist.join(","));
                QString result = QString::fromUtf8(QHttpGet::getContentOfURL(url));
                //按行进行分割
                QStringList rows = result.split(QRegExp("[\\r\\n]"));
                foreach (QString row, rows) {
                    QStringList list = row.split(QRegExp("[,\" ;]"));
                    if(list.length() > 20)
                    {
                        QString code = list[1].mid(9,6);
                        FINANCE_DATA data;
                        data.mCode = code.toInt();
                        data.mMGSY = round(list[6].toDouble()*100);
                        data.mMGJZC = round(list[7].toDouble()*100);
                        data.mTotalShare = qint64(list[9].toDouble() * 10000);
                        data.mMutalShare = qint64(list[10].toDouble() * 10000);
                        data.mJZCSYL = round(list[18].toDouble()*100);
                        sharelist.append(data);
                        //qDebug()<<data.mCode<<data.mMGSY<<data.mTotalShare<<data.mJZCSYL;
                    }
                }
            }
        }
        //将数据写入到文件
        if(sharelist.length() > 0)
        {
            FILE *fp = fopen(STOCK_FINANCE_FILE, "wb+");
            if(fp)
            {
                //首先写入所有财务数据
                qint64 cur = QDateTime::currentDateTime().addDays(-1).toMSecsSinceEpoch();
                fwrite(&cur, sizeof(cur), 1, fp);
                //qDebug()<<"sizeof:"<<sizeof(FINANCE_DATA);
                for(int i=0; i<sharelist.size(); i++){
                    fwrite(&(sharelist[i]), sizeof(FINANCE_DATA), 1, fp);
                }
                //然后在移动到开头写入时间，保证是最新的
                fseek(fp, 0, SEEK_SET);
                cur = QDateTime::currentMSecsSinceEpoch();
                fwrite(&cur, sizeof(cur), 1, fp);
                fclose(fp);
            }
        }
    }
    emit DATA_SERVICE->signalUpdateShareFinanceInfo(sharelist);
    qDebug()<<"update financial info end!!!!!!!!!!";
}
