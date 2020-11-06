#include "qexchangerecordworker.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QStringList>
#include <QTimer>



using namespace QXlsx;
bool profit_cmp_desc = true;

bool ShareExchangeDataMgr_CMP(const ShareExchangeDataMgr& p1, const ShareExchangeDataMgr& p2)
{
    if(profit_cmp_desc) return p1.mProfit > p2.mProfit;
    return p1.mProfit <p2.mProfit;
}


QExchangeRecordWorker::QExchangeRecordWorker(QObject *parent) :
    QObject(parent),
    mXlsx(0)
{
    qRegisterMetaType<QList<ShareExchangeDataMgr>>("const QList<ShareExchangeDataMgr>&");
    connect(this, SIGNAL(signalQueryShareProfitList(bool)), this, SLOT(slotQueryShareProfitList(bool)));
    moveToThread(&mWorkThread);
    mWorkThread.start();
    QTimer::singleShot(1000, this, SLOT(slotStartImport()));
}

QExchangeRecordWorker::~QExchangeRecordWorker()
{
    qDebug()<<"file end now...........";
    mWorkThread.quit();
    if(mXlsx)
    {
        mXlsx->save();
        delete mXlsx;
    }
}

bool QExchangeRecordWorker::isDateCol(const QString &title)
{
    return title.contains(QStringLiteral("日期")) || title.contains(QStringLiteral("清算周期"));
}

bool QExchangeRecordWorker::isCodeCol(const QString &title)
{
    return title.contains(QStringLiteral("证券代码"));
}

bool QExchangeRecordWorker::isNameCol(const QString &title)
{
    return title.contains(QStringLiteral("证券名称"));
}

bool QExchangeRecordWorker::isTypeCol(const QString &title)
{
    return title.contains(QStringLiteral("类型")) || title.contains(QStringLiteral("方向"));
}

bool QExchangeRecordWorker::isPriceCol(const QString &title)
{
    return title.contains(QStringLiteral("价格")) || title.contains(QStringLiteral("均价"));
}


bool QExchangeRecordWorker::isSerialNumCol(const QString& title)
{
    return title.contains(QStringLiteral("成交编号")) || title.contains(QStringLiteral("序列号")) || title.contains(QStringLiteral("流水号"));
}

bool QExchangeRecordWorker::isAccountCol(const QString& title)
{
    return title.contains(QStringLiteral("账户"));
}

bool QExchangeRecordWorker::isTotalCountCol(const QString& title)
{
    return title.contains(QStringLiteral("剩余数量"));
}

bool QExchangeRecordWorker::isNetCol(const QString& title)
{
    return title.contains(QStringLiteral("发生金额"));
}

bool QExchangeRecordWorker::isOtherCol(const QString& title)
{
    return title.contains(QStringLiteral("其他")) || title.contains(QStringLiteral("过户费"));
}

bool QExchangeRecordWorker::isStamptaxCol(const QString& title)
{
    return title.contains(QStringLiteral("印花税"));
}

bool QExchangeRecordWorker::isBrokerageCol(const QString& title)
{
    return title.contains(QStringLiteral("佣金"));
}

bool QExchangeRecordWorker::isNumberCol(const QString& title)
{
    return title.contains(QStringLiteral("成交数量"));
}

bool QExchangeRecordWorker::isMoneyCol(const QString& title)
{
    return title.contains(QStringLiteral("成交金额"));
}

int QExchangeRecordWorker::parseTypeOfString(const QString& type)
{
    if(type.contains(QStringLiteral("卖出"))) return ShareExchange_Sell;
    if(type.contains(QStringLiteral("买入"))) return ShareExchange_Buy;
    if(type.contains(QStringLiteral("派息"))) return ShareExchange_Share_Bonus;
    if(type.contains(QStringLiteral("送转"))) return ShareExchange_Dividend_Bonus;
    return ShareExchange_None;
}

void QExchangeRecordWorker::slotUpdateRecordSucceed()
{
//    emit DATA_SERVICE->signalQueryShareExchangeRecord(1, "000651", "", "");
}

void QExchangeRecordWorker::slotQueryShareProfitList(bool clear)
{
    if(mShareDataMgrMap.size() == 0) return;
    QList<ShareExchangeDataMgr> list;
    foreach (ShareExchangeDataMgr data, mShareDataMgrMap) {
        if(clear && data.mVol == 0)
        {
            list.append(data);
        }
        if((!clear) && data.mVol > 0)
        {
            list.append(data);
        }
    }

    qStableSort(list.begin(), list.end(), ShareExchangeDataMgr_CMP);


}

void QExchangeRecordWorker::slotStartImport()
{
    QString sFilePathName = "record.xlsx";
    //检查文件是否存在
    if(sFilePathName.isEmpty() || !QFile::exists(sFilePathName))
    {

        qDebug()<<"file does not exist.......";
        return ;
    }

    mXlsx = new QXlsx::Document(sFilePathName);
    QXlsx::Worksheet *sheet = mXlsx->currentWorksheet();
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

    mShareDataMgrMap.clear();
    int  index = 0;
    QString currentDate;
    for(int i=1; i<=nRowCount; i++)
    {
        QStringList rows;
        for(int k=1; k<=nColCount; k++)
        {
            QString str = mXlsx->read(i, k).toString();
            rows.append(str.remove(QRegularExpression("[=\",]")));
        }
//        qDebug()<<"rows:"<<rows;

        if(i==1)
        {
            //读取列表标题,将标题和类型对应
            for(int i=0; i<rows.length(); i++)
            {
                if(isDateCol(rows[i]))
                {
                    mColMap[Date] = i;
                } else if(isTypeCol(rows[i]))
                {
                    mColMap[Type] = i;
                } else if(isCodeCol(rows[i]))
                {
                    mColMap[Code] = i;
                } else if(isNameCol(rows[i]))
                {
                    mColMap[Name] = i;
                } else if(isPriceCol(rows[i]))
                {
                    mColMap[Price] = i;
                } else if(isNumberCol(rows[i]))
                {
                    mColMap[Number] = i;
                } else if(isMoneyCol(rows[i]))
                {
                    mColMap[Money] = i;
                } else if(isNetCol(rows[i]))
                {
                    mColMap[Net] = i;
                } else if(isBrokerageCol(rows[i]))
                {
                    mColMap[Brokerage] = i;
                } else if(isStamptaxCol(rows[i]))
                {
                    mColMap[StampTax] = i;
                } else if(isOtherCol(rows[i]))
                {
                    mColMap[Other] = i;
                } else if(isTotalCountCol(rows[i]))
                {
                    mColMap[TotalCount] = i;
                } else if(isAccountCol(rows[i]))
                {
                    mColMap[Account] = i;
                } else if(isSerialNumCol(rows[i]))
                {
                    mColMap[SerialNum] = i;
                }
            }
            //检查最基本的项目在不在,异常退出
            bool col_ok = false;
            if(mColMap.contains(Date) && mColMap.contains(Code) && mColMap.contains(Price) && mColMap.contains(Number) && mColMap.contains(Type))
            {
                col_ok = true;
            }
            if(!col_ok)
            {
                qDebug()<<"invalid xlsx file found.";
                break;
            }

        } else
        {
            if(mColMap.size() == 0) continue;
            if(rows.size() < mColMap.size()) continue;

            ShareExchangeData data;
            data.mCode = rows[mColMap[Code]].right(6);
            if(data.mCode.trimmed().size() == 0) continue;
            data.mDateTime = rows[mColMap[Date]].remove("-");
            data.mID = -1;
            data.mNum = rows[mColMap[Number]].toInt();
            data.mPrice = rows[mColMap[Price]].toDouble();
            data.mType = parseTypeOfString(rows[mColMap[Type]]);
            if(data.mType == 0) continue;
            if(data.mType == ShareExchange_Share_Bonus) data.mNum = 0;

            if(mColMap.contains(Name))  data.mName = rows[mColMap[Name]];
            if(data.mName.contains(QStringLiteral("金腾通"))) continue;
            if(mColMap.contains(Money))
            {
                data.mMoney = rows[mColMap[Money]].toDouble();
            } else
            {
                data.mMoney = data.mNum * data.mPrice;
            }
            if(mColMap.contains(Other)) data.mOther = rows[mColMap[Other]].toDouble();
            if(mColMap.contains(SerialNum))
            {
                data.mSerialText =  data.mDateTime + rows[mColMap[SerialNum]];
            } else
            {
                if(currentDate != data.mDateTime)
                {
                    currentDate = data.mDateTime;
                    index = 1;
                } else
                {
                    index++;
                }
                data.mSerialText = data.mDateTime + data.mCode + QString::number(index);
            }
            if(mColMap.contains(TotalCount)) data.mTotalNum = rows[mColMap[TotalCount]].toInt();
            if(mColMap.contains(StampTax))
            {
                data.mYinhuasui = rows[mColMap[StampTax]].toDouble();
            } else
            {
                data.mYinhuasui = 0.0;
                if(data.mType == ShareExchange_Sell && (data.mCode.startsWith("6") || data.mCode.startsWith("0") || data.mCode.startsWith("3")))
                {
                    data.mYinhuasui = data.mMoney * 1.0 / 1000;
                }
            }
            if(mColMap.contains(Brokerage))
            {
                data.mYongjin = rows[mColMap[Brokerage]].toDouble();
            } else
            {
                data.mYongjin = data.mMoney * 2.5 / 10000;
                if(data.mYongjin < 5.00) data.mYongjin = 5.00;
            }

            if(mColMap.contains(Net))
            {
                data.mNetIncome = rows[mColMap[Net]].toDouble();
            } else
            {
                if(data.mType == ShareExchange_Buy)
                {
                    data.mNetIncome = (data.mMoney + data.mYinhuasui + data.mYongjin + data.mOther) * (-1);
                } else if(data.mType == ShareExchange_Sell)
                {
                    data.mNetIncome = data.mMoney - data.mYinhuasui - data.mYongjin - data.mOther;
                } else
                {
                    data.mNetIncome = 0.0;
                }
            }
            ShareExchangeDataMgr  &mgr = mShareDataMgrMap[data.mCode];
            mgr.mProfit += data.mNetIncome;
            mgr.mCode = data.mCode;
            mgr.mName = data.mName;
            int preVol = mgr.mVol;
            mgr.mVol += data.mNum *(data.mType == ShareExchange_Buy || data.mType == ShareExchange_Dividend_Bonus? 1:(-1));
            if(preVol >= 0)
            {
                mgr.mList.append(data);
            } else
            {
                mgr.mList.insert(mgr.mList.size() - 1, data);
            }

        }
    }

    emit signalSendShareProfitList(mShareDataMgrMap.values());
    return ;

}
