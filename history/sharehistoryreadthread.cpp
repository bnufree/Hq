#include "sharehistoryreadthread.h"
#include <QFile>

ShareHistoryReadThread::ShareHistoryReadThread(const QString& code, const QDate& date, QObject *parent) :
    QThread(parent),
    mCode(code),
    mDate(date)
{
    qRegisterMetaType<GRAPHIC_DATA_LIST>("const GRAPHIC_DATA_LIST&");
}

void  ShareHistoryReadThread::run()
{
    if(mDate >= QDate::currentDate().addDays(-30))
    {
        mDate = QDate::currentDate().addDays(-30);
    }

    qDebug()<<"work date:"<<mDate;

    QString fileName = QString("%1/%2").arg(HQ_DAY_HISTORY_DIR).arg(mCode);
    if(!QFile::exists(fileName))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<fileName<<" not exist";
        return ;
    }
    ShareDailyDataList list;

    //读取文件
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<fileName<<" open error";
        return ;
    }
    while (!file.atEnd() ) {
        ShareDailyData data;
        file.read((char*)(&data), sizeof(ShareDailyData));
        if(!mDate.isNull())
        {
            if(data.mDate < QDateTime(mDate).toTime_t()) continue;
        }
        list.append(data);
    }
    file.close();

    qDebug()<<"code:"<<mCode<<"date:"<<mDate<<" data:"<<list.size();

    //生成图形数据
    emit signalSendGraphicDataList(GRAPHIC_DATA_LIST(list));
    return ;
}

