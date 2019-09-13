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
#include "table/qsharetablewidget.h"
#include "qandroidlistwidget.h"
#include "utils/comdatadefines.h"
#include <QDir>
#include "real/qnorthflowinfodisplaywidget.h"
#include "table/qshareforeignvoltablewidget.h"

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
    ui(new Ui::MainDialog),systemIcon(0)
{
    qDebug()<<__func__<<__LINE__;
    ui->setupUi(this);
    while(ui->mainStackWidget->count())
    {
        ui->mainStackWidget->removeWidget(ui->mainStackWidget->widget(0));
    }
    qDebug()<<__func__<<__LINE__;
    mShareTableWidget = new QShareTablewidget(this);
    ui->mainStackWidget->addWidget(mShareTableWidget);
    mDataMgrWidget = new QDataMgrWidget(this);
    ui->mainStackWidget->addWidget(mDataMgrWidget);
    mBlockTableWidget = new QBlockTableWidget(this);
    ui->mainStackWidget->addWidget(mBlockTableWidget);
    connect(ui->DataMgrBtn, SIGNAL(clicked()), this, SLOT(on_dataMgrBtn_clicked()));
    connect(ui->HqCenterButton, SIGNAL(clicked()), this, SLOT(slotHqCenterBtnClicked()));
    ui->mainStackWidget->addWidget(new QNorthFlowInfoDisplayWidget(this));
    mForeignVolTableWidget = new QShareForeignVolTableWidget(this);
    ui->mainStackWidget->addWidget(mForeignVolTableWidget);

    //指数显示
    QHBoxLayout *indexLayout = new QHBoxLayout;
    ui->indexframe->setLayout(indexLayout);
    indexLayout->setContentsMargins(5, 0, 5, 0);
    if(!mIndexWidget)
    {
        mIndexWidget = new QIndexWidget(this);
        ui->indexframe->layout()->addWidget(mIndexWidget);
    }
    this->setWindowFlags(Qt::FramelessWindowHint);
#ifdef Q_OS_WIN
    ui->tool_frame->setVisible(true);
    ui->min->setVisible(false);
    ui->max->setVisible(false);
#else
    ui->tool_frame->setVisible(false);
#endif
    this->setMouseTracking(true);
#ifdef Q_OS_WIN
    //系统托盘
    QIcon appIcon = QIcon(":/icon/image/Baidu_96px.png");
    if(appIcon.isNull())
    {
        qDebug()<<"icon image is not found";
    }
    this->setWindowIcon(appIcon);
    systemIcon = new QSystemTrayIcon(this);
    systemIcon->setIcon(appIcon);
    systemIcon->hide();
    connect(systemIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(setDlgShow(QSystemTrayIcon::ActivationReason)));
//    //创建快捷事件
    QShortcut *shotcut = new QShortcut(QKeySequence("Alt+X"), this);  //隐藏
    connect(shotcut, SIGNAL(activated()), this, SLOT(slotWhetherDisplay()));
//    QShortcut *shotcut1 = new QShortcut(QKeySequence("Alt+A"), this);
//    connect(shotcut1, SIGNAL(activated()), this, SLOT(slotDisplayAll()));
//    QShortcut *shotcut2 = new QShortcut(QKeySequence("Alt+S"), this);
//    connect(shotcut2, SIGNAL(activated()), this, SLOT(slotDisplayBlock()));
//    QShortcut *shotcut3 = new QShortcut(QKeySequence("Alt+D"), this);
//    connect(shotcut3, SIGNAL(activated()), this, SLOT(slotDisplayShareMini()));
    //    setHook(this);
#endif
    //
    mTaskMgr = new HQTaskMagrCenter;
    connect(mShareTableWidget, SIGNAL(signalSetFavCode(QString)), mTaskMgr, SIGNAL(signalSetFavCode(QString)));
    connect(mTaskMgr, SIGNAL(signalSendIndexCodesList(QStringList)), this, SLOT(slotRecvIndexCodesList(QStringList)));
    connect(mTaskMgr, SIGNAL(signalSendIndexRealDataList(ShareDataList)), mIndexWidget, SLOT(updateData(ShareDataList)));
    connect(mTaskMgr, SIGNAL(signalSendShareRealDataList(ShareDataList)), this, SLOT(updateHqTable(ShareDataList)));
    connect(mShareTableWidget, SIGNAL(signalSetDisplayHSHK(QString)), this, SLOT(slotUpdateHSGTOfCode(QString)));
    connect(mShareTableWidget, SIGNAL(signalSetShareMarket(int)), mTaskMgr, SLOT(setMktType(int)));
    connect(mShareTableWidget, SIGNAL(signalSetSortType(int)), mTaskMgr, SLOT(setSortType(int)));
    connect(mShareTableWidget, SIGNAL(signalDisplayPage(int)), mTaskMgr, SLOT(setDisplayPage(int)));
    connect(mShareTableWidget, SIGNAL(signalSetDisplayBlockDetail(QStringList)), mTaskMgr, SLOT(setSelfCodesList(QStringList)));
    connect(mBlockTableWidget, SIGNAL(signalDisplayBlockDetailCodesList(QStringList)), mTaskMgr, SLOT(setSelfCodesList(QStringList)));
    connect(mTaskMgr, SIGNAL(signalBlockDataListUpdated(BlockDataVList)), this, SLOT(updateBlockTable(BlockDataVList)));
    connect(mBlockTableWidget, SIGNAL(signalSetSortType(int)), mTaskMgr, SLOT(reverseSortRule()));
    connect(mBlockTableWidget, SIGNAL(signalSetBlockType(int)), mTaskMgr, SLOT(setCurBlockType(int)));
    connect(mTaskMgr, SIGNAL(signalUpdateHistoryMsg(QString)), this, SLOT(slotUpdateMsg(QString)));
    connect(mTaskMgr, SIGNAL(signalSendNotrhBoundDataList(ShareHsgtList)), mIndexWidget, SLOT(updateData(ShareHsgtList)));
    //
    connect(mTaskMgr, SIGNAL(signalWorkingDayfinished()), mForeignVolTableWidget, SLOT(slotStartInit()));
    mTaskMgr->signalStart();

    ui->mainStackWidget->setCurrentIndex(0);
}


void Dialog::slotRecvIndexCodesList(const QStringList &list)
{
    foreach (QString code, list) {
        mIndexWidget->insertWidget(code);
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
        close();
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
//    this->setVisible(false);
////    if(systemIcon) systemIcon->setVisible(true);
//    event->ignore();
}

void Dialog::on_srchBtn_clicked()
{
    //重新更新数据资料

}

void Dialog::slotUpdateMsg(const QString &msg)
{
    //ui->updatelbl->setText(msg);
}

void Dialog::on_minBtn_clicked()
{
    this->hide();
}

void Dialog::resizeEvent(QResizeEvent *event)
{
    if(event->size().width() > QApplication::desktop()->availableGeometry().size().width() ||
       event->size().height() > QApplication::desktop()->availableGeometry().size().height()     ) return;
    QDialog::resizeEvent(event);
    ui->searchTxt->setMaximumWidth(event->size().width()*0.4);
    ui->searchTxt->setMaximumWidth(event->size().width()*0.4);
}

void Dialog::updateHqTable(const ShareDataList& pDataList)
{
    mShareTableWidget->setDataList(pDataList);
}

void Dialog::updateBlockTable(const BlockDataList& pDataList)
{
    mBlockTableWidget->setDataList(pDataList);
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
    mShareTableWidget->setVisible(true);
    mBlockTableWidget->setVisible(true);
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

    mBlockTableWidget->setVisible(true);
    mShareTableWidget->setVisible(false);
    mDisplayCol = mBlockTableWidget->horizontalHeader()->count();
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
    mBlockTableWidget->setVisible(false);
    mShareTableWidget->setVisible(true);
    mDisplayCol = 0;
    for(int i=0; i<mShareTableWidget->columnCount(); i++)
    {
        if(!mShareTableWidget->isColumnHidden(i)) mDisplayCol++;
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
    //ui->HSGTBTN->setStyleSheet("background-color:red");
}

void Dialog::slotUpdateHSGTOfCode(const QString &code)
{
    qDebug()<<"history code:"<<code;
    QShareHistoryDialog *dlg = new QShareHistoryDialog(code) ;
    dlg->setModal(false);
    dlg->show();
}

void Dialog::on_dataMgrBtn_clicked()
{
    QAndroidListWidget *list = new QAndroidListWidget(this);
    list->addItem(QStringLiteral("陆股通"), DATA_MUTUAL_MARKET);
    list->addItem(QStringLiteral("龙虎榜"), DATA_LHB);
    list->addItem(QStringLiteral("北向实时"), DATA_NORTH_REAL);
    list->addItem(QStringLiteral("外资持股数据管理"), DATA_FOREIGN_CHECK);
    connect(list, SIGNAL(signalItemClicked(int)), this, SLOT(slotDisplayDataPage(int)));
    QPoint tp = ((QWidget*)ui->DataMgrBtn->parent())->mapToGlobal(ui->DataMgrBtn->geometry().topLeft());
    list->move(0, tp.y() - list->size().height());
    list->show();
}

void Dialog::slotDisplayDataPage(int val)
{
    QAndroidListWidget* widget = qobject_cast<QAndroidListWidget*>(sender());
    if(!widget) return;
    if(val == DATA_MUTUAL_MARKET)
    {
        ui->mainStackWidget->setCurrentWidget(mDataMgrWidget);
        mDataMgrWidget->setDataType(val);
        mDataMgrWidget->updateData();
    } else if(val == DATA_NORTH_REAL)
    {
        for(int i=0; i<ui->mainStackWidget->count(); i++)
        {
            QNorthFlowInfoDisplayWidget* w = qobject_cast<QNorthFlowInfoDisplayWidget*>(ui->mainStackWidget->widget(i));
            if(w)
            {
                ui->mainStackWidget->setCurrentWidget(w);
                break;
            }
        }
    } else if(val == DATA_FOREIGN_CHECK)
    {
        for(int i=0; i<ui->mainStackWidget->count(); i++)
        {
            QShareForeignVolTableWidget* w = qobject_cast<QShareForeignVolTableWidget*>(ui->mainStackWidget->widget(i));
            if(w)
            {
                ui->mainStackWidget->setCurrentWidget(w);
                break;
            }
        }
    }
    widget->hide();
    widget->deleteLater();

}

void Dialog::slotDisplayHqCenterPage(int val)
{
    QAndroidListWidget* widget = qobject_cast<QAndroidListWidget*>(sender());
    if(!widget) return;
    mShareTableWidget->resetDisplayRows();
    mTaskMgr->setMktType(val);
    widget->hide();
    widget->deleteLater();

}

void Dialog::slotHqCenterBtnClicked()
{
    if(ui->mainStackWidget->currentWidget() == mShareTableWidget)
    {
        QAndroidListWidget *list = new QAndroidListWidget(this);
        list->addItem(QStringLiteral("自选"), MKT_ZXG);
        list->addItem(QStringLiteral("沪深"), MKT_ALL);
        list->addItem(QStringLiteral("沪市"), MKT_SH);
        list->addItem(QStringLiteral("深市"), MKT_SZ);
        list->addItem(QStringLiteral("中小板"), MKT_ZXB);
        list->addItem(QStringLiteral("创业板"), MKT_CYB);
        list->addItem(QStringLiteral("科创板"), MKT_KCB);
        list->addItem(QStringLiteral("ETF"), MKT_JJ);
        list->addItem(QStringLiteral("陆股通TOP10"), MKT_LGT_TOP10);
        connect(list, SIGNAL(signalItemClicked(int)), this, SLOT(slotDisplayHqCenterPage(int)));
        QPoint tp = ((QWidget*)ui->HqCenterButton->parent())->mapToGlobal(ui->DataMgrBtn->geometry().topLeft());
        list->move(0, tp.y() - list->size().height());
        list->show();
    } else
    {
        //默认显示自选
        ui->mainStackWidget->setCurrentWidget(mShareTableWidget);
        slotDisplayHqCenterPage(MKT_ZXG);
    }

}

void Dialog::on_min_clicked()
{
    hide();
    if(systemIcon) systemIcon->setVisible(true);
}

void Dialog::on_max_clicked()
{
    if(isMaximized()) showNormal();
    else showMaximized();
}

void Dialog::on_close_clicked()
{
    hide();
    if(systemIcon) systemIcon->setVisible(true);
}
