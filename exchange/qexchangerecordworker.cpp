#include "qexchangerecordworker.h"
//#include <QtXlsx/QtXlsx>
//#include <QtXlsx/xlsxworksheet.h>
//#include <QtXlsx/xlsxworkbook.h>
//#include <QtXlsx/xlsxcellrange.h>
#include <QDebug>
#include <QFile>

QExchangeRecordWorker::QExchangeRecordWorker(QObject *parent) : QObject(parent)
{
    this->moveToThread(&mWorkThread);
    connect(this, SIGNAL(signalStartImport(QString)), this, SLOT(slotStartImport(QString)));
    mWorkThread.start();
}

void QExchangeRecordWorker::slotStartImport(const QString &sFilePathName)
{
#if 0
    //检查文件是否存在
    if(!QFile::exists(sFilePathName))
    {

        qDebug()<<"file does not exist.......";
        return ;
    }
    StockDataList list;
    if(sFilePathName.isEmpty())
    {
        return ;
    }
    QXlsx::Document xlsx(sFilePathName);
    QXlsx::Worksheet *sheet = xlsx.currentWorksheet();
    int nRowCount = sheet->dimension().lastRow();
    int nColCount = sheet->dimension().lastColumn();
    if(nRowCount<=0||nColCount<=0)
    {
        return ;
    }

    QStringList codelist;
    for(int i = 1;i<nRowCount;i++)
    {
        StockData data;
        data.mCode = xlsx.read(i+1,2).toString();
        data.mProfit = xlsx.read(i+1,5).toDouble();
        if(data.mCode.contains("mf")) continue;
        list.append(data);
        codelist.append(data.mCode);
    }
    emit signalSendStkProfitList(list);
    emit signalSendCodeList(codelist);
#endif
    return ;

}
