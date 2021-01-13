#include "northboundtop10displaywidget.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include "utils/qhttpget.h"
#include <QVBoxLayout>
#include "data_structure/hqutils.h"
#include "dbservices/dbservices.h"

QShareLgtTop10Thread::QShareLgtTop10Thread(QObject *parent) : QThread(parent)
{
}

QShareLgtTop10Thread::~QShareLgtTop10Thread()
{

}

void QShareLgtTop10Thread::run()
{
    QDate last_update_date;
    while (true) {
        if(last_update_date != QDate::currentDate())
        {
            ShareHsgtList list;
            last_update_date = QDate::currentDate();
            while (1) {
                //从网络获取
                if(!getDataFromEastMoney(list, last_update_date))
                {
                    getDataFromHKEX(list, last_update_date);
                }
                if(list.size() > 0)
                {
                    emit signalChinaAShareTop10Updated(list, last_update_date.toString("yyyy-MM-dd"));
                    break;
                }
                last_update_date = last_update_date.addDays(-1);
            }
        }

        sleep(60);
    }
}

bool QShareLgtTop10Thread::getDataFromEastMoney(ShareHsgtList &list, QDate &date)
{
    qDebug()<<"start get from eastmoney"<<date;
    //从网络获取.
    QString url = QString("http://dcfm.eastmoney.com//EM_MutiSvcExpandInterface/api/js/get?type=HSGTCJB&token=70f12f2f4f091e459a279469fe49eca5&filter=(DetailDate=^%1^)&js=(x)&sr=1&st=Rank&rt=50014200")
            .arg(date.toString("yyyy-MM-dd"));
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(QHttpGet::getContentOfURL(url), &err);
    if(err.error != QJsonParseError::NoError)
    {
        qDebug()<<"east money json error:"<<date<<err.errorString();
        return false;
    }
    if(!doc.isArray())
    {
        qDebug()<<"east money content error:"<<date;
        return false;
    }
    //开始解析.
    ShareHsgtList resList;
    QJsonArray result = doc.array();
    for(int i=0; i<result.size(); i++)
    {
        QJsonObject obj = result.at(i).toObject();
        int market_type = obj.value("MarketType").toInt();
        if(market_type == 2 || market_type == 4) continue;
        ShareHsgt data;
        data.mCode = obj.value("Code").toString();
        data.mName = obj.value("Name").toString();
        if(data.mCode.left(1) == "6")
        {
            //上海
            data.mBuy = obj.value("HGTMRJE").toDouble();
            data.mSell = obj.value("HGTMCJE").toDouble();
        } else
        {
            //深圳
            data.mBuy = obj.value("SGTMRJE").toDouble();
            data.mSell = obj.value("SGTMCJE").toDouble();
        }
        data.mIsTop10 = true;
        data.mPure = data.mBuy - data.mSell;
        data.mDate = date;
        data.mTotal = data.mBuy + data.mSell;
        qDebug()<<"EAST"<<data.mCode<<data.mName<<data.mPure;
        resList.append(data);
    }
    if(resList.size() > 0) list.append(resList);

    return resList.size() > 0;
}

bool QShareLgtTop10Thread::getDataFromHKEX(ShareHsgtList &list, QDate &date)
{
    QByteArray recv = QHttpGet::getContentOfURL(QString("http://sc.hkex.com.hk/TuniS/www.hkex.com.hk/chi/csm/DailyStat/data_tab_daily_%1c.js?_=%2").arg(date.toString("yyyyMMdd"))
                                                .arg(QDateTime::currentDateTime().toMSecsSinceEpoch()));
    ShareHsgtList resList;
    int index = recv.indexOf("[");
    if(index >= 0) recv = recv.mid(index);
    QString result = QString::fromUtf8(recv).remove(QRegExp("[\\r\\n\\t]"));
    QRegularExpression start_reg("\\[\\[\"[0-9]{1,2}\"");
    int start_index = 0;
    while ((start_index = result.indexOf(start_reg, start_index)) >= 0) {
        int end_index = result.indexOf("]]", start_index);
        if(end_index == -1) break;
        QString line = result.mid(start_index, end_index - start_index+2);
        QStringList lineList = line.split("\", \"", QString::SkipEmptyParts);
        start_index = end_index;
        if(lineList.size() == 6)
        {
            ShareHsgt data;
            data.mCode = lineList[1];
            if(data.mCode.size() == 5) continue;
            int code = data.mCode.toInt();
            data.mCode = QString("").sprintf("%06d", code);
            data.mName = lineList[2].trimmed();
            data.mBuy = lineList[3].remove(",").toDouble();
            data.mSell = lineList[4].remove(",").toDouble();
            data.mTotal = data.mBuy + data.mSell;
            data.mPure = data.mBuy - data.mSell;
            data.mIsTop10 = true;
            data.mDate = date;
            qDebug()<<"HKEX"<<data.mCode<<data.mName<<data.mPure;
            resList.append(data);
        }
    }
    if(resList.size()) list.append(resList);
    return resList.size() > 0;
}



NorthBoundTop10DisplayTable::NorthBoundTop10DisplayTable(QWidget *parent) : HqTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("序号"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_CODE));
    datalist.append(TableColData(QStringLiteral("总计"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("净额"), STK_DISPLAY_SORT_TYPE_MONEYR));
    datalist.append(TableColData(QStringLiteral("日期"), STK_DISPLAY_SORT_TYPE_NONE));
    setHeaders(datalist);
    mSortCol = 3;
    mSortMode = Qt::AscendingOrder;
//    setSortingEnabled(true);
}

void NorthBoundTop10DisplayTable::slotHeaderClicked(int col)
{
    if(col != 3) return;
    mSortMode = (mSortMode + 1) % 2;

    slotSetDataList(ShareHsgtList());
}

void NorthBoundTop10DisplayTable::slotSetDataList(const ShareHsgtList &list)
{
    if(list.size() > 0)  mDataList = list;
    if(mSortMode == Qt::AscendingOrder)
    {
        qStableSort(mDataList.begin(), mDataList.end(), std::greater<ShareHsgt>());
    } else
    {
        qStableSort(mDataList.begin(), mDataList.end(), std::less<ShareHsgt>());
    }
    prepareUpdateTable(mDataList.size());
    int i = 0;
    foreach (ShareHsgt data, mDataList) {
        int k =0;
        this->setItemText(i, k++, QString::number(i+1));
        this->setItemText(i, k++, data.mName);
        this->setItemText(i, k++, QString("").sprintf("%.0f",(data.mBuy + data.mSell) / 10000.0));
        this->setItemText(i, k++, QString("").sprintf("%.0f",(data.mPure)/10000.0));
        this->setItemText(i, k++, data.mDate.toString());
        this->item(i,0)->setData(Qt::UserRole,data.mCode);
        i++;
    }
}

void NorthBoundTop10DisplayTable::slotCellDoubleClicked(int row, int col)
{
    QTableWidgetItem* item = this->item(row, 0);
    if(!item) return;
//    emit signalDisplayMutualBundHistory(item->data(Qt::UserRole).toString());
}

NorthBoundTop10DisplayWidget::NorthBoundTop10DisplayWidget(QWidget *parent) : QWidget(parent)
{
    mCommonStr = QString::fromUtf8("北向成交榜数据更新：");
    mTimeLabel = new QLabel(this);
    mTimeLabel->setAlignment(Qt::AlignLeft);
    mTimeLabel->setFixedHeight(HqUtils::convertMM2Pixel(8));
    QFont font1 = mTimeLabel->font();
    font1.setPixelSize(HqUtils::convertMM2Pixel(6));
    mTimeLabel->setFont(font1);
    QVBoxLayout *layout1 = new QVBoxLayout;
    this->setLayout(layout1);
    layout1->setMargin(1);
    layout1->setSpacing(1);
    mTable = new NorthBoundTop10DisplayTable(this);
    layout1->addWidget(mTimeLabel);
    layout1->addWidget(mTable);

    connect(DATA_SERVICE, SIGNAL(signalSendShareHsgtTop10List(ShareHsgtList, QString, QDate)),
            this, SLOT(slotSetDataList(ShareHsgtList, QString, QDate)));

    DATA_SERVICE->signalQueryLatestHsgtData();

}

void NorthBoundTop10DisplayWidget::slotSetDataList(const ShareHsgtList &list, const QString& code, const QDate& date)
{
    mTimeLabel->setText(mCommonStr + date.toString("yyyy-MM-dd"));
    mTable->slotSetDataList(list);
}
