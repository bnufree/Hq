#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QMenu>
#include "profiles.h"
#include "stkmktcap.h"
#include <QDateTime>
#include "qexchangedatemangagedialog.h"
#include "qeastmoneyzjlxthread.h"
#include <QShortcut>
#include "qthook.h"
#include <QProcess>
#include <QDesktopWidget>
#include <QResizeEvent>

#define     STK_ZXG_SEC         "0520"
#define     STK_HSJJ_SEC        "4521"
#define     STK_ZXG_NAME        "codes"

class HqTableWidgetItem : public QTableWidgetItem
{
public:
    HqTableWidgetItem(const QString& text, Qt::AlignmentFlag flg = Qt::AlignCenter)
        :QTableWidgetItem(text)
    {
        setTextAlignment(flg);
//        QFont font = this->font();
//        font.setPointSize(10);
//        setFont(font);
    }

    ~HqTableWidgetItem()
    {

    }

};

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),mBlockThread(NULL)/*,mStockThread(NULL)*/,mSearchThread(NULL),mDisplayCol(0),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    mSecSize = 75;
    mDisplayMode = E_DISPLAY_ALL;
    int wkwidth = 0;
    //设定标题
    QStringList blockTitleList;
    blockTitleList<<QStringLiteral("名称")<<QStringLiteral("涨跌");
    ui->blocktbl->setColumnCount(blockTitleList.length());
    ui->blocktbl->setHorizontalHeaderLabels(blockTitleList);
    ui->blocktbl->horizontalHeader()->setDefaultSectionSize(mSecSize);
    mDisplayCol += blockTitleList.length();

    mHqHeaderList<<QStringLiteral("代码")<<QStringLiteral("名称")<<QStringLiteral("现价")<<QStringLiteral("涨跌")\
               <<QStringLiteral("成交")<<QStringLiteral("资金比")<<QStringLiteral("3日")<<QStringLiteral("资金流")
               <<QStringLiteral("股息率")<<QStringLiteral("送转")<<QStringLiteral("总市值")<<QStringLiteral("流通市值")
               <<QStringLiteral("登记日")<<QStringLiteral("公告日");
    ui->hqtbl->setColumnCount(mHqHeaderList.length());
    ui->hqtbl->setHorizontalHeaderLabels(mHqHeaderList);
    ui->hqtbl->horizontalHeader()->setDefaultSectionSize(mSecSize);
    //开始给各列绑定数据
    ui->hqtbl->horizontalHeaderItem(0)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_NONE);
    ui->hqtbl->horizontalHeaderItem(1)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_NONE);
    ui->hqtbl->horizontalHeaderItem(2)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_PRICE);
    ui->hqtbl->horizontalHeaderItem(3)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_CHGPER);
    ui->hqtbl->horizontalHeaderItem(4)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_CJE);
    ui->hqtbl->horizontalHeaderItem(5)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_MONEYR);
    ui->hqtbl->horizontalHeaderItem(6)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_LAST3);
    ui->hqtbl->horizontalHeaderItem(7)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_ZJLX);
    ui->hqtbl->horizontalHeaderItem(8)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_GXL);
    ui->hqtbl->horizontalHeaderItem(9)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_SZZBL);
    ui->hqtbl->horizontalHeaderItem(10)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_TCAP);
    ui->hqtbl->horizontalHeaderItem(11)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_MCAP);
    ui->hqtbl->horizontalHeaderItem(12)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_GQDJR);
    ui->hqtbl->horizontalHeaderItem(13)->setData(Qt::UserRole+1, STK_DISPLAY_SORT_TYPE_NONE);


    for(int i=3; i<ui->hqtbl->columnCount(); i++)
    {
        ui->hqtbl->setColumnHidden(i, true);
    }
    mDisplayCol += 3;
    mRestartTimer = new QTimer(this);
    //mRestartTimer->setInterval(1000 * 60 *60);
    mRestartTimer->setInterval(1000*60);
    connect(mRestartTimer, SIGNAL(timeout()), this, SLOT(slotRestartMyself()));
    ui->blocktbl->verticalHeader()->setHidden(true);
    ui->hqtbl->setVisible(true);
    qDebug()<<"hqtbl visible:"<<ui->hqtbl->isVisible();
    mTargetSize.setWidth(mDisplayCol * mSecSize);
    mTargetSize.setHeight(QApplication::desktop()->availableGeometry(QApplication::desktop()->primaryScreen()).height());
    setTargetSize(mTargetSize);

    ui->closeBtn->setIcon(style()->standardPixmap(QStyle::SP_TitleBarCloseButton));
    ui->minBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton));
    //ui->srchBtn->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
#if 1
    //系统托盘
    QIcon appIcon = QIcon(":/icon/image/Baidu_96px.png");
    if(appIcon.isNull())
    {
        qDebug()<<"icon image is not found";
    }
    this->setWindowIcon(appIcon);
    systemIcon = new QSystemTrayIcon(this);
    systemIcon->setIcon(appIcon);
    systemIcon->show();
    connect(systemIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(setDlgShow(QSystemTrayIcon::ActivationReason)));
    connect(ui->hqtbl->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(setSortType(int)));
    connect(ui->blocktbl->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(setBlockSort(int)));

    //index更新
    QStringList indexlist;
    indexlist<<"sh000001"<<"sh000300"<<"sz399001"<<"sh000043"<<"sz399006";
    mIndexThread = new QSinaStkInfoThread(this);
    connect(mIndexThread, SIGNAL(sendStkDataList(StockDataList)), this, SLOT(slotUpdateIndex(StockDataList)));
    mIndexThread->setStkList(indexlist);
    mIndexThread->start();


    //行情初始化
//    mStockThread = new QSinaStkInfoThread;
//    connect(mStockThread, SIGNAL(sendStkDataList(StockDataList)), this, SLOT(updateHqTable(StockDataList)));
//    mStockThread->start();
    mBlockThread = new QEastMoneyBlockThread;
    connect(mBlockThread, SIGNAL(sendBlockDataList(BlockDataList)), this, SLOT(updateBlockTable(BlockDataList)));
    connect(mBlockThread, SIGNAL(updateBlockCodesFinished()), this, SLOT(displayBlockRealtimeInfo()));
    mBlockThread->start();
    mSearchThread = new QSinaSearchThread;
    connect(mSearchThread, SIGNAL(sendSearchResult(QStringList)), this, SLOT(displayBlockDetailInfoInTable(QStringList)));
    mSearchThread->start();
    mMergeThread = new QSinaStkResultMergeThread;
    connect(mMergeThread, SIGNAL(sendStkDataList(StockDataList)), this, SLOT(updateHqTable(StockDataList)));
    mMergeThread->setMktType(MKT_ZXG);
    connect(mBlockThread, SIGNAL(sendStkinfoUpdateProgress(int,int)), this, SLOT(slotUpdate(int,int)));
    connect(mBlockThread, SIGNAL(signalUpdateMsg(QString)), this, SLOT(slotUpdateMsg(QString)));
//    mMergeThread->start();

    //读取自选
    mFavStkList = Profiles::instance()->value(STK_ZXG_SEC, STK_ZXG_NAME).toStringList();
    mHSFoundsList = Profiles::instance()->value(STK_HSJJ_SEC, STK_ZXG_NAME).toStringList();
    mMergeThread->setSelfCodesList(mFavStkList);
    mMergeThread->setActive(true);
    mMergeThread->setMktType(MKT_ZXG);

#endif
//    on_zxgBtn_clicked();
    mCurBlockType = BLOCK_INDUSTORY;

    //创建快捷事件
    QShortcut *shotcut = new QShortcut(QKeySequence("Alt+X"), this);  //隐藏
    connect(shotcut, SIGNAL(activated()), this, SLOT(slotWhetherDisplay()));
    QShortcut *shotcut1 = new QShortcut(QKeySequence("Alt+A"), this);
    connect(shotcut1, SIGNAL(activated()), this, SLOT(slotDisplayAll()));
    QShortcut *shotcut2 = new QShortcut(QKeySequence("Alt+S"), this);
    connect(shotcut2, SIGNAL(activated()), this, SLOT(slotDisplayBlock()));
    QShortcut *shotcut3 = new QShortcut(QKeySequence("Alt+D"), this);
    connect(shotcut3, SIGNAL(activated()), this, SLOT(slotDisplayStockMini()));
    //setHook(this);
    mInit = false;

    iniHqCenterAction();
    ((QHBoxLayout*) ui->btnframe->layout())->setStretch(0, /*ui->blocktbl->columnCount()*/2);
    ((QHBoxLayout*) ui->btnframe->layout())->setStretch(1, /*ui->hqtbl->columnCount()*/3);
}

void Dialog::setDlgShow(QSystemTrayIcon::ActivationReason val)
{
    qDebug()<<"val:"<<val;
    switch (val) {
    case QSystemTrayIcon::DoubleClick:
        this->setVisible(!this->isVisible());
        break;
    case QSystemTrayIcon::Context:
        qDebug()<<"context coming";
    {
        QMenu *popMenu = new QMenu(this);
        QList<QAction*> actlist;
        QStringList poplist;
        poplist<<QStringLiteral("显示")<<QStringLiteral("退出");
        int index = -1;
        foreach (QString name, poplist) {
            index++;
            QAction *act = new QAction(this);
            act->setText(name);
            act->setData(index);
            connect(act, &QAction::triggered, this, &Dialog::slotSystemTrayMenuClicked);
            actlist.append(act);
        }

        popMenu->addActions(actlist);
        popMenu->popup(QCursor::pos());
    }
        break;
    default:
        break;
    }

}

void Dialog::slotSystemTrayMenuClicked()
{
    QAction *src = (QAction*) sender();
    if(!src) return;
    int val = src->data().toInt();
    if(val == 0)
    {
        //显示
        this->setVisible(true);
    } else
    {
        this->close();
    }

}

void Dialog::displayBlockRealtimeInfo()
{
    ui->updatelbl->clear();
//    MktCapFile::instance()->setValue("Update", "time", QDateTime::currentDateTime().toTime_t());
    if(mMergeThread)
    {
        mMergeThread->start();
        mMergeThread->setActive(true);
    }
    if(mBlockThread)
    {
        mBlockThread->SetUpdateBlockCodes(false);
        if(mBlockThread->isFinished())
        {
            mBlockThread->start();
        }
    }
}

Dialog::~Dialog()
{
    //unHook();
    qDebug()<<"close dialog now";
//    if(mStockThread) mStockThread->deleteLater();
    if(mBlockThread) mBlockThread->deleteLater();
    delete ui;
}

void Dialog::setSortType(int index)
{
    if(index < 2 || index > 13) return;
//    if(mStockThread && mStockThread->isActive())
//    {
//        mStockThread->setOptType((STK_DISPLAY_TYPE)(index-2));
//    }

    if(mMergeThread && mMergeThread->isActive())
    {
        mMergeThread->setSortType((STK_DISPLAY_TYPE)(index-2));
    }

}

void Dialog::setBlockSort(int val)
{
    qDebug("click val = %d, total = %d", val, ui->blocktbl->rowCount() -1);
    if(val != 1) return;
    if(mBlockThread) mBlockThread->reverseSortRule();
}

void Dialog::setBlockName()
{
    if(!mBlockThread) return;
    QAction *act = (QAction*)sender();
    if(act == NULL) return;
    int index = act->data().toInt();
    mCurBlockType = index;
    mBlockThread->setOptType((BLOCK_OPT_TYPE)index);
    qDebug()<<"act name:"<<act->text();

}

void Dialog::on_zxgBtn_clicked()
{
    if(mMergeThread) /*mMergeThread->setActive(false);*/
    {
        mMergeThread->setSelfCodesList(mFavStkList);
        mMergeThread->setMktType(MKT_ZXG);
    }
//    if(mStockThread)
//    {
//        mStockThread->setActive(true);
//        mStockThread->setStkList(mFavStkList);
//    }
}

void Dialog::iniHqCenterAction()
{
    mHqCenterMenu = new QMenu(QStringLiteral("行情中心"), this);
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
    QList<QAction*> actlist2;
    for(int i=2; i<mHqHeaderList.length(); i++)
    {
        QAction *act = new QAction(this);
        act->setText(mHqHeaderList[i]);
        TableColDisplayStatus data;
        data.mTable = ui->hqtbl;
        data.mColIndex = i;
        data.mIsDisplay = true;
        //act->setData(i);
        act->setCheckable(true);
        data.mIsDisplay = i>2? false : true;
        act->setChecked(data.mIsDisplay);
        act->setData(QVariant::fromValue(data));
        connect(act, &QAction::triggered, this, &Dialog::setDisplayCol);
        actlist2.append(act);
    }

    mHqHeaderMenu->addActions(actlist2);
}

void Dialog::setDisplayCol(bool isDisplay)
{
    QAction *act = (QAction*)sender();
    if(act == NULL) return;
    TableColDisplayStatus sts = act->data().value<TableColDisplayStatus>();
    int col = sts.mColIndex;
    if(ui->hqtbl->isColumnHidden(col) && isDisplay)
    {
        mDisplayCol++;
        ui->hqtbl->setColumnHidden(col, false);
    } else if((!ui->hqtbl->isColumnHidden(col)) && (!isDisplay))
    {
        mDisplayCol--;
        ui->hqtbl->setColumnHidden(col, true);
    }
    mTargetSize.setWidth(mDisplayCol * mSecSize);
    setTargetSize(mTargetSize);
}

void Dialog::setStockMarket()
{
    //if(mStockThread) mStockThread->setActive(false);
    if(mMergeThread)
    {
        mMergeThread->setActive(true);
        QAction *act = (QAction*)sender();
        if(act == NULL) return;
        qDebug()<<"mkt_type:"<<act->data().toInt();
        MktType type = (MktType)(act->data().toInt());
        if(type != MKT_JJ && type != MKT_ZXG)
        {
            mMergeThread->setMktType((MktType)(act->data().toInt()));
        } else
        {
            qDebug()<<mHSFoundsList;
            if(type ==MKT_JJ)mMergeThread->setSelfCodesList(mHSFoundsList);
            else mMergeThread->setSelfCodesList(mFavStkList);
            mMergeThread->setMktType(MKT_OTHER);
        }
    }

}

void Dialog::on_blkbtn_clicked()
{
    QMenu *popMenu = new QMenu(this);
    QList<QAction*> actlist;

    QStringList poplist;
    poplist<<QStringLiteral("地域")<<QStringLiteral("行业")<<QStringLiteral("概念");
    int index = 0;
    foreach (QString name, poplist) {
        index++;
        QAction *act = new QAction(this);
        act->setText(name);
        act->setData(index);
        connect(act, &QAction::triggered, this, &Dialog::setBlockName);
        actlist.append(act);
    }

    popMenu->addActions(actlist);
    popMenu->popup(QCursor::pos());

}

void Dialog::on_zjlxBtn_clicked()
{

}

void Dialog::on_lhbBtn_clicked()
{

}

void Dialog::on_closeBtn_clicked()
{
    this->hide();
}

void Dialog::on_srchBtn_clicked()
{
    //重新更新数据资料

}

void Dialog::slotUpdateMsg(const QString &msg)
{
    ui->updatelbl->setText(msg);
}

void Dialog::on_minBtn_clicked()
{
    this->hide();
}

void Dialog::resizeEvent(QResizeEvent *event)
{
    qDebug()<<"target size:"<<mTargetSize<<"param size:"<<event->size();
    if(mInit)
    {
        ui->topframe->setVisible(ui->hqtbl->isVisible() && ui->blocktbl->isVisible());
        ui->hqframe->setVisible(ui->hqtbl->isVisible());
    } else
    {
        mInit = true;
    }
    QDialog::resizeEvent(event);
}

//void Dialog::HQLISIINFOCBKFUNC(StockDataList& pDataList, void *pUser)
//{
//    Dialog *pDlg = (Dialog*)pUser;
//    if(pDlg == NULL) return;

//    pDlg->updateHqTable(pDataList);
//}

void Dialog::updateHqTable(const StockDataList& pDataList)
{
//    qDebug()<<"input";
    //qDebug()<<"main Thread:"<<QThread::currentThreadId();
    if(pDataList.length() == 0) return;
    ui->hqtbl->setRowCount(pDataList.count());
    int i=0;
    foreach (StockData data, pDataList) {
        if(data.name.isEmpty()) continue;
        int k =0;
//        qDebug()<<data.code;
        ui->hqtbl->setRowHeight(i, 20);
        ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(data.code, Qt::AlignRight));
        ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(data.name));
        QString tempStr;
        ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(tempStr.sprintf("%.2f", data.cur)));
        //ui->hqtbl->setItem(i, k++, new QTableWidgetItem(tempStr.sprintf("%.2f", data.chg)));
//        if(!mStockMap.contains(data.code))
//        {
//            ui->hqtbl->setItem(i, k++, new QTableWidgetItem(tempStr.sprintf("%.2f%%", data.per)));
//        } else {
            double val = mStockMap[data.code];
            QString up = QString::fromLocal8Bit("↑");
            QString down = QString::fromLocal8Bit("↓");
            if(val > data.per)
            {
                ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(QString("%1%2%").arg(down).arg(QString::number(data.per, 'f', 2))));
            } else if(val < data.per)
            {
               ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(QString("%1%2%").arg(up).arg(QString::number(data.per, 'f', 2))));
            } else {
               ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(QString("%1%2%").arg("").arg(QString::number(data.per, 'f', 2))));
            }

//        }
        mStockMap[data.code] = data.per;
        //ui->hqtbl->setItem(i, k++, new QTableWidgetItem(QString::number(data.vol / 10000) + QStringLiteral("万")));
        if(data.money >= 1000){
            ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(tempStr.sprintf("%.2f", data.money / 10000.0) + QStringLiteral("亿")));
        } else {
            ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(tempStr.sprintf("%.0f", data.money) + QStringLiteral("万")));
        }
        //ui->hqtbl->setItem(i, k++, new QTableWidgetItem(QString::number(data.money)));
        ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(tempStr.sprintf("%.2f",data.money_ratio)));
        ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(tempStr.sprintf("%.2f",data.last_three_pers)));
        if(fabs(data.zjlx) >= 1000){
            ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(tempStr.sprintf("%.2f", data.zjlx / 10000.0) + QStringLiteral("亿")));
        } else {
            ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(tempStr.sprintf("%.0f", data.zjlx) + QStringLiteral("万")));
        }
        ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(tempStr.sprintf("%.2f%(%.2f)",data.gxl * 100, data.xjfh)));
        ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(tempStr.sprintf("%.0f",data.szzbl)));
        ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(tempStr.sprintf("%.0f",data.totalCap / 100000000.0 ) + QStringLiteral("亿")));
        ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(tempStr.sprintf("%.0f",data.mutalbleCap/ 100000000.0 )+ QStringLiteral("亿")));

        ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(data.gqdjr));
        ui->hqtbl->setItem(i, k++, new HqTableWidgetItem(data.yaggr));

#if 0
        int btnindex = k;
        QPushButton* btn = (QPushButton*)(ui->hqtbl->cellWidget(i, btnindex));
        if(!btn)
        {
            btn = new QPushButton(this);
            btn->setStyleSheet("border:none;color:blue;");
            connect(btn, SIGNAL(clicked()), this, SLOT(editFavorite()));
        }
#endif
        QString code = data.code;
        if(code.left(1) == "5" || code.left(1) == "6")
        {
            code = "sh"+code;
        } else
        {
            code = "sz"+code;
        }
        if(!mFavStkList.contains(code))
        {
           // btn->setText(QStringLiteral("添加"));
            //ui->hqtbl->item(i, 0)->setIcon(QIcon(""));
        } else
        {
            //btn->setText(QStringLiteral("删除"));
            ui->hqtbl->item(i, 0)->setIcon(QIcon(":/icon/image/zxg.ico"));
        }
        //btn->setProperty("code", code);
        //ui->hqtbl->setCellWidget(i, btnindex, btn);
        ui->hqtbl->item(i, 0)->setData(Qt::UserRole, code);
//        qDebug()<<"data.blocklist:"<<data.blocklist;
        ui->hqtbl->item(i, 0)->setData(Qt::UserRole+1, data.blocklist);
        i++;

    }


}

//void Dialog::HQBLOCKINFOCBKFUNC(BlockDataList& pDataList, void *pUser)
//{
//    Dialog *pDlg = (Dialog*)pUser;
//    if(pDlg == NULL) return;

//    pDlg->updateBlockTable(pDataList);
//}

void Dialog::updateBlockTable(const BlockDataList& pDataList)
{
    qDebug()<<"input:"<<pDataList.length();
    if(pDataList.length() == 0) return;

    //ui->blocktbl->clearContents();
    int totalrow = pDataList.count();
    int i=0;
    foreach (BlockData data, pDataList) {
        mBlockNameMap[data.code] = data.name;
        mBlockStkList[data.code] = data.stklist;
        if((data.code.left(1) == "1" && mCurBlockType != BLOCK_DISTRICT)||
           (data.code.left(1) == "2" && mCurBlockType != BLOCK_INDUSTORY) ||
           (data.code.left(1) == "3" && mCurBlockType != BLOCK_CONCEPT))
        {
            mBlockMap[data.code] = data.changePer;
            ui->blocktbl->setRowCount(--totalrow );
            continue;
        }
        int k =0;
        ui->blocktbl->setRowHeight(i, 20);
        ui->blocktbl->setItem(i, k++, new HqTableWidgetItem(data.name));

        //ui->blocktbl->setItem(i, k++, new QTableWidgetItem(QString::number(data.mktkap)));
        QString tempStr = QString("%1%2%");
        QString up = QString::fromLocal8Bit("↑");
        QString down = QString::fromLocal8Bit("↓");
        if(mBlockMap[data.code] > data.changePer)
        {
            ui->blocktbl->setItem(i, k++, new HqTableWidgetItem(tempStr.arg(down).arg(QString::number(data.changePer, 'f', 2))));
        } else if(mBlockMap[data.code] < data.changePer)
        {
            ui->blocktbl->setItem(i, k++, new HqTableWidgetItem(tempStr.arg(up).arg(QString::number(data.changePer, 'f', 2))));
        } else
        {
            ui->blocktbl->setItem(i, k++, new HqTableWidgetItem(tempStr.arg("").arg(QString::number(data.changePer, 'f', 2))));
        }
        mBlockMap[data.code] = data.changePer;
        QVariant val;
        val.setValue(data);
        ui->blocktbl->item(i, 0)->setData(Qt::UserRole, val);
        i++;

    }


}

void Dialog::on_blocktbl_itemDoubleClicked(QTableWidgetItem *item)
{
    qDebug()<<"double block";
    if(item == NULL) return;
    QTableWidgetItem *wkItem = item;
    if(wkItem->column() != 0){
        wkItem = ui->blocktbl->item(item->row(), 0);
    }
    BlockData data = wkItem->data(Qt::UserRole).value<BlockData>();
    qDebug()<<"code:"<<data.code<<" name:"<<data.name;
    //int code = wkItem->data(Qt::UserRole).toInt();
    qDebug()<<"code:"<<data.stklist;
   displayBlockDetailInfoInTable(data.stklist);
}

void Dialog::displayBlockDetailInfoInTable(const QStringList& stklist)
{
    if(mMergeThread) /*mMergeThread->setActive(false);*/
    {
        mMergeThread->setMktType(MKT_OTHER);
        mMergeThread->setSelfCodesList(stklist);
    }
//    if(mStockThread)
//    {
//        mStockThread->setActive(true);
//        mStockThread->setStkList(stklist);
//    }


}

void Dialog::on_blocktbl_customContextMenuRequested(const QPoint &pos)
{
    qDebug()<<"right menu clicked";
    QMenu *popMenu = new QMenu(this);
    QList<QAction*> actlist;

    QStringList poplist;
    poplist<<QStringLiteral("概念")<<QStringLiteral("行业")<<QStringLiteral("地域");
    int index = 4;
    foreach (QString name, poplist) {
        index--;
        QAction *act = new QAction(this);
        act->setText(name);
        act->setData(index);
        connect(act, &QAction::triggered, this, &Dialog::setBlockName);
        actlist.append(act);
    }

    popMenu->addActions(actlist);
   qDebug()<<"cursor pos:"<<QCursor::pos() <<" param pos:"<<pos;
   // qDebug()<<"parent:"<<((QWidget*)(ui->blkbtn->parent()))->geometry();
   // qDebug()<<"pos:"<<ui->blkbtn->pos();
    popMenu->popup(QCursor::pos());
   // popMenu->popup(ui->blocktbl->mapFromGlobal(pos));
}

void Dialog::on_hqtbl_customContextMenuRequested(const QPoint &pos)
{
    QMenu *popMenu = new QMenu(this);
    popMenu->addMenu(mHqCenterMenu);
    popMenu->addMenu(mHqHeaderMenu);

    //自选股编辑
    QTableWidgetItem *item = ui->hqtbl->itemAt(pos);
    if(item)
    {        
        QList<QAction*> actlist;
        int row = item->row();
        item = ui->hqtbl->item(row, 0);
        QStringList poplist;
        poplist<<QStringLiteral("分时图")<<QStringLiteral("日线图");
        QList<int> Optlist;
        Optlist<<MENU_OPT_MINUTE<<MENU_OPT_DAY;
        int index = -1;
        foreach (QString name, poplist) {
            index++;
            QAction *act = new QAction(this);
            act->setText(name);
            act->setData(Optlist[index]);
            connect(act, &QAction::triggered, this, &Dialog::hqMenuOpt);
            actlist.append(act);
        }
        QMenu *submenu = new QMenu(QStringLiteral("所属板块"), this);
        QStringList blocklist = item->data(Qt::UserRole+1).toStringList();
        qDebug()<<"blocklist:"<<blocklist<<" code:"<<item->data(Qt::UserRole).toString();
        foreach (QString name, blocklist) {
            if(name.trimmed().isEmpty()) continue;
            if(mBlockNameMap[name].trimmed().isEmpty()) continue;
            QAction *act = new QAction(this);
            act->setText(QString("%1:%2%").arg(mBlockNameMap[name]).arg(mBlockMap[name]));
            qDebug()<<"subtext:"<<act->text();
            act->setData(name);
            connect(act, &QAction::triggered, this, &Dialog::hqMenuOpt);
            submenu->addAction(act);
        }

        popMenu->addActions(actlist);
        popMenu->addMenu(submenu);
    }

    popMenu->popup(QCursor::pos());
}

void Dialog::editFavorite()
{
    QPushButton* send = (QPushButton*) sender();
    if(!send) return;

    QString code = send->property("code").toString();
    if(mFavStkList.contains(code))
    {
        mFavStkList.removeAll(code);
    } else
    {
        mFavStkList.append(code);
    }
    if(mMergeThread && mMergeThread->getMktType() == MKT_ZXG) mMergeThread->setSelfCodesList(mFavStkList);
    Profiles::instance()->setValue(STK_ZXG_SEC, STK_ZXG_NAME, mFavStkList);
    qDebug()<<"fav:"<<mFavStkList;
}

void Dialog::on_searchTxt_textChanged(const QString &arg1)
{
    if(mSearchThread)
    {
        mSearchThread->setSearchString(arg1);
    }
//    if(mMergeThread)
//    {
//        mMergeThread->setActive(false);
//    }
//    if(mStockThread)
//    {
//        mStockThread->setActive(true);
//    }

}

void Dialog::slotUpdate(int cur, int total)
{
 //   qDebug()<<"cur:"<<cur<<" total:"<<total;
    ui->updatelbl->setText(QString("%1/%2").arg(cur).arg(total));
}

void Dialog::hqMenuOpt()
{
    QAction *act = (QAction*)sender();
    if(act == NULL) return;
    QString opt = act->data().toString();
    if(opt.length())
    {
        displayBlockDetailInfoInTable(mBlockStkList[opt]);
    }
}

void Dialog::on_DateMgrBtn_clicked()
{
    QExchangeDateMangageDialog *dlg = new QExchangeDateMangageDialog;
    if(dlg == NULL) return;
    connect(dlg, SIGNAL(accepted()), dlg, SLOT(deleteLater()));
    dlg->exec();
}

void Dialog::on_hqtbl_itemDoubleClicked(QTableWidgetItem *item)
{
    if(!item) return;
    if(item->column() != 0) return;

    QString code = item->data(Qt::UserRole).toString();
    if(mFavStkList.contains(code))
    {
        mFavStkList.removeAll(code);
    } else
    {
        mFavStkList.append(code);
    }
    if(mMergeThread && mMergeThread->getMktType() == MKT_ZXG) mMergeThread->setSelfCodesList(mFavStkList);
    Profiles::instance()->setValue(STK_ZXG_SEC, STK_ZXG_NAME, mFavStkList);
    qDebug()<<"fav:"<<mFavStkList;
}

void Dialog::slotUpdateIndex(const StockDataList &pDataList)
{
    //indexlist<<"sh000001"<<"sh000300"<<"sz399001"<<"sh000043"<<"sz399006";
    foreach (StockData data, pDataList) {
        if(data.code == "000001")
        {
            ui->shlbl->setText(QString("%1 %2%").arg(data.cur).arg(data.per, 0, 'f', 2));
        } else if(data.code == "399001")
        {
            ui->szlbl->setText(QString("%1 %2%").arg(data.cur).arg(data.per, 0, 'f', 2));
        } else if(data.code == "000300")
        {
            ui->hslbl->setText(QString("%1 %2%").arg(data.cur).arg(data.per, 0, 'f', 2));
        } else if(data.code == "000043")
        {
            ui->dpglbl->setText(QString("%1 %2%").arg(data.cur).arg(data.per, 0, 'f', 2));
        } else if(data.code == "399006")
        {
            ui->cyblbl->setText(QString("%1 %2%").arg(data.cur).arg(data.per, 0, 'f', 2));
        }
    }

}

void Dialog::on_hqtbl_itemEntered(QTableWidgetItem *item)
{
    QTableWidgetItem *wkitem = ui->hqtbl->item(item->row(), 0);
    //qDebug()<<"entered item:"<<wkitem->data(Qt::UserRole).toString();
}

//void Dialog::keyPressEvent(QKeyEvent *e)
//{
//    qDebug()<<__LINE__<<__FUNCTION__;

//}


void Dialog::slotWhetherDisplay()
{
    this->setVisible(!this->isVisible());
}

void Dialog::slotRestartMyself()
{
    QProcess::execute("restart.bat");
}

void Dialog::on_MainDialog_customContextMenuRequested(const QPoint &pos)
{

}

void Dialog::slotDisplayAll()
{
    mDisplayMode = E_DISPLAY_ALL;
    ui->hqtbl->setVisible(true);
    ui->blocktbl->setVisible(true);
    for(int i=0; i<ui->hqtbl->horizontalHeader()->count(); i++)
    {
        ui->hqtbl->setColumnHidden(i, false);

    }
    mDisplayCol = ui->blocktbl->horizontalHeader()->count() + ui->hqtbl->horizontalHeader()->count();
    mTargetSize.setWidth(mDisplayCol * mSecSize);
    setTargetSize(mTargetSize);

}

void Dialog::slotDisplayBlock()
{
    mDisplayMode = E_DISPLAY_BLOCK;

    ui->blocktbl->setVisible(true);
    ui->hqtbl->setVisible(false);
    mDisplayCol = ui->blocktbl->horizontalHeader()->count();
    mTargetSize.setWidth(mDisplayCol * mSecSize);
    setTargetSize(mTargetSize);
}

void Dialog::slotDisplayStockFull()
{
    mDisplayMode = E_DISPLAY_STOCK_FULL;
}


void Dialog::slotDisplayStockMini()
{
    if(mDisplayMode != E_DISPLAY_STOCK_MINI)
    {
        mDisplayMode = E_DISPLAY_STOCK_MINI;
    }
    ui->blocktbl->setVisible(false);
    ui->hqtbl->setVisible(true);
    mDisplayCol = 0;
    for(int i=0; i<ui->hqtbl->columnCount(); i++)
    {
        if(!ui->hqtbl->isColumnHidden(i)) mDisplayCol++;
    }
    mTargetSize.setWidth(mDisplayCol * mSecSize);
    setTargetSize(mTargetSize);
}

void Dialog::setTargetSize(const QSize &size)
{
    int screenW =  QApplication::desktop()->availableGeometry(QApplication::desktop()->primaryScreen()).width();
    int screenH =  QApplication::desktop()->availableGeometry(QApplication::desktop()->primaryScreen()).height();
    this->setGeometry(screenW-size.width(), screenH - size.height(), size.width(), size.height());
}
