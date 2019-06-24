#include "qhkexchangevoldataprocess.h"
#include "utils/qhttpget.h"
#include <QDir>
#include <QRegularExpression>
#include "utils/comdatadefines.h"
#include <QTextCodec>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#define     POST_VAL        "__VIEWSTATE=%2FwEPDwUJNjIxMTYzMDAwZGQ79IjpLOM%2BJXdffc28A8BMMA9%2Byg%3D%3D&__VIEWSTATEGENERATOR=EC4ACD6F&__EVENTVALIDATION=%2FwEdAAdtFULLXu4cXg1Ju23kPkBZVobCVrNyCM2j%2BbEk3ygqmn1KZjrCXCJtWs9HrcHg6Q64ro36uTSn%2FZ2SUlkm9HsG7WOv0RDD9teZWjlyl84iRMtpPncyBi1FXkZsaSW6dwqO1N1XNFmfsMXJasjxX85jz8PxJxwgNJLTNVe2Bh%2Fbcg5jDf8%3D&today=TODAY_DATE&sortBy=stockcode&sortDirection=asc&alertMsg=&txtShareholdingDate=TXTSHAREDATE&btnSearch=%E6%90%9C%E5%B0%8B"
#define     HK_URL      "https://www.hkexnews.hk/sdw/search/mutualmarket_c.aspx?t=%1&t=%2"
#define     EASTMONEY_URL "http://dcfm.eastmoney.com//em_mutisvcexpandinterface/api/js/get?type=HSGTHDSTA&token=70f12f2f4f091e459a279469fe49eca5&st=SHAREHOLDPRICE&sr=-1&p=%1&ps=100000&js={pages:(tp),data:(x)}&filter=(MARKET%20in%20(%27001%27,%27003%27))(HDDATE=^%2^)&rt=52030379"

QHKExchangeVolDataProcess::QHKExchangeVolDataProcess(const QDate& start, const QDate& end, QObject* parent) : QRunnable()
{
    mStartDate = start;
    mEndDate = end;
    mParent = parent;
}

void QHKExchangeVolDataProcess::getMktVolInfo(ShareDataList& list, int& num, const QDate &date, int mkt)
{
    QString postVal = POST_VAL;
    postVal.replace("TODAY_DATE", QDate::currentDate().toString("yyyyMMdd"));
    postVal.replace("TXTSHAREDATE", QString("").sprintf("%s", date.toString("yyyy/MM/dd").toStdString().data()));
    QByteArray value = QHttpGet::getContentOfURLWithPost(QString(HK_URL).arg(mkt == 0? "sh":"sz").arg(mkt == 0? "sh":"sz"), postVal.toUtf8());
    qDebug()<<"recv len:"<<value.length();
    qDebug()<<value;
    QFile file(QString("https_%1").arg(QDateTime::currentMSecsSinceEpoch()));
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(value);
        file.close();
    } else
    {
        qDebug()<<__FUNCTION__<<"open file error:"<<file.fileName();
    }

    QString res = QString::fromUtf8(value);
    int start_index = 0;
    qDebug()<<__FUNCTION__<<start_index;
    QRegExp codeExp("7[072]{1}[0-9]{3}|9[0-9]{4}");
    //QRegExp nameExp("[\u4e00-\u9fa5A-Z]{1,}");
    QRegExp volExp(">(([0-9]{1,3},){0,}[0-9]{1,})<");
    QRegExp dateExp(": (\\d{4}/\\d{2}/\\d{2})");
    QRegExp volPercentExp("([0-9]{1,3}\.[0-9]{2})%");

    start_index = dateExp.indexIn(res, start_index);
    QDate resDate;
    if(start_index >=0 )
    {
        resDate = QDate::fromString(dateExp.cap(1), "yyyy/MM/dd");
    }
    if(resDate != date) return;

    while ( (start_index = codeExp.indexIn(res, start_index)) >= 0) {
        QString tmpCode = codeExp.cap();
        if(tmpCode.left(1) == "7")
        {
            if(tmpCode.left(2)== "77")
            {
                tmpCode.replace(0, 2, "300");
            } else
            {
                tmpCode.replace(0, 1, "00");
            }
        } else
        {
            tmpCode.replace(0, 1, "60");
        }
        int code = tmpCode.toInt();
        //qDebug()<<"res:"<<codeExp.cap();
        start_index += 5;
        start_index = volExp.indexIn(res, start_index);
        if(start_index >= 0)
        {
            ShareData data;
            data.mCode = tmpCode.right(6);
            data.mHsgtData.mVolTotal = volExp.cap(1).remove(",").toLongLong();
            data.mTime.setDate(date);
            //qDebug()<<"res:"<<volExp.cap();
            start_index += volExp.cap().length();
            start_index = volPercentExp.indexIn(res, start_index);
            if(start_index >= 0)
            {
                data.mHsgtData.mVolMutablePercent = volPercentExp.cap(1).toDouble();
                list.append(data);
                //qDebug()<<data.mCode<<data.mHsgtData.mVolTotal<<data.mHsgtData.mVolMutablePercent;
                start_index += volPercentExp.cap().length();
            } else
            {
                break;
            }

        } else
        {
            //qDebug()<<"code:"<<tmpCode<<"not found";
            break;
        }
//        qDebug()<<list.last();
    }
    num = list.size();
    return;
}

void QHKExchangeVolDataProcess::getMktVolInfo(QStringList& list, const QDate& date,const QString &fileName)
{
    if(!QFile::exists(fileName)) return;
    //读取文件
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) return;
    int size = file.size();
    if(size >= sizeof(int))
    {
        int total_size = 0;
        file.read((char*)(&total_size), sizeof(int));
        int count = 0;
        int code = 0;
        qint64 vol = 0;
        while (!file.atEnd() ) {
            count++;
            if(count == 1)
            {
                file.read((char*)(&code), sizeof(int));
            } else if(count == 2)
            {
                file.read((char*)(&vol), sizeof(qint64));
                list.append(QString("%1|%2").arg(code).arg(vol));
                count = 0;
            }
        }
        //qDebug()<<"total_size:"<<total_size<<" read count:"<<list.size()<<date;
    }
    file.close();
    return ;
}

void QHKExchangeVolDataProcess::getVolofDateFromEastMoney(ShareDataList &list, const QDate &date)
{
    int page = 1;
    int totalpage = 1;
    while (page <= totalpage) {
        QByteArray value = QHttpGet::getContentOfURL(QString(EASTMONEY_URL).arg(page).arg(date.toString("yyyy-MM-dd")));
        value.replace("pages", "\"pages\"");
        value.replace("data", "\"data\"");
        page++;
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(value, &err);
        if(err.error != QJsonParseError::NoError)
        {
            qDebug()<<"error:"<<err.errorString();
            continue;
        }
        if(!doc.isObject()) continue;
        QJsonObject obj = doc.object();
        totalpage = obj.value("pages").toInt();
        QJsonArray array = obj.value("data").toArray();
        for(int i=0; i<array.size(); i++)
        {
            QJsonObject sub = array[i].toObject();
            ShareData data;
            data.mCode = sub.value("SCODE").toString();
            data.mName = sub.value("SNAME").toString();
            data.mTime.setDate(QDate::fromString(sub.value("HDDATE").toString().left(10), "yyyy-MM-dd"));
            data.mHsgtData.mVolTotal = qint64(sub.value("SHAREHOLDSUM").toDouble());
            data.mHsgtData.mVolMutablePercent = sub.value("SHARESRATE").toDouble();
            data.mHsgtData.mVolChange = qint64(sub.value("ShareHoldSumChg").toDouble());
            data.mClose = sub.value("CLOSEPRICE").toDouble();
            data.mChgPercent = sub.value("ZDF").toDouble();
            list.append(data);

        }

    }
    return ;
}

void QHKExchangeVolDataProcess::getVolofDate(ShareDataList &list, const QDate &date)
{
    int errorNUm = 0;
    int sh_num = 0, sz_num = 0;
    do {
        getMktVolInfo(list, sh_num, date, 0);
        getMktVolInfo(list, sz_num, date, 1);
        errorNUm++;
        if(errorNUm == 3)
        {
            break;
        }
    } while (sh_num == 0 && sz_num == 0);

    if((sh_num == 0) ^ (sz_num == 0))
    {
        while (sh_num == 0) {
            getMktVolInfo(list, sh_num, date, 0);
        }
        while (sz_num == 0) {
            getMktVolInfo(list, sz_num, date, 1);
        }
    }
}

void QHKExchangeVolDataProcess::run()
{
    ShareDataList list;
    QDate start = mStartDate;
    while(start <= mEndDate)
    {
        getVolofDate(list, start);
        //getVolofDateFromEastMoney(list, start);
        start = start.addDays(1);
    }
    qDebug()<<__FUNCTION__<<mParent;
    if(mParent)
    {
        qDebug()<<"date:"<<mStartDate.toString("yyyy-MM-dd")<<mEndDate.toString("yyyy-MM-dd")<<list.size();
        QMetaObject::invokeMethod(mParent,\
                                  "slotUpdateForignVolInfo",\
                                  Qt::DirectConnection,\
                                  Q_ARG(ShareDataList, list)\
                                  );
    }
}
