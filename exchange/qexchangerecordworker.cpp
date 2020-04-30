#include "qexchangerecordworker.h"
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
#include <QDebug>
#include <QFile>


using namespace QXlsx;


QExchangeRecordWorker::QExchangeRecordWorker(QObject *parent) : QObject(parent)
{
    this->moveToThread(&mWorkThread);
    connect(this, SIGNAL(signalStartImport(QString)), this, SLOT(slotStartImport(QString)));
    mWorkThread.start();
}

void QExchangeRecordWorker::slotStartImport(const QString &sFilePathName)
{
    //检查文件是否存在
    if(sFilePathName.isEmpty() || !QFile::exists(sFilePathName))
    {

        qDebug()<<"file does not exist.......";
        return ;
    }

    QXlsx::Document xlsx(sFilePathName);
    QXlsx::Worksheet *sheet = xlsx.currentWorksheet();
    if(!sheet)
    {
        qDebug()<<"not found sheet....";
        return;
    }
    int nRowCount = sheet->dimension().lastRow();
    int nColCount = sheet->dimension().lastColumn();
    if(nRowCount <= 0 || nColCount <= 0)
    {
        qDebug()<<"no countent found now.......";
        return;
    }

    QStringList codelist;
    for(int i=1; i<nRowCount; i++)
    {
        QStringList rows;
        for(int k=1; k<nColCount; k++)
        {
            rows.append(xlsx.read(i, k).toString());
        }
        qDebug()<<"rows:"<<rows;
    }
    return ;

}
