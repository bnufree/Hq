#include "qhkexchangevoldataprocess.h"
#include "qhttpget.h"
#include <QDir>
#include <QRegularExpression>

#define     POST_VAL        "__VIEWSTATE=hfdegH8vXbYKMlAzFdRLabzEpRCND3OAIeQspbX5N8aDTZyKVNgZguPgAGFS2IKx2z7HtjF3bZ2rpgFw3bRIkQjGUivXs1rltkHxELBtK6Gb%2Fc9edA64kxXJCRnJrYPvsDKi3%2BmYQRRioHFrjk%2FXHvE5TC3zeVT07dozLfa2VcdxQXKnbeo5Bps%2F4snmILAYFwYJ1g%3D%3D&__VIEWSTATEGENERATOR=EC4ACD6F&__EVENTVALIDATION=Thovyr9ostDjYwd7geVYart4EmuDg%2FVRN0QVtFX4pGMAD2t2fBZ44pi%2F9W0HRNg%2BH2crk3bxMQJnOiahnz5MxDmItriKi7LKRwr61Iqon2oCatX%2FSJc73rku4ThwGD0kENg8NCFiRlKI6u%2BeiA7yu0eU9QJUlX6S1NA2PibwE0S%2BNp8CaoUSr%2BMbFsn6L7ItTWoxfM%2BsxsqRGVy6Nb1nAI8xS3fxU9PrPD2AyViiGwwl1U9MHImWFhYe3axkt4EM%2BBlw%2BItyoTk67NPmJCfQI4R%2Bt9j1VdQKSg968U0%2FqXhTM0iX%2FO2tsDESWKNZo9Wd2X%2FgplexC9vsWOjAVysRGiuazEmGcLkPGu7jhduJdL3vMXk%2Bt%2Bsn5bTgD%2BH%2F2U8FBamRcQu%2FrJxnYMDrn%2BUeeSIKtYxblzntAVAC%2BmW7DPfWwXqQrw%2BojX2c5WyzXDM2GJclbOs%2BHwdkN3Z%2BYPJeWNH%2FjRwS6b2Fd2khhG%2BiKsX3Dt1dalsAx4n2OlPL6eAerfAgsLx0jc89yw3IVntyS9LXKdKCKqU5ESkxFaWNXYlKl5b%2B8PK6vjSFEEpdmTRbn0voFF1Q4insBbIORE9P085u4lmDWshlLyXlFtkUz5XFaUIHsOoKfZMO9k%2FU%2FSspqs71dFd0crIrjlmJDiMNTFWCIRvb8npTZZELTegoF2CzxIrUvPI5JHD3ue9EF1Shz1pLAqf16lP8cmwa35DOghaXUj3gemmxwxmNqJlDD1xUugll6yFinOnhNYETgX1e00zfOyblBhdZwtorBR7BITHKEFez%2F3g8M9iX4QPceku5XyCMcF77qU%2BHjBw1oz0rbLVqTXNPyN711yQUfeupvaeijhoOtNG%2ByrXyMdVo7d2vxy67%2BVVOA8%2F4MNkjEM0oZnnsHp%2Fv8JQ1FAO7GWWL0l6yTCl2yPm2z4ESd%2BwqWKCG5rCiFWLmFsRqNBSpgz%2Fxe1nKeJaVDYJXH2%2Fwh0jqJTJ1QHYTG10O34WEc2tXcqMAJ2mzJvSmvfo3R5cg0Y8xw4yH%2BQw%2BXeCLLJigyXqn%2F%2BVwhVtRisPCPfQSq4OBEmkoerfl%2BGzf%2FPRrUC6Byw55Qo6mL9RYZWSUkb0mjd0hnXjWmiaNqTQrbYhuqU0DyDbCPfkvWWGu5p7ilCbocg0zEizZEF96uPH%2FvsyHkMCnDbpTwZE%3D&today=TODAY_DATE&sortBy=&alertMsg=&ddlShareholdingDay=TARDAY&ddlShareholdingMonth=TARMONTH&ddlShareholdingYear=TARYEAR&btnSearch.x=26&btnSearch.y=11"

#define     HK_URL      "http://sc.hkexnews.hk/TuniS/www.hkexnews.hk/sdw/search/mutualmarket_c.aspx?t=%1"
#define     SAVE_DIR    QDir::currentPath() + "/hk/"

QHKExchangeVolDataProcess::QHKExchangeVolDataProcess(const QDate& date, QObject* parent) : QRunnable()
{
    mDate = date;
    mParent = parent;
}

void QHKExchangeVolDataProcess::getMktVolInfo(StockDataList &list, const QDate &date, int mkt)
{
    QString postVal = POST_VAL;
    postVal.replace("TODAY_DATE", QDate::currentDate().toString("yyyyMMdd"));
    postVal.replace("TARDAY", QString("").sprintf("%02d", date.day()));
    postVal.replace("TARMONTH", QString("").sprintf("%02d", date.month()));
    postVal.replace("TARYEAR", QString("").sprintf("%04d", date.year()));
    QByteArray value = QHttpGet::getContentOfURLWithPost(QString(HK_URL).arg(mkt == 0? "sh":"sz"), postVal.toLatin1());
    QString res = QString::fromUtf8(value).remove(QRegularExpression("[\\s]"));
    res.remove(QRegularExpression("[\\s]"));
    qDebug()<<"start analysis!!!!!!!!!!!!!!";
    int start_index = 0;
    QRegExp codeExp("7[07]{1}[0-9]{3}|9[0-9]{4}");
    //QRegExp nameExp("[\u4e00-\u9fa5A-Z]{1,}");
    QRegExp volExp("([0-9]{1,3},){1,}[0-9]{1,}");
    QRegExp dateExp("\\d{2}/\\d{2}/\\d{4}");

    start_index = dateExp.indexIn(res, start_index);
    QDate resDate;
    if(start_index >=0 )
    {
        resDate = QDate::fromString(dateExp.cap(), "dd/MM/yyyy");
    } else
    {
        return;
    }
    qDebug()<<resDate<<mDate;
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
            qint64 vol = volExp.cap().remove(",").toLongLong();
            //qDebug()<<"res:"<<volExp.cap();
            start_index += volExp.cap().length();
            StockData data;
            data.mCode = code;
            data.mForeignVol = vol;
            data.mDate = mDate;
            list.append(data);
        } else
        {
            break;
        }
    }

    qDebug()<<"end analysis!!!!!!!!!!!!!!";
    return;
}

void QHKExchangeVolDataProcess::run()
{
    //分别取得各个市场的数据
    StockDataList list;
    for(int i=0; i<2; i++)
    {
        getMktVolInfo(list, mDate, i);
    }
    if(mParent)
    {
        QMetaObject::invokeMethod(mParent,\
                                  "slotUpdateForignVolInfo",\
                                  Qt::DirectConnection,\
                                  Q_ARG(StockDataList, list),\
                                  Q_ARG(QDate, mDate)\
                                  );
        QString msg = QStringLiteral("更新外资持股：") + mDate.toString("yyyy-MM-dd");
        QMetaObject::invokeMethod(mParent,\
                                  "signalUpdateHistoryMsg",\
                                  Qt::DirectConnection,\
                                  Q_ARG(QString,msg )\
                                  );
    }

//    //写入文件保存
//    QDir wkdir(SAVE_DIR);
//    if(!wkdir.exists())
//    {
//        if(wkdir.mkpath(SAVE_DIR))
//        {
//            qDebug()<<"make path "<<SAVE_DIR<<" ok.";
//        } else
//        {
//            qDebug()<<"make path "<<SAVE_DIR<<" falied.";
//        }

//    }
//    QString fileName = QString("%1%2.dat").arg(SAVE_DIR).arg(mDate.toString("yyyyMMdd"));
//    //将数据写入到文件
//    if(list.length() > 0)
//    {
//        FILE *fp = fopen(fileName.toStdString().data(), "wb+");
//        if(fp)
//        {
//            QDateTime wkDateTime;
//            wkDateTime.setDate(mDate);
//            qint64 cur =wkDateTime.addDays(-1).toMSecsSinceEpoch();
//            fwrite(&cur, sizeof(cur), 1, fp);
//            for(int i=0; i<list.size(); i++){
//                fwrite(&(list[i]), sizeof(HKVOLDATA), 1, fp);
//            }
//            //然后在移动到开头写入时间，保证是最新的
//            fseek(fp, 0, SEEK_SET);
//            cur = wkDateTime.toMSecsSinceEpoch();
//            fwrite(&cur, sizeof(cur), 1, fp);
//            fclose(fp);
//        }
//    }
}
