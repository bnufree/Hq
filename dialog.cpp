#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QMenu>
#include "utils/profiles.h"
#include <QDateTime>
#include "qexchangedatemangagedialog.h"
#include <QShortcut>
//#include "qthook.h"
#include <QProcess>
#include <QDesktopWidget>
#include <QResizeEvent>
#include "qindexwidget.h"
#include "basic_info/qsharebasicinfoworker.h"
#include "history/qsharehistorydialog.h"
#include "hqtaskmagrcenter.h"

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
    QDialog(parent),mTaskMgr(0),mIndexWidget(0),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    //指数显示
    if(!mIndexWidget)
    {
        mIndexWidget = new QIndexWidget(this);
        ui->verticalLayout->insertWidget(0, mIndexWidget);
    }
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

    //创建快捷事件
    QShortcut *shotcut = new QShortcut(QKeySequence("Alt+X"), this);  //隐藏
    connect(shotcut, SIGNAL(activated()), this, SLOT(slotWhetherDisplay()));
    QShortcut *shotcut1 = new QShortcut(QKeySequence("Alt+A"), this);
    connect(shotcut1, SIGNAL(activated()), this, SLOT(slotDisplayAll()));
    QShortcut *shotcut2 = new QShortcut(QKeySequence("Alt+S"), this);
    connect(shotcut2, SIGNAL(activated()), this, SLOT(slotDisplayBlock()));
    QShortcut *shotcut3 = new QShortcut(QKeySequence("Alt+D"), this);
    connect(shotcut3, SIGNAL(activated()), this, SLOT(slotDisplayShareMini()));
    //    setHook(this);

    //
    mTaskMgr = new HQTaskMagrCenter;
    connect(ui->hqtbl, SIGNAL(signalSetFavCode(QString)), mTaskMgr, SIGNAL(signalSetFavCode(QString)));
    connect(mTaskMgr, SIGNAL(signalSendIndexCodesList(QStringList)), this, SLOT(slotRecvIndexCodesList(QStringList)));
    connect(mTaskMgr, SIGNAL(signalSendIndexRealDataList(ShareDataList)), mIndexWidget, SLOT(updateData(ShareDataList)));
    connect(mTaskMgr, SIGNAL(signalSendShareRealDataList(ShareDataList)), this, SLOT(updateHqTable(ShareDataList)));
    connect(ui->hqtbl, SIGNAL(signalSetDisplayHSHK(QString)), this, SLOT(slotUpdateHSGTOfCode(QString)));
    connect(ui->hqtbl, SIGNAL(signalSetShareMarket(int)), mTaskMgr, SLOT(setMktType(int)));
    connect(ui->hqtbl, SIGNAL(signalSetSortType(int)), mTaskMgr, SLOT(setSortType(int)));
    connect(ui->hqtbl, SIGNAL(signalDisplayPage(int)), mTaskMgr, SLOT(setDisplayPage(int)));
    connect(ui->hqtbl, SIGNAL(signalSetDisplayBlockDetail(QStringList)), mTaskMgr, SLOT(setSelfCodesList(QStringList)));
    connect(ui->blocktbl, SIGNAL(signalDisplayBlockDetailCodesList(QStringList)), mTaskMgr, SLOT(setSelfCodesList(QStringList)));
    connect(mTaskMgr, SIGNAL(signalBlockDataListUpdated(BlockDataVList)), this, SLOT(updateBlockTable(BlockDataVList)));
    connect(ui->blocktbl, SIGNAL(signalSetSortType(int)), mTaskMgr, SLOT(reverseSortRule()));
    connect(ui->blocktbl, SIGNAL(signalSetBlockType(int)), mTaskMgr, SLOT(setCurBlockType(int)));
    connect(mTaskMgr, SIGNAL(signalUpdateHistoryMsg(QString)), this, SLOT(slotUpdateMsg(QString)));
    //

    mTaskMgr->signalStart();
}


void Dialog::slotRecvIndexCodesList(const QStringList &list)
{
    foreach (QString code, list) {
        mIndexWidget->insetWidget(code);
    }
}

void Dialog::setDlgShow(QSystemTrayIcon::ActivationReason val)
{
    switch (val) {
    case QSystemTrayIcon::DoubleClick:
        this->setVisible(!this->isVisible());
        break;
    case QSystemTrayIcon::Context:
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
    QDialog::resizeEvent(event);
}

void Dialog::updateHqTable(const ShareDataList& pDataList)
{
    ui->hqtbl->setDataList(pDataList);
}

void Dialog::updateBlockTable(const BlockDataVList& pDataList)
{
    ui->blocktbl->setDataList(pDataList);
}
void Dialog::on_searchTxt_textChanged(const QString &arg1)
{
    if(mTaskMgr)
    {
        mTaskMgr->signalSearchCodesOfText(arg1.toUpper());
    }

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

void Dialog::slotDisplayShareFull()
{
    mDisplayMode = E_DISPLAY_Share_FULL;
}


void Dialog::slotDisplayShareMini()
{
    if(mDisplayMode != E_DISPLAY_Share_MINI)
    {
        mDisplayMode = E_DISPLAY_Share_MINI;
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

void Dialog::on_HSGTBTN_clicked()
{
    if(!mTaskMgr)
    {
        return;
    }
    //mTaskMgr->signalUpdateAllShareHistoryFromDate(true, QDate(2017,3,17));

}

void Dialog::slotTodayHSGUpdated()
{
    ui->HSGTBTN->setStyleSheet("background-color:red");
}

void Dialog::slotUpdateHSGTOfCode(const QString &code)
{
    QShareHistoryDialog *dlg = new QShareHistoryDialog(code) ;
    dlg->setModal(false);
    dlg->show();
}
