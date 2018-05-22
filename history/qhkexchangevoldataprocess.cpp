#include "qhkexchangevoldataprocess.h"
#include "utils/qhttpget.h"
#include <QDir>
#include <QRegularExpression>
#include "utils/comdatadefines.h"

#define     POST_VAL        "__VIEWSTATE=hfdegH8vXbYKMlAzFdRLabzEpRCND3OAIeQspbX5N8aDTZyKVNgZguPgAGFS2IKx2z7HtjF3bZ2rpgFw3bRIkQjGUivXs1rltkHxELBtK6Gb%2Fc9edA64kxXJCRnJrYPvsDKi3%2BmYQRRioHFrjk%2FXHvE5TC3zeVT07dozLfa2VcdxQXKnbeo5Bps%2F4snmILAYFwYJ1g%3D%3D&__VIEWSTATEGENERATOR=EC4ACD6F&__EVENTVALIDATION=Thovyr9ostDjYwd7geVYart4EmuDg%2FVRN0QVtFX4pGMAD2t2fBZ44pi%2F9W0HRNg%2BH2crk3bxMQJnOiahnz5MxDmItriKi7LKRwr61Iqon2oCatX%2FSJc73rku4ThwGD0kENg8NCFiRlKI6u%2BeiA7yu0eU9QJUlX6S1NA2PibwE0S%2BNp8CaoUSr%2BMbFsn6L7ItTWoxfM%2BsxsqRGVy6Nb1nAI8xS3fxU9PrPD2AyViiGwwl1U9MHImWFhYe3axkt4EM%2BBlw%2BItyoTk67NPmJCfQI4R%2Bt9j1VdQKSg968U0%2FqXhTM0iX%2FO2tsDESWKNZo9Wd2X%2FgplexC9vsWOjAVysRGiuazEmGcLkPGu7jhduJdL3vMXk%2Bt%2Bsn5bTgD%2BH%2F2U8FBamRcQu%2FrJxnYMDrn%2BUeeSIKtYxblzntAVAC%2BmW7DPfWwXqQrw%2BojX2c5WyzXDM2GJclbOs%2BHwdkN3Z%2BYPJeWNH%2FjRwS6b2Fd2khhG%2BiKsX3Dt1dalsAx4n2OlPL6eAerfAgsLx0jc89yw3IVntyS9LXKdKCKqU5ESkxFaWNXYlKl5b%2B8PK6vjSFEEpdmTRbn0voFF1Q4insBbIORE9P085u4lmDWshlLyXlFtkUz5XFaUIHsOoKfZMO9k%2FU%2FSspqs71dFd0crIrjlmJDiMNTFWCIRvb8npTZZELTegoF2CzxIrUvPI5JHD3ue9EF1Shz1pLAqf16lP8cmwa35DOghaXUj3gemmxwxmNqJlDD1xUugll6yFinOnhNYETgX1e00zfOyblBhdZwtorBR7BITHKEFez%2F3g8M9iX4QPceku5XyCMcF77qU%2BHjBw1oz0rbLVqTXNPyN711yQUfeupvaeijhoOtNG%2ByrXyMdVo7d2vxy67%2BVVOA8%2F4MNkjEM0oZnnsHp%2Fv8JQ1FAO7GWWL0l6yTCl2yPm2z4ESd%2BwqWKCG5rCiFWLmFsRqNBSpgz%2Fxe1nKeJaVDYJXH2%2Fwh0jqJTJ1QHYTG10O34WEc2tXcqMAJ2mzJvSmvfo3R5cg0Y8xw4yH%2BQw%2BXeCLLJigyXqn%2F%2BVwhVtRisPCPfQSq4OBEmkoerfl%2BGzf%2FPRrUC6Byw55Qo6mL9RYZWSUkb0mjd0hnXjWmiaNqTQrbYhuqU0DyDbCPfkvWWGu5p7ilCbocg0zEizZEF96uPH%2FvsyHkMCnDbpTwZE%3D&today=TODAY_DATE&sortBy=&alertMsg=&ddlShareholdingDay=TARDAY&ddlShareholdingMonth=TARMONTH&ddlShareholdingYear=TARYEAR&btnSearch.x=26&btnSearch.y=11"

#define     HK_URL      "http://sc.hkexnews.hk/TuniS/www.hkexnews.hk/sdw/search/mutualmarket_c.aspx?t=%1"

QHKExchangeVolDataProcess::QHKExchangeVolDataProcess(const QDate& date, QObject* parent) : QRunnable()
{
    mDate = date;
    mParent = parent;
}

void QHKExchangeVolDataProcess::getMktVolInfo(QStringList& list, int& num, const QDate &date, int mkt)
{
    QString postVal = POST_VAL;
    postVal.replace("TODAY_DATE", QDate::currentDate().toString("yyyyMMdd"));
    postVal.replace("TARDAY", QString("").sprintf("%02d", date.day()));
    postVal.replace("TARMONTH", QString("").sprintf("%02d", date.month()));
    postVal.replace("TARYEAR", QString("").sprintf("%04d", date.year()));
    QByteArray value = QHttpGet::getContentOfURLWithPost(QString(HK_URL).arg(mkt == 0? "sh":"sz"), postVal.toLatin1());
    QString res = QString::fromUtf8(value).remove(QRegularExpression("[\\s]"));
    res.remove(QRegularExpression("[\\s]"));
//    qDebug()<<"start analysis!!!!!!!!!!!!!!";
    int start_index = 0;
    QRegExp codeExp("7[072]{1}[0-9]{3}|9[0-9]{4}");
    //QRegExp nameExp("[\u4e00-\u9fa5A-Z]{1,}");
    QRegExp volExp(">(([0-9]{1,3},){0,}[0-9]{1,})<");
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
 //   qDebug()<<resDate<<mDate;
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
            qint64 vol = volExp.cap(1).remove(",").toLongLong();
            //qDebug()<<"res:"<<volExp.cap();
            start_index += volExp.cap().length();
            list.append(QString("%1|%2").arg(code).arg(vol));
        } else
        {
            //qDebug()<<"code:"<<tmpCode<<"not found";
            break;
        }
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

void QHKExchangeVolDataProcess::run()
{
    QString fileName = QString("%1%2").arg(HQ_LGTHISTORY_DIR).arg(mDate.toString("yyyyMMdd"));
    //分别取得各个市场的数据
    QStringList list;
    //检查文件是否存在，存在的话直接从文件读取
    getMktVolInfo(list, mDate, fileName);
    if(list.size() == 0)
    {
        int errorNUm = 0;
        int sh_num = 0, sz_num = 0;
        do {
            getMktVolInfo(list, sh_num, mDate, 0);
            getMktVolInfo(list, sz_num, mDate, 1);
            errorNUm++;
            if(errorNUm == 3)
            {
                break;
            }
        } while (sh_num == 0 && sz_num == 0);

        if((sh_num == 0) ^ (sz_num == 0))
        {
            while (sh_num == 0) {
                getMktVolInfo(list, sh_num, mDate, 0);
            }
            while (sz_num == 0) {
                getMktVolInfo(list, sz_num, mDate, 1);
            }
        }
        //写入文件保存
        //将数据写入到文件
        if(list.size() > 0)
        {
            FILE *fp = fopen(fileName.toStdString().data(), "wb+");
            if(fp)
            {
                //写入列表的长度
                int size = list.size();
                fwrite(&size, sizeof(int), 1, fp);
                foreach (QString str, list) {
                    QStringList wk = str.split("|");
                    if(wk.size() >= 2)
                    {
                        int code = wk[0].toInt();
                        qint64 val = wk[1].toLongLong();
                        fwrite(&code, sizeof(int), 1, fp);
                        fwrite(&val, sizeof(qint64), 1, fp);
                    }
                }
                fclose(fp);
            }
        }
    }
    if(mParent)
    {
        QMetaObject::invokeMethod(mParent,\
                                  "slotUpdateForignVolInfo",\
                                  Qt::DirectConnection,\
                                  Q_ARG(QStringList, list),\
                                  Q_ARG(QDate, mDate)\
                                  );

    }
}
