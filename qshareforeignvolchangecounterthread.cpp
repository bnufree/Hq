#include "qshareforeignvolchangecounterthread.h"
#include "utils/qhttpget.h"
#include "utils/profiles.h"
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>



QShareForeignVolChangeCounterThread::QShareForeignVolChangeCounterThread(QObject *parent) : QThread(parent)
{
    mFileName = QString("%1/hsgt_counter.dat").arg(HQ_LGT_HISTORY_DIR);
    qRegisterMetaType<QList<ShareForeignVolCounter>>("const QList<ShareForeignVolCounter>&");
}

bool QShareForeignVolChangeCounterThread::getData(const QString& type, const QDate& date)
{
    int cur_page = 1;
    bool data_recv = false;
    while (1) {
        QString url = QString("http://dcfm.eastmoney.com/EM_MutiSvcExpandInterface/api/js/get?type=HSGT20_GGTJ_SUM&token=894050c76af8597a853f5b408b759f5d&st=ShareSZ_Chg_One&sr=1&p=%1&ps=5000&js={pages:(tp),data:(x)}&filter=(DateType='%2' and HdDate='%3')&rt=53663676")
                .arg(cur_page).arg(type).arg(date.toString("yyyy-MM-dd"));
        QByteArray bytes = QHttpGet::getContentOfURL(url);
        if(bytes.size() == 0) break;
        bytes.replace("data", "\"data\"");
        bytes.replace("pages", "\"pages\"");
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(bytes, &err);

        if(!doc.isObject()) break;
        QJsonObject obj = doc.object();
        int total_page = obj.value("pages").toInt();
        if(total_page == 0) break;
        QJsonArray array = obj.value("data").toArray();
        foreach(QJsonValue val, array)
        {
            obj = val.toObject();
            int code = obj.value("SCode").toString().toInt();
            ShareForeignVolCounter &data = mDataList[code];
            QString dateType = obj.value("DateType").toString();
            data.mDate = QDateTime::fromString(obj.value("HdDate").toString(), "yyyy-MM-dd").toTime_t();
            data.mCode = obj.value("SCode").toString().toInt();
//            data.mName = obj.value("SName").toString();
//            data.mHYName = obj.value("HYName").toString();
//            data.mHYCode = obj.value("ORIGINALCODE").toString();
            data.mPrice = obj.value("NewPrice").toDouble();
            data.mChangePercent = obj.value("Zdf").toDouble();
            data.mShareHold = obj.value("ShareHold").toDouble();
            data.mShareSZ = obj.value("ShareSZ").toDouble();
            data.mLTZB = obj.value("LTZB").toDouble();
            data.mZZB = obj.value("ZZB").toDouble();
            data.mLTSZ = obj.value("LTSZ").toDouble();
            data.mZSZ = obj.value("ZSZ").toDouble();
            ShareForeignVolChgCounter *counter = 0;
            if(dateType == "1")
            {
                counter = &data.mChg1;
            } else if(dateType == "3")
            {
                counter = &data.mChg3;
            } else if(dateType == "5")
            {
                counter = &data.mChg5;
            } else if(dateType == "10")
            {
                counter = &data.mChg10;
            } else
            {
                counter = &data.mChgM;
            }
            counter->mShareHold_Change = obj.value("ShareHold_Chg_One").toDouble();
            counter->mShareSZ_Change = obj.value("ShareSZ_Chg_One").toDouble();
            counter->mShareRate_Change = obj.value("LTZB_One").toDouble();
//            mDataList.append(data);
            if(!data_recv) data_recv = true;
        }
        cur_page++;
        if(cur_page > total_page) break;
    }

    return data_recv;
}


void QShareForeignVolChangeCounterThread::read()
{
    QList<ShareForeignVolCounter> list;
    if(!QFile::exists(mFileName))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<mFileName<<" not exist";
        return ;
    }
    //读取文件
    QFile file(mFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<mFileName<<" open error";
        return ;
    }
    int size = file.size();
    qint64 totalNum = 0;
    if(size > sizeof(qint64))
    {
        file.read((char*)(&totalNum), sizeof(qint64));
        while (!file.atEnd() ) {
            ShareForeignVolCounter data;
            file.read((char*)(&data), sizeof(ShareForeignVolCounter));
            qDebug()<<data.mCode<<data.mChangePercent<<data.mChg1.mShareHold_Change;
            list.append(data);
        }
    }
    file.close();
    if(list.size() > 0)
    {
        QDateTime date = QDateTime::fromTime_t(totalNum);
        emit signalSendDataList(list, date.date().toString("yyyy-MM-dd"));
    }


    return;
}

void QShareForeignVolChangeCounterThread::write()
{
    if(mDataList.size() == 0) return;
    FILE *fp = fopen(mFileName.toStdString().data(), "wb+");
    if(fp)
    {
        qint64 date = mDataList.first().mDate;
        fwrite(&date, sizeof(date), 1, fp);
        foreach(int key, mDataList.keys())
        {
            ShareForeignVolCounter &data = mDataList[key];
            fwrite(&data, sizeof(ShareForeignVolCounter), 1, fp);
        }
        fclose(fp);
        PROFILES_INS->setValue("Update", "Foreign_counter_update", QDateTime::fromTime_t(date).toString("yyyy-MM-dd"));
    }
}


void QShareForeignVolChangeCounterThread::run()
{
    //首先读取本地文件的信息
    read();

    QString last_date = PROFILES_INS->value("Update", "Foreign_counter_update").toString();
    if(QDate::fromString(last_date, "yyyy-MM-dd") != QDate::currentDate())
    {
        QStringList typeList;
        typeList.append("1");
        typeList.append("3");
        typeList.append("5");
        typeList.append("10");
        typeList.append("m");

        QDate date = QDate::currentDate();

        while (typeList.size() > 0) {
            QString type = typeList.takeFirst();
            while (!getData(type, date))
            {
                date = date.addDays(-1);
            }
        }
        if(mDataList.size() > 0)
        {
            QDateTime date = QDateTime::fromTime_t(mDataList.first().mDate);
            emit signalSendDataList(mDataList.values(), date.date().toString("yyyy-MM-dd"));
            write();
        }
    }
}
