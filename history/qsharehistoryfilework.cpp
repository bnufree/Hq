#include "qsharehistoryfilework.h"
#include "data_structure/sharedata.h"
#include <QFile>

QShareHistoryFileWork::QShareHistoryFileWork(int mode, const QString& fileName, \
                                               const ShareDataList& list, QObject* parent):
    mMode(mode),mFileName(fileName),mHistoryList(list),mParent(parent),QRunnable()
{

}

QShareHistoryFileWork::~QShareHistoryFileWork()
{

}

void QShareHistoryFileWork::run()
{
    if(mMode == FILE_READ) read();
    else write();
}

void QShareHistoryFileWork::read()
{
    ShareDataList list;
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
    int totalNum = 0;
    if(size > sizeof(int))
    {
        file.read((char*)(&totalNum), sizeof(int));
        while (!file.atEnd() ) {
            ShareData data;
            file.read((char*)(&data), sizeof(ShareData));

            if(data.mRealInfo.mClose < 0.000001) continue;
//            if(QString::fromStdString(data.mCode) == "600804")
//            {
//                qDebug()<<data.mCode<<data.mName<<data.mMoney<<data.mClose<<QDateTime::fromMSecsSinceEpoch(data.mTime).toString("yyyy-MM-dd");
//            }
            list.append(data);
        }
    }
    file.close();
    if(list.count() > 0)
    {
        if(mParent)
        {
            QMetaObject::invokeMethod(mParent,\
                                      "slotUpdateReadHistoryInfo",\
                                      Qt::DirectConnection,\
                                      Q_ARG(ShareDataList, list)
                                      );
        }


    }


    return;
}

void QShareHistoryFileWork::write()
{
    FILE *fp = fopen(mFileName.toStdString().data(), "wb+");
    if(fp)
    {
        int size = mHistoryList.size();
        fwrite(&size, sizeof(size), 1, fp);
        //qDebug()<<__FUNCTION__<<__LINE__<<mFileName<<size<<sizeof(ShareData);
        for(int i=0; i<size; i++)
        {
            ShareData data = mHistoryList[i];
            if(data.mCode.length() != 6)
            {
                //qDebug()<<__FUNCTION__<<__LINE__<<data.mCode<<data.mName;
            }
            fwrite(&data, sizeof(ShareData), 1, fp);
        }
        //qDebug()<<__FUNCTION__<<__LINE__<<mFileName<<size<<sizeof(ShareData)<<size*sizeof(ShareData)<<ftell(fp);
        fclose(fp);
    }

    if(mParent)
    {
        QMetaObject::invokeMethod(mParent,\
                                  "slotUpdateWriteHistoryInfo",\
                                  Qt::DirectConnection,\
                                  Q_ARG(ShareDataList, mHistoryList)
                                  );
    }
}
