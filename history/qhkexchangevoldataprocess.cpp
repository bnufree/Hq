#include "qhkexchangevoldataprocess.h"
#include "utils/qhttpget.h"
#include <QDir>
#include <QRegularExpression>
#include "utils/comdatadefines.h"
#include <QTextCodec>

#define     POST_VAL        "__VIEWSTATE=%2FwEPDwUJNjIxMTYzMDAwZGSFj8kdzCLeVLiJkFRvN5rjsPotqw%3D%3D&__VIEWSTATEGENERATOR=3C67932C&__EVENTVALIDATION=%2FwEdAAdbi0fj%2BZSDYaSP61MAVoEdVobCVrNyCM2j%2BbEk3ygqmn1KZjrCXCJtWs9HrcHg6Q64ro36uTSn%2FZ2SUlkm9HsG7WOv0RDD9teZWjlyl84iRMtpPncyBi1FXkZsaSW6dwqO1N1XNFmfsMXJasjxX85ju3P1WAPUeweM%2Fr0%2FuwwyYLgN1B8%3D&today=TODAY_DATE&sortBy=stockcode&sortDirection=asc&alertMsg=&txtShareholdingDate=TXTSHAREDATE&btnSearch=Search"
#define     HK_URL      "http://www.hkexnews.hk/sdw/search/mutualmarket.aspx?t=%1"

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
    QByteArray value = QHttpGet::getContentOfURLWithPost(QString(HK_URL).arg(mkt == 0? "sh":"sz"), postVal.toLatin1());
    QString res = QString::fromUtf8(value);
    int start_index = 0;
    qDebug()<<__FUNCTION__<<start_index;
    QRegExp codeExp("7[072]{1}[0-9]{3}|9[0-9]{4}");
    //QRegExp nameExp("[\u4e00-\u9fa5A-Z]{1,}");
    QRegExp volExp(">(([0-9]{1,3},){0,}[0-9]{1,})<");
    QRegExp dateExp("Shareholding Date: (\\d{4}/\\d{2}/\\d{2})");
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
    while(mStartDate <= mEndDate)
    {
        ShareDataList list;
        getVolofDate(list, mStartDate);
        mStartDate = mStartDate.addDays(1);
        if(mParent)
        {
            QMetaObject::invokeMethod(mParent,\
                                      "slotUpdateForignVolInfo",\
                                      Qt::DirectConnection,\
                                      Q_ARG(ShareDataList, list)\
                                      );

        }
    }
}
