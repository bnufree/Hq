#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QMenu>
//#include "profiles.h"
#include "stkmktcap.h"
#include <QDateTime>
#include "qexchangedatemangagedialog.h"
#include "qeastmoneyzjlxthread.h"
#include <QShortcut>
//#include "qthook.h"
#include <QProcess>
#include <QDesktopWidget>
#include <QResizeEvent>
#include "qindexwidget.h"
#include "qeastmonystockcodesthread.h"
#include "qeastmoneychinashareexchange.h"
#include "qeastmoneynorthboundthread.h"
#include "qeastmoneyhsgtdialog.h"
#include "./history/qsharehistoryinfomgr.h"
#include "qhttpget.h"
#include "exchange/qexchangerecordworker.h"
#include "qeastmoneyhsgtshareamount.h"

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
    QDialog(parent),mBlockMgr(NULL)/*,mStockThread(NULL)*/,mSearchThread(NULL),mDisplayCol(0),mMergeThread(0),
    mShareHistoryMgr(0),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);
    mDisplayMode = E_DISPLAY_ALL;
    ui->closeBtn->setIcon(style()->standardPixmap(QStyle::SP_TitleBarCloseButton));
    ui->minBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton));
    //ui->srchBtn->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
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
    //数据库初始化
    DATA_SERVICE->signalInitDBTables();
    connect(ui->hqtbl, SIGNAL(signalSetFavCode(QString)), DATA_SERVICE, SLOT(slotSetFavCode(QString)));

#if 0
    //开始更新历史信息，龙虎榜信息，沪港通信息
    QShareHistoryInfoMgr *mgr = new QShareHistoryInfoMgr();
    connect(mgr, SIGNAL(signalUpdateProcess(int,int)), this, SLOT(slotUpdate(int,int)));
    connect(mgr, SIGNAL(signalHistoryDataFinished()), this, SLOT(slotHistoryDataFinish()));
    connect(mgr, SIGNAL(signalUpdateAmountProcess(QString)), this, SLOT(slotUpdateMsg(QString)));


    //更新记录

    QExchangeRecordWorker *work = new QExchangeRecordWorker;
    connect(work, SIGNAL(signalSendStkProfitList(StockDataList)), DATA_SERVICE, SIGNAL(signalUpdateStkProfitList(StockDataList)));
    connect(work, SIGNAL(signalSendCodeList(QStringList)), this, SLOT(slotUpdateFavList(QStringList)));
    work->signalStartImport("test.xlsx");
#endif
   QEastMonyStockCodesThread *codesThread = new QEastMonyStockCodesThread;
   connect(codesThread, SIGNAL(signalSendCodesList(QStringList)), this, SLOT(slotUpdateStockCodesList(QStringList)));
//   connect(codesThread, SIGNAL(finished()), codesThread, SLOT(deleteLater()));
   codesThread->start();
    //创建快捷事件
    QShortcut *shotcut = new QShortcut(QKeySequence("Alt+X"), this);  //隐藏
    connect(shotcut, SIGNAL(activated()), this, SLOT(slotWhetherDisplay()));
    QShortcut *shotcut1 = new QShortcut(QKeySequence("Alt+A"), this);
    connect(shotcut1, SIGNAL(activated()), this, SLOT(slotDisplayAll()));
    QShortcut *shotcut2 = new QShortcut(QKeySequence("Alt+S"), this);
    connect(shotcut2, SIGNAL(activated()), this, SLOT(slotDisplayBlock()));
    QShortcut *shotcut3 = new QShortcut(QKeySequence("Alt+D"), this);
    connect(shotcut3, SIGNAL(activated()), this, SLOT(slotDisplayStockMini()));
//    setHook(this);
    mInit = false;
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

Dialog::~Dialog()
{
    //unHook();
    delete ui;
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

void Dialog::closeEvent(QCloseEvent *event)
{
    DATA_SERVICE->saveShares();
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
        //ui->hqframe->setVisible(ui->hqtbl->isVisible());
    } else
    {
        mInit = true;
    }
    QDialog::resizeEvent(event);
}

void Dialog::updateHqTable(const StockDataList& pDataList)
{
    ui->hqtbl->setDataList(pDataList);
}

void Dialog::updateBlockTable(const BlockDataVList& pDataList)
{
    ui->blocktbl->setDataList(pDataList);
}
void Dialog::on_searchTxt_textChanged(const QString &arg1)
{
    if(mSearchThread)
    {
        mSearchThread->slotRecvSearchString(arg1);
    }

}

void Dialog::slotUpdate(int cur, int total)
{
 //   qDebug()<<"cur:"<<cur<<" total:"<<total;
    ui->updatelbl->setText(QString("正在更新日线数据：%1/%2").arg(cur).arg(total));
}



void Dialog::on_DateMgrBtn_clicked()
{
    QExchangeDateMangageDialog *dlg = new QExchangeDateMangageDialog;
    if(dlg == NULL) return;
    connect(dlg, SIGNAL(accepted()), dlg, SLOT(deleteLater()));
    dlg->exec();
}



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
    mDisplayCol = 4;
    foreach (QAction* act, mHqColActList) {
        if(act->isChecked())
        {
            mDisplayCol++;
        }
    }
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
    this->resize(size);
        windowPos = QPoint(screenW-size.width(), screenH - size.height()-20);
        this->move(windowPos);
        //this->setGeometry(screenW-size.width(), screenH - size.height()-20, size.width(), size.height());

}

void Dialog::mousePressEvent(QMouseEvent *event)

{
    qDebug()<<__FUNCTION__<<__LINE__;

     this->windowPos = this->pos();                // 获得部件当前位置

     this->mousePos = event->globalPos();     // 获得鼠标位置

     this->dPos = mousePos - windowPos;       // 移动后部件所在的位置

}



void Dialog::mouseMoveEvent(QMouseEvent *event)

{
      //  qDebug()<<__FUNCTION__<<__LINE__;
     //this->move(event->globalPos() - this->dPos);

}

void Dialog::slotUpdateStockCodesList(const QStringList &list)
{
    qDebug()<<"update code finshed:"<<list.length();
    //更新股本信息等
    if(!mShareHistoryMgr)
    {
        mShareHistoryMgr = new QShareHistoryInfoMgr(list);
        connect(mShareHistoryMgr, SIGNAL(signalUpdateHistoryMsg(QString)),
                this, SLOT(slotUpdateMsg(QString)));
    }
    mShareHistoryMgr->signalGetFianceInfo();
    mAllStkList = list;
    //更新指数
    QIndexWidget *indexw = new QIndexWidget(this);
    ui->verticalLayout->insertWidget(0, indexw);
    QStringList indexlist;
    indexlist<<"s_sh000001"<<"s_sh000300"<<"s_sz399001"<<"s_sz399006"<<"s_sh000016"<<"s_sh000010";
    foreach (QString code, indexlist) {
        indexw->insetWidget(code);
    }
    mIndexThread = new QSinaStkInfoThread(0);
    connect(mIndexThread, SIGNAL(sendStkDataList(StockDataList)), indexw, SLOT(updateData(StockDataList)));
    connect(mIndexThread, SIGNAL(finished()), mIndexThread, SLOT(deleteLater()));
    mIndexThread->signalSetStkList(indexlist);
    QEastmoneyNorthBoundThread *north = new QEastmoneyNorthBoundThread();
    connect(north, SIGNAL(signalUpdateNorthBoundList(StockDataList)), indexw, SLOT(updateData(StockDataList)));
    connect(north, SIGNAL(finished()), north, SLOT(deleteLater()));
    north->start();
    //行情中心初始化开始为自选股
    //读取自选
    if(mFavStkList.length() == 0) mFavStkList = Profiles::instance()->value(STK_ZXG_SEC, STK_ZXG_NAME).toStringList();
    mHSFoundsList = Profiles::instance()->value(STK_HSJJ_SEC, STK_ZXG_NAME).toStringList();
    mMergeThread = new QSinaStkResultMergeThread();
    connect(mMergeThread, SIGNAL(sendStkDataList(StockDataList)), this, SLOT(updateHqTable(StockDataList)));
    connect(ui->hqtbl, SIGNAL(signalSetDisplayHSHK(QString)), this, SLOT(slotUpdateHSGTOfCode(QString)));
    connect(ui->hqtbl, SIGNAL(signalSetStockMarket(int)), mMergeThread, SLOT(setMktType(int)));
    connect(ui->hqtbl, SIGNAL(signalSetSortType(int)), mMergeThread, SLOT(setSortType(int)));
    connect(ui->hqtbl, SIGNAL(signalDisplayPage(int)), mMergeThread, SLOT(setDisplayPage(int)));
    connect(ui->hqtbl, SIGNAL(signalSetDisplayBlockDetail(QStringList)), mMergeThread, SLOT(setSelfCodesList(QStringList)));
    connect(ui->blocktbl, SIGNAL(signalDisplayBlockDetailCodesList(QStringList)), mMergeThread, SLOT(setSelfCodesList(QStringList)));
    mMergeThread->setStkList(mAllStkList);
    mMergeThread->setSelfCodesList(mFavStkList);
    mMergeThread->setActive(true);
    mMergeThread->setMktType(MKT_ZXG);
    mMergeThread->start();
    //板块行情初始化
    //mCurBlockType = BLOCK_HY;
    mBlockMgr = new QEastMoneyBlockMangagerThread();
    connect(mBlockMgr, SIGNAL(signalBlockDataListUpdated(BlockDataVList)), this, SLOT(updateBlockTable(BlockDataVList)));
    connect(ui->blocktbl, SIGNAL(signalSetSortType(int)), mBlockMgr, SLOT(reverseSortRule()));
    connect(ui->blocktbl, SIGNAL(signalSetBlockType(int)), mBlockMgr, SLOT(setCurBlockType(int)));

    mBlockMgr->start();

    //查询接口初始化
    mSearchThread = new QSinaSearchThread(this);
    connect(mSearchThread, SIGNAL(sendSearchResult(QStringList)), mMergeThread, SLOT(setSelfCodesList(QStringList)));
}

void Dialog::on_HSGTBTN_clicked()
{
//    ui->HSGTBTN->setStyleSheet("background-color:transparent");
//    QEastMoneyHSGTDialog* dlg = new QEastMoneyHSGTDialog;
//    dlg->setModal(false);
//    dlg->show();
    if(!mShareHistoryMgr)
    {
        return;
    }
    mShareHistoryMgr->signalUpdateAllShareFrom20170317();

}

void Dialog::slotTodayHSGUpdated()
{
    ui->HSGTBTN->setStyleSheet("background-color:red");
}

void Dialog::slotUpdateHSGTOfCode(const QString &code)
{
    QEastMoneyHSGTDialog *dlg = new QEastMoneyHSGTDialog ;
    dlg->setModal(false);
    dlg->slotDisplayCode(code);
    dlg->show();
}
