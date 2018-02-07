#include "qhkexchangevoldataprocess.h"

QHKExchangeVolDataProcess::QHKExchangeVolDataProcess(QObject *parent) : QObject(parent)
{

}


//QByteArray post("__VIEWSTATE=hfdegH8vXbYKMlAzFdRLabzEpRCND3OAIeQspbX5N8aDTZyKVNgZguPgAGFS2IKx2z7HtjF3bZ2rpgFw3bRIkQjGUivXs1rltkHxELBtK6Gb%2Fc9edA64kxXJCRnJrYPvsDKi3%2BmYQRRioHFrjk%2FXHvE5TC3zeVT07dozLfa2VcdxQXKnbeo5Bps%2F4snmILAYFwYJ1g%3D%3D&__VIEWSTATEGENERATOR=EC4ACD6F&__EVENTVALIDATION=Thovyr9ostDjYwd7geVYart4EmuDg%2FVRN0QVtFX4pGMAD2t2fBZ44pi%2F9W0HRNg%2BH2crk3bxMQJnOiahnz5MxDmItriKi7LKRwr61Iqon2oCatX%2FSJc73rku4ThwGD0kENg8NCFiRlKI6u%2BeiA7yu0eU9QJUlX6S1NA2PibwE0S%2BNp8CaoUSr%2BMbFsn6L7ItTWoxfM%2BsxsqRGVy6Nb1nAI8xS3fxU9PrPD2AyViiGwwl1U9MHImWFhYe3axkt4EM%2BBlw%2BItyoTk67NPmJCfQI4R%2Bt9j1VdQKSg968U0%2FqXhTM0iX%2FO2tsDESWKNZo9Wd2X%2FgplexC9vsWOjAVysRGiuazEmGcLkPGu7jhduJdL3vMXk%2Bt%2Bsn5bTgD%2BH%2F2U8FBamRcQu%2FrJxnYMDrn%2BUeeSIKtYxblzntAVAC%2BmW7DPfWwXqQrw%2BojX2c5WyzXDM2GJclbOs%2BHwdkN3Z%2BYPJeWNH%2FjRwS6b2Fd2khhG%2BiKsX3Dt1dalsAx4n2OlPL6eAerfAgsLx0jc89yw3IVntyS9LXKdKCKqU5ESkxFaWNXYlKl5b%2B8PK6vjSFEEpdmTRbn0voFF1Q4insBbIORE9P085u4lmDWshlLyXlFtkUz5XFaUIHsOoKfZMO9k%2FU%2FSspqs71dFd0crIrjlmJDiMNTFWCIRvb8npTZZELTegoF2CzxIrUvPI5JHD3ue9EF1Shz1pLAqf16lP8cmwa35DOghaXUj3gemmxwxmNqJlDD1xUugll6yFinOnhNYETgX1e00zfOyblBhdZwtorBR7BITHKEFez%2F3g8M9iX4QPceku5XyCMcF77qU%2BHjBw1oz0rbLVqTXNPyN711yQUfeupvaeijhoOtNG%2ByrXyMdVo7d2vxy67%2BVVOA8%2F4MNkjEM0oZnnsHp%2Fv8JQ1FAO7GWWL0l6yTCl2yPm2z4ESd%2BwqWKCG5rCiFWLmFsRqNBSpgz%2Fxe1nKeJaVDYJXH2%2Fwh0jqJTJ1QHYTG10O34WEc2tXcqMAJ2mzJvSmvfo3R5cg0Y8xw4yH%2BQw%2BXeCLLJigyXqn%2F%2BVwhVtRisPCPfQSq4OBEmkoerfl%2BGzf%2FPRrUC6Byw55Qo6mL9RYZWSUkb0mjd0hnXjWmiaNqTQrbYhuqU0DyDbCPfkvWWGu5p7ilCbocg0zEizZEF96uPH%2FvsyHkMCnDbpTwZE%3D&today=20180207&sortBy=&alertMsg=&ddlShareholdingDay=10&ddlShareholdingMonth=01&ddlShareholdingYear=2018&btnSearch.x=26&btnSearch.y=11");
//QByteArray value = QHttpGet::getContentOfURLWithPost("http://sc.hkexnews.hk/TuniS/www.hkexnews.hk/sdw/search/mutualmarket_c.aspx?t=sh", post);
//QString res = QString::fromUtf8(value).remove(QRegularExpression("[\\s]"));
//res.remove(QRegularExpression("[\\s]"));
//qDebug()<<"start analysis!!!!!!!!!!!!!!";
//int start_index = 0;
//int end_index = 0;
////QRegExp codeExp("7[07]{1}[0-9]{3}");
//QRegExp codeExp("9[0-9]{4}");
//QRegExp nameExp("[\u4e00-\u9fa5A-Z]{1,}");
//QRegExp volExp("([0-9]{1,3},){1,}[0-9]{1,}");
//while ( (start_index = codeExp.indexIn(res, start_index)) >= 0) {
//    qDebug()<<"res:"<<codeExp.cap();
//    start_index += 5;
//    start_index = volExp.indexIn(res, start_index);
//    qDebug()<<"res:"<<volExp.cap();
//    start_index += volExp.cap().length();
//}
////    start_index = res.indexOf("<tr class", start_index);
////    if(start_index >= 0)
////    {
////        end_index = res.lastIndexOf("</tr");
////    }
////    qDebug()<<"start:"<<start_index<<" end:"<<end_index;
////    if(end_index > start_index)
////    {
////        QString substr = res.mid(start_index, end_index - start_index + 1);
////        qDebug()<<"substr:"<<substr;
////    }
////    while ((start_index = res.indexOf("<tr class=\"row0\">", start_index)) >= 0) {
////        end_index = res.indexOf("</tr>", start_index);
////        if(end_index < 0)
////        {
////            break;
////        }
////        QString substr = res.mid(start_index, end_index - start_index + 1);
////        qDebug()<<"substr:"<<substr.remove(QRegularExpression("[<> a-z:;=\"\\r\\n\-]"));
////        start_index = end_index + 1;
////        start_index = res.indexOf("<tr class=\"row1\">", start_index);
////        if(start_index < 0){
////            break;
////        }
////        end_index = res.indexOf("</tr>", start_index);
////        if(end_index < 0)
////        {
////            break;
////        }
////        substr = res.mid(start_index, end_index - start_index + 1);
////        qDebug()<<"substr:"<<substr.remove(QRegularExpression("[<> a-z:;=\"\\r\\n]"));
////        start_index = end_index + 1;

////    }

//qDebug()<<"end analysis!!!!!!!!!!!!!!";
//#if 1
//qDebug()<<"res:"<<value;
//QFile file("test.txt");
//file.open(QIODevice::WriteOnly);
//file.write(res.toStdString().data());
//file.close();
//#endif
