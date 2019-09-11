#include "sharehistoryreadthread.h"
#include <QFile>

ShareHistoryReadThread::ShareHistoryReadThread(const QString& code, QObject *parent) : QThread(parent), mCode(code)
{
    qRegisterMetaType<GRAPHIC_DATA_LIST>("const GRAPHIC_DATA_LIST&");
}

void  ShareHistoryReadThread::run()
{

    QString fileName = QString("%1/%2").arg(HQ_DAY_HISTORY_DIR).arg(mCode);
    if(!QFile::exists(fileName))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<fileName<<" not exist";
        return ;
    }
    ShareHistoryFileDataList list;

    //读取文件
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<fileName<<" open error";
        return ;
    }
    while (!file.atEnd() ) {
        ShareHistoryFileData data;
        file.read((char*)(&data), sizeof(ShareHistoryFileData));
        list.append(data);
    }
    file.close();

    //生成图形数据
    emit signalSendGraphicDataList(GRAPHIC_DATA_LIST(list));
    return ;
}

