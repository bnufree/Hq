#include "annualcloseddaydisclosure.h"
#include "utils/qhttpget.h"

AnnualClosedDayDisclosure::AnnualClosedDayDisclosure( bool async, QObject *parent) : QThread(parent)
{
    mAsyncSend = async;
}


void AnnualClosedDayDisclosure::run()
{
    QString content = QString::fromUtf8(QHttpGet::getContentOfURL("http://www.sse.com.cn/disclosure/dealinstruc/closed/"));
    QRegExp yearReg("(\\d{4})年休市安排");
    int index = 0;
    index = yearReg.indexIn(content, index);
    int year = 0;
    if(index >= 0)
    {
        year = yearReg.cap(1).toInt();
        index += yearReg.matchedLength();
    } else
    {
        return;
    }
    QRegExp reg("((\\d{1,2})月(\\d{1,2})日（星期[一二三四五六日]）至)?(\\d{1,2})月(\\d{1,2})日（星期[一二三四五六日]）休市");
    while ((index = reg.indexIn(content, index)) >= 0) {
        index += reg.matchedLength();
        if(reg.captureCount() != 5) continue;
        QDate start, end;
        if(reg.cap(2).size() > 0 && reg.cap(3).size() > 0)
        {
            start.setDate(year, reg.cap(2).toInt(), reg.cap(3).toInt());
        }

        if(reg.cap(4).size() > 0 && reg.cap(5).size() > 0)
        {
            end.setDate(year, reg.cap(4).toInt(), reg.cap(5).toInt());
        }
        if(!end.isValid()) continue;
        if(start.isValid())
        {
            while (start < end) {
                mDateList.append(start);
                start = start.addDays(1);
            }
        }
        mDateList.append(end);
    }

    if(mAsyncSend)
    {
        emit signalSendClosedDateList(mDateList);
    }

}
