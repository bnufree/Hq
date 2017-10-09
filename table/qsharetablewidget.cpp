#include "qsharetablewidget.h"

QShareTablewidget::QShareTablewidget(QWidget *parent) : QWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("代码"), STK_DISPLAY_SORT_TYPE_CODE));
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NAME));
    datalist.append(TableColData(QStringLiteral("现价"), STK_DISPLAY_SORT_TYPE_PRICE));
    datalist.append(TableColData(QStringLiteral("涨跌"), STK_DISPLAY_SORT_TYPE_CHGPER));
    datalist.append(TableColData(QStringLiteral("成交"), STK_DISPLAY_SORT_TYPE_CJE));
    datalist.append(TableColData(QStringLiteral("资金比"), STK_DISPLAY_SORT_TYPE_MONEYR));
    datalist.append(TableColData(QStringLiteral("3日"), STK_DISPLAY_SORT_TYPE_LAST3));
    datalist.append(TableColData(QStringLiteral("资金流"), STK_DISPLAY_SORT_TYPE_ZJLX));
    datalist.append(TableColData(QStringLiteral("股息率"), STK_DISPLAY_SORT_TYPE_GXL));
    datalist.append(TableColData(QStringLiteral("送转"), STK_DISPLAY_SORT_TYPE_SZZBL));
    datalist.append(TableColData(QStringLiteral("总市值"), STK_DISPLAY_SORT_TYPE_TCAP));
    datalist.append(TableColData(QStringLiteral("流通市值"), STK_DISPLAY_SORT_TYPE_MCAP));
    datalist.append(TableColData(QStringLiteral("外资持股"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL));
    datalist.append(TableColData(QStringLiteral("外资持股△"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG));
    datalist.append(TableColData(QStringLiteral("持股市值"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP));
    datalist.append(TableColData(QStringLiteral("持股市值△"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP_CHG));
    datalist.append(TableColData(QStringLiteral("登记日"), STK_DISPLAY_SORT_TYPE_GQDJR));
    datalist.append(TableColData(QStringLiteral("公告日"), STK_DISPLAY_SORT_TYPE_YAGGR));
    datalist.append(TableColData(QStringLiteral("换手率"), STK_DISPLAY_SORT_TYPE_HSL));
}

void QShareTablewidget::setDataList(const StockDataList &list)
{
    prepareUpdateTable();
    int newRowCount = list.size();
    int i = 0;
    foreach (StockData data, list) {
        if(i+1 > oldRowCount) this->appendRow();
        int k =0;
        this->setRowHeight(i, 20);
        this->setItemText(i, k++, data.mCode, Qt::AlignRight);
        this->setItemText(i, k++, data.mName);
        this->setItemText(i, k++, QString("").sprintf("%.2f", data.mCur));
        double val = mStockMap[data.mCode];
        QString flag = val > data.mChgPercent ? QStringLiteral("↑") : val < data.mChgPercent ? QStringLiteral("↓") : "";
        this->setItemText(i, k++, QString("%1%2%").arg(flag).arg(QString::number(data.mChgPercent, 'f', 2)));
        mStockMap[data.mCode] = data.mChgPercent;
        if(data.mMoney >= 1000){
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mMoney / 10000.0) + QStringLiteral("亿"));
        } else {
            this->setItemText(i, k++, QString("").sprintf("%.0f", data.mMoney) + QStringLiteral("万"));
        }
        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mMoneyRatio));
        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mLast3DaysChgPers));
        if(fabs(data.mZJLX) >= 1000){
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mZJLX / 10000.0) + QStringLiteral("亿"));
        } else {
            this->setItemText(i, k++, QString("").sprintf("%.0f", data.mZJLX) + QStringLiteral("万"));
        }
        this->setItemText(i, k++, QString("").sprintf("%.2f%(%.2f)",data.mGXL * 100, data.mXJFH));
        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mSZZG));
        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mTotalCap / 100000000.0 ) + QStringLiteral("亿"));
        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mMutalbleCap/ 100000000.0 )+ QStringLiteral("亿"));

        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mProfit));
        if(data.mForeignVol >= 10000000){
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignVol / 100000000.0) + QStringLiteral("亿"));
        } else {
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignVol / 10000.0)  + QStringLiteral("万"));
        }
        if(data.mForeignVolChg >= 10000000){
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignVolChg / 100000000.0) + QStringLiteral("亿"));
        } else {
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignVolChg / 10000.0)  + QStringLiteral("万"));
        }
        if(data.mForeignCap >= 1000000){
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignCap / 100000000.0) + QStringLiteral("亿"));
        } else {
            this->setItemText(i, k++, QString("").sprintf("%.0f", data.mForeignCap / 10000.0) + QStringLiteral("万"));
        }

        if(data.mForeignCapChg >= 1000000){
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignCapChg / 100000000.0) + QStringLiteral("亿"));
        } else {
            this->setItemText(i, k++, QString("").sprintf("%.0f", data.mForeignCapChg / 10000.0) + QStringLiteral("万"));
        }
        this->setItemText(i, k++, data.mGQDJR.toString("yyyy-MM-dd"));
        this->setItemText(i, k++, data.mYAGGR.toString("yyyy-MM-dd"));
        this->updateFavShareIconOfRow(i);
        this->item(i, 0)->setData(Qt::UserRole, code);
        this->item(i, 0)->setData(Qt::UserRole+1, data.mBlockList);
        i++;

    }
    afterUpdateTable(newRowCount);
}

void QShareTablewidget::initMenu()
{
    QMenu* hqCenterMenu = new QMenu(QStringLiteral("行情中心"), this);
    QList<QAction*> actlist;

    QStringList poplist;
    poplist<<QStringLiteral("自选")<<QStringLiteral("沪深")<<QStringLiteral("沪市")<<QStringLiteral("深市")
          <<QStringLiteral("中小板")<<QStringLiteral("创业板")<<QStringLiteral("沪深基金")
         <<QStringLiteral("恒指")<<QStringLiteral("恒生国企")
        <<QStringLiteral("港股通");
    QList<int> mktlist;
        mktlist<<MKT_ZXG<<MKT_ALL<<MKT_SH<<MKT_SZ<<MKT_ZXB<<MKT_CYB<<MKT_JJ<<MKT_HK_HSZS<<MKT_HK_HSGQ<<MKT_HK_GGT;
        int index = -1;
        foreach (QString name, poplist) {
            index++;
            QAction *act = new QAction(this);
            act->setText(name);
            act->setData(mktlist[index]);
            connect(act, &QAction::triggered, this, &Dialog::setStockMarket);
            actlist.append(act);
        }

        mHqCenterMenu->addActions(actlist);


        mHqHeaderMenu = new QMenu(QStringLiteral("列表标题"), this);
        for(int i=0; i<mHqHeaderList.length(); i++)
        {
            QAction *act = new QAction(this);
            act->setText(mHqHeaderList[i]);
            TableColDisplayStatus data;
            data.mTable = ui->hqtbl;
            data.mColIndex = i;
            data.mIsDisplay = true;
            //act->setData(i);
            act->setCheckable(true);
            data.mIsDisplay = true;
            act->setChecked(data.mIsDisplay);
            act->setData(QVariant::fromValue(data));
            connect(act, &QAction::triggered, this, &Dialog::setDisplayCol);
            mHqColActList.append(act);
        }

        mHqHeaderMenu->addActions(mHqColActList);

        mHqPageMenu = new QMenu(QStringLiteral("页面控制"), this);
        poplist.clear();
        poplist<<QStringLiteral("首页")<<QStringLiteral("前一页")<<QStringLiteral("后一页")<<QStringLiteral("末页");
        for(int i=0; i<poplist.length(); i++)
        {
            QAction *act = new QAction(this);
            act->setText(poplist[i]);
            act->setData(i);
            connect(act, &QAction::triggered, this, &Dialog::setDisplayPage);
            mHqPageMenu->addAction(act);
        }

    }

}

void QShareTablewidget::slotCustomContextMenuRequested(const QPoint &point)
{
    QMenu *popMenu = new QMenu(this);
    popMenu->addMenu(mHqCenterMenu);
    popMenu->addMenu(mHqHeaderMenu);
    popMenu->addMenu(mHqPageMenu);

    //自选股编辑
    QTableWidgetItem *item = ui->hqtbl->itemAt(pos);
    if(item)
    {
        item = ui->hqtbl->item(item->row(), 0);
        QString stkCode = item->data(Qt::UserRole).toString();
        QList<QAction*> actlist;
        int row = item->row();
        item = ui->hqtbl->item(row, 0);
        QStringList poplist;
        poplist<<QStringLiteral("分时图")<<QStringLiteral("日线图")<<QStringLiteral("沪深股通");
        QList<int> Optlist;
        Optlist<<MENU_OPT_MINUTE<<MENU_OPT_DAY<<MENU_OPT_HSGT;
        int index = -1;
        foreach (QString name, poplist) {
            index++;
            QAction *act = new QAction(this);
            act->setText(name);
            HqTableMenuData  data;
            data.mStockCode = stkCode;
            data.mMenuCmd = Optlist[index];
            act->setData(QVariant::fromValue(data));
            connect(act, &QAction::triggered, this, &Dialog::hqMenuOpt);
            actlist.append(act);
        }
        QMenu *submenu = new QMenu(QStringLiteral("所属板块"), this);
        QStringList blocklist = item->data(Qt::UserRole+1).toStringList();
        //qDebug()<<"blocklist:"<<blocklist<<" code:"<<item->data(Qt::UserRole).toString();
        foreach (QString name, blocklist) {
            if(name.trimmed().isEmpty()) continue;
            if(mBlockDataMap[name].name.trimmed().isEmpty()) continue;
            QAction *act = new QAction(this);
            act->setText(QString("%1:%2%").arg(mBlockDataMap[name].name).arg(mBlockDataMap[name].changePer));
            qDebug()<<"subtext:"<<act->text();
            HqTableMenuData data;
            data.mStockCode = stkCode;
            data.mBlockCode = name;
            data.mMenuCmd = MENU_OPT_BLOCK;
            act->setData(QVariant::fromValue(data));
            connect(act, &QAction::triggered, this, &Dialog::hqMenuOpt);
            submenu->addAction(act);
        }
        popMenu->addActions(actlist);
        popMenu->addMenu(submenu);
    }

    popMenu->popup(QCursor::pos());
}
