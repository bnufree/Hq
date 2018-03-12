#include "qsharehistoryfilework.h"
#include "utils/sharedata.h"
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
    if(!QFile::exists(mFileName)) return ;
    //读取文件
    QFile file(mFileName);
    if(!file.open(QIODevice::ReadOnly)) return ;
    int size = file.size();
    int totalNum = 0;
    if(size > sizeof(int))
    {
        file.read((char*)(&totalNum), sizeof(int));
        while (!file.atEnd() ) {
            ShareData data;
            file.read((char*)(&data), sizeof(ShareBaseData));
            list.append(data);
        }
    }
    file.close();
    if(totalNum != 0 && list.count() == totalNum)
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
        for(int i=0; i<size; i++)
        {
            fwrite(&(mHistoryList[i]), sizeof(ShareData), 1, fp);
        }
        fclose(fp);
    }

    if(mParent)
    {
        QMetaObject::invokeMethod(mParent,\
                                  "slotUpdateWriteHistoryInfo",\
                                  Qt::DirectConnection
                                  );
    }
}
