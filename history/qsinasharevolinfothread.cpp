#include "qsinasharevolinfothread.h"
#include "qhttpget.h"
#include "dbservices/dbservices.h"
#include "utils/hqutils.h"
#include <QFile>
#include <QMap>

#define Share_FINANCE_FILE  "share_financial.dat"

//文件结构更新时间+所有的信息

QSinaShareVolInfoThread::QSinaShareVolInfoThread(const QStringList& codes,QObject *parent) : QThread(parent)
{
    mShareCodesList = codes;
}


void QSinaShareVolInfoThread::run2()
{
    QMap<int, FINANCE_DATA> sharelist;
    //检查本地文件是否已经更新过
    bool updatedFromFile = false;
    if(QFile::exists(Share_FINANCE_FILE))
    {
        //读取文件
        QFile file(Share_FINANCE_FILE);
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
                        sharelist[data.mCode] = data;
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
                        sharelist[data.mCode] = data;
                        //qDebug()<<data.mCode<<data.mMGSY<<data.mTotalShare<<data.mJZCSYL;
                    }
                }
            }
        }
        //获取分红参数
        //updateFHSPInfoWithDate(sharelist, "2017-12-31");
        //将数据写入到文件
        if(sharelist.size() > 0)
        {
            FILE *fp = fopen(Share_FINANCE_FILE, "wb+");
            if(fp)
            {
                //首先写入所有财务数据
                qint64 cur = QDateTime::currentDateTime().addDays(-1).toMSecsSinceEpoch();
                fwrite(&cur, sizeof(cur), 1, fp);
                //qDebug()<<"sizeof:"<<sizeof(FINANCE_DATA);
                foreach (int key, sharelist.keys()) {
                    FINANCE_DATA data = sharelist.value(key);
//                    qDebug()<<data.mCode<<data.mSZBL<<data.mXJFH<<data.mMGSY<<data.mJZCSYL;
                    fwrite(&data, sizeof(FINANCE_DATA), 1, fp);
                }
                //然后在移动到开头写入时间，保证是最新的
                fseek(fp, 0, SEEK_SET);
                cur = QDateTime::currentMSecsSinceEpoch();
                fwrite(&cur, sizeof(cur), 1, fp);
                fclose(fp);
            }
        }
    }
    //emit DATA_SERVICE->signalUpdateShareFinanceInfo(sharelist.values());
    qDebug()<<"update financial info end!!!!!!!!!!";
}

void QSinaShareVolInfoThread::updateFHSPInfoWithDate(QMap<QString, ShareData>& map, const QString &date)
{
    //wkdate = 2016-12-31
    qDebug()<<__FUNCTION__<<__LINE__;
    QString url("http://data.eastmoney.com/DataCenter_V3/yjfp/getlist.ashx?js=var vWLdLOFe&pagesize=5000&page=1&sr=-1&sortType=SZZBL&mtk=%C8%AB%B2%BF%B9%C9%C6%B1&filter=(ReportingPeriod=^%1^)&rt=49499306");
    QString wkURL = url.arg(date);
    //结果分析
    QString result = QString::fromUtf8(QHttpGet::getContentOfURL(wkURL));
    int startindex = -1, endindex = -1;
    startindex = result.indexOf("[{");
    endindex = result.indexOf("}]");
    if(startindex < 0 || endindex < 0)
    {
        qDebug()<<__FUNCTION__<<__LINE__;
        return;
    }
    endindex += 2;
    QString hqStr = result.mid(startindex, endindex - startindex);

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(hqStr.toUtf8().data(), &err);
    if(err.error != QJsonParseError::NoError)
    {
        qDebug()<<__FUNCTION__<<__LINE__;
        return;
    }

    if(!doc.isArray())
    {
        qDebug()<<"json with wrong format.";
        qDebug()<<__FUNCTION__<<__LINE__;
        return;
    }
    qDebug()<<__FUNCTION__<<__LINE__;
    QJsonArray array = doc.array();
    for(int i=0; i<array.count(); i++)
    {
        QJsonValue value = array.at(i);
        if(!value.isObject()) continue;
        QJsonObject subobj = value.toObject();
        //开始解析角色信息数据
        ShareData &data = map[subobj.value("Code").toString()];
        data.mSZZG = subobj.value("SZZBL").toString().toDouble();
        data.mXJFH = subobj.value("XJFH").toString().toDouble()/10;
        data.mGQDJR = QDateTime(HqUtils::dateFromStr(subobj.value("GQDJR").toString().left(10))).toMSecsSinceEpoch();
        data.mYAGGR = QDateTime(HqUtils::dateFromStr(subobj.value("YAGGR").toString().left(10))).toMSecsSinceEpoch();
    }
}


void QSinaShareVolInfoThread::run()
{
    //检查当前的更新日期
    QDate date = DATA_SERVICE->getLastUpdateDateOfBasicInfo();
    if(date == HqUtils::latestActiveDay()) return;
    //联网更新
    QMap<QString, ShareData> shareList;
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
                    ShareData data;
                    data.setCode(list[1].mid(9,6));
                    data.setPY(list[3].toUpper());
                    data.mMGSY = list[6].toDouble();
                    data.mMGJZC = list[7].toDouble();
                    data.mTotalShare = qint64(list[9].toDouble() * 10000);
                    data.mMutalShare = qint64(list[10].toDouble() * 10000);
                    data.mJZCSYL = list[18].toDouble();
                    shareList[data.mCode] = data;
                }
            }
        }
    }
    //获取分红参数
    updateFHSPInfoWithDate(shareList, "2017-12-31");
    emit DATA_SERVICE->signalUpdateShareBasicInfo(shareList.values());
    qDebug()<<"update financial info end!!!!!!!!!!";

    return;
}

