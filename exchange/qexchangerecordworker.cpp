#include "qexchangerecordworker.h"
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QStringList>


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

    mColMap.clear();

    QList<ShareExchangeData> list;
    for(int i=1; i<=nRowCount; i++)
    {
        QStringList rows;
        for(int k=1; k<=nColCount; k++)
        {
            QString str = xlsx.read(i, k).toString();
            rows.append(str.remove(QRegularExpression("[=\"]")));
        }
//        qDebug()<<"rows:"<<rows;

        if(i==1)
        {
            //读取列表标题,将标题和类型对应
            for(int i=0; i<rows.length(); i++)
            {
                if(rows[i] == QStringLiteral("日期") || rows[i] == QStringLiteral("清算周期"))
                {
                    mColMap[Date] = i;
                } else if(rows[i] == QStringLiteral("交易类型"))
                {
                    mColMap[Type] = i;
                } else if(rows[i] == QStringLiteral("证券代码"))
                {
                    mColMap[Code] = i;
                } else if(rows[i] == QStringLiteral("证券名称"))
                {
                    mColMap[Name] = i;
                } else if(rows[i] == QStringLiteral("证券名称"))
                {
                    mColMap[Name] = i;
                } else if(rows[i] == QStringLiteral("成交均价"))
                {
                    mColMap[Price] = i;
                } else if(rows[i] == QStringLiteral("成交数量"))
                {
                    mColMap[Number] = i;
                } else if(rows[i] == QStringLiteral("成交金额"))
                {
                    mColMap[Money] = i;
                } else if(rows[i] == QStringLiteral("发生金额"))
                {
                    mColMap[Net] = i;
                } else if(rows[i] == QStringLiteral("佣金"))
                {
                    mColMap[Brokerage] = i;
                } else if(rows[i] == QStringLiteral("印花税"))
                {
                    mColMap[StampTax] = i;
                } else if(rows[i] == QStringLiteral("其他费用"))
                {
                    mColMap[Other] = i;
                } else if(rows[i] == QStringLiteral("剩余数量"))
                {
                    mColMap[TotalCount] = i;
                } else if(rows[i] == QStringLiteral("账户名称"))
                {
                    mColMap[Account] = i;
                } else if(rows[i] == QStringLiteral("序列号"))
                {
                    mColMap[SerialNum] = i;
                }
            }

        } else
        {
            if(mColMap.size() == 0) continue;
            if(rows.size() < mColMap.size()) continue;

            ShareExchangeData data;
            data.mCode = rows[mColMap[Code]];
            data.mDateTime = rows[mColMap[Date]];
            data.mID = -1;
            data.mName = rows[mColMap[Name]];
            data.mMoney = rows[mColMap[Money]].toDouble();
            data.mNetIncome = rows[mColMap[Net]].toDouble();
            data.mNum = rows[mColMap[Number]].toInt();
            data.mOther = rows[mColMap[Other]].toDouble();
            data.mPrice = rows[mColMap[Price]].toDouble();
            if(mColMap.contains(SerialNum)) data.mSerialNum = rows[mColMap[SerialNum]].toInt();
            if(mColMap.contains(TotalCount)) data.mTotalNum = rows[mColMap[TotalCount]].toInt();
            data.mType = rows[mColMap[Type]].toInt();
            data.mYinhuasui = rows[mColMap[StampTax]].toDouble();
            data.mYongjin = rows[mColMap[Brokerage]].toDouble();

            qDebug()<<data.mDateTime<<data.mCode<<data.mName<<data.mType<<data.mMoney<<data.mNetIncome;

            list.append(data);
        }

    }

    qDebug()<<mColMap;
    return ;

}
