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
#include <QMessageBox>
#include "qsahreoptwidget.h"
#include <QHeaderView>

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
    QDialog(parent),mTaskMgr(0),mIndexWidget(0),mIsMini(false),
    ui(new Ui::MainDialog),systemIcon(0),mCtrlListWidget(0)
{
    qDebug()<<__func__<<__LINE__;
    ui->setupUi(this);
    ui->searchTxt->setMaximumWidth(this->fontMetrics().width(ui->searchTxt->placeholderText()) + 10);
    ui->HqCenterButton->setMaximumWidth(this->fontMetrics().width(ui->HqCenterButton->text()) + 10);
    ui->DataMgrBtn->setMaximumWidth(this->fontMetrics().width(ui->DataMgrBtn->text()) + 10);
//    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
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
//    mHqWidget->layout()->addWidget(mBlockTableWidget);
    ui->mainStackWidget->addWidget(mBlockTableWidget);
    connect(ui->DataMgrBtn, SIGNAL(clicked()), this, SLOT(on_dataMgrBtn_clicked()));
    connect(ui->HqCenterButton, SIGNAL(clicked()), this, SLOT(slotHqCenterBtnClicked()));
    ui->mainStackWidget->addWidget(new QNorthFlowInfoDisplayWidget(this));
    mForeignVolTableWidget = NULL;
//    mForeignVolTableWidget = new QShareForeignVolTableWidget(this);
//    ui->mainStackWidget->addWidget(mForeignVolTableWidget);

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
    connect(systemIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(setDlgShow(QSystemTrayIcon::ActivationReason)));
//    //创建快捷事件
//    QShortcut *shotcut = new QShortcut(QKeySequence("Alt+X"), this);  //隐藏
//    connect(shotcut, SIGNAL(activated()), this, SLOT(slotWhetherDisplay()));
//    QShortcut *shotcut1 = new QShortcut(QKeySequence("Alt+A"), this);
//    connect(shotcut1, SIGNAL(activated()), this, SLOT(slotDisplayAll()));
//    QShortcut *shotcut2 = new QShortcut(QKeySequence("Alt+S"), this);
//    connect(shotcut2, SIGNAL(activated()), this, SLOT(slotDisplayBlock()));
//    QShortcut *shotcut3 = new QShortcut(QKeySequence("Alt+D"), this);
//    connect(shotcut3, SIGNAL(activated()), this, SLOT(slotDisplayShareMini()));
    //    setHook(this);
#endif
#if 0
    //
    mTaskMgr = new HQTaskMagrCenter;
    connect(mShareTableWidget, SIGNAL(signalSetFavCode(QString)), mTaskMgr, SIGNAL(signalSetFavCode(QString)));
    connect(mTaskMgr, SIGNAL(signalSendIndexCodesList(QStringList)), this, SLOT(slotRecvIndexCodesList(QStringList)));
    connect(mTaskMgr, SIGNAL(signalSendIndexRealDataList(ShareDataList)), mIndexWidget, SLOT(updateData(ShareDataList)));
    connect(mTaskMgr, SIGNAL(signalSendShareRealDataList(ShareDataList)), this, SLOT(updateHqTable(ShareDataList)));
    connect(mShareTableWidget, SIGNAL(signalSetDisplayHSHK(QString)), this, SLOT(slotUpdateHSGTOfCode(QString)));
    connect(mShareTableWidget, SIGNAL(signalSetSpecialConcern(QString)), this, SLOT(slotSetSpecialConcern(QString)));
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
    connect(mShareTableWidget, SIGNAL(signalDoubleClickCode(QString)),
            this, SLOT(slotDoubClickedCode(QString)));
    //
    if(mForeignVolTableWidget)connect(mTaskMgr, SIGNAL(signalWorkingDayfinished()), mForeignVolTableWidget, SLOT(slotStartInit()));
    mTaskMgr->signalStart();
#endif

    ui->mainStackWidget->setCurrentIndex(0);
}

void Dialog::slotDoubClickedCode(const QString &code)
{
    QSahreOptWidget* widget = new QSahreOptWidget(code, this);
    QPoint pos = QCursor::pos();
    QPoint target = this->mapFromGlobal(pos);
    widget->move(target.x() - widget->width() / 2, target.y());
    widget->show();
}


void Dialog::slotSetSpecialConcern(const QString &code)
{
    if(mTaskMgr) mTaskMgr->addSpecialConcern(code);
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
    {
//        if(mIsMini) showMax();
//        else showMini();
        setVisible(!isVisible());
    }
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
        setVisible(true);
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
//    mShareTableWidget->setDataList(1, 1, pDataList);
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
//    if(mDisplayMode != E_DISPLAY_Share_MINI)
//    {
//        mDisplayMode = E_DISPLAY_Share_MINI;
//    }
//    mBlockTableWidget->setVisible(false);
//    mShareTableWidget->setVisible(true);
//    mDisplayCol = 0;
//    for(int i=0; i<mShareTableWidget->columnCount(); i++)
//    {
//        if(!mShareTableWidget->isColumnHidden(i)) mDisplayCol++;
//    }
//    mTargetSize.setWidth(mDisplayCol * mSecSize);
//    setTargetSize(mTargetSize);
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

void Dialog::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Backspace)
    {
        qDebug()<<"backspace!!!:";
        static int backspace_num = 0;
        backspace_num++;
        if(backspace_num == 2)
        {
            close();
            exit(0);
            return;
        }
        QMessageBox *box = new QMessageBox(QMessageBox::Information, tr("提示"), tr("再按一次退出程序"));
        box->setAttribute(Qt::WA_DeleteOnClose);
        box->removeButton(box->button(QMessageBox::Cancel));
        QTimer::singleShot(2000, box, SLOT(close()));
        box->show();
        event->ignore();
    }
    event->ignore();
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
    bool found = false;
    for(int i=0; i<ui->mainStackWidget->count(); i++)
    {
        QShareHistoryDialog* w = qobject_cast<QShareHistoryDialog*>(ui->mainStackWidget->widget(i));
        if(w)
        {
            ui->mainStackWidget->setCurrentWidget(w);
            found = true;
            break;
        }
    }
    if(!found)
    {
        QShareHistoryDialog *dlg = new QShareHistoryDialog(code) ;
        dlg->setModal(false);
        dlg->show();
    }

}

void Dialog::on_dataMgrBtn_clicked()
{
    if(mCtrlListWidget) delete mCtrlListWidget;
    mCtrlListWidget = new QAndroidListWidget(0, 0, this);
    mCtrlListWidget->addItem(QStringLiteral("陆股通"), DATA_MUTUAL_MARKET);
//    mCtrlListWidget->addItem(QStringLiteral("龙虎榜"), DATA_LHB);
    mCtrlListWidget->addItem(QStringLiteral("北向实时"), DATA_NORTH_REAL);
//    mCtrlListWidget->addItem(QStringLiteral("外资持股数据管理"), DATA_FOREIGN_CHECK);
    connect(mCtrlListWidget, SIGNAL(signalItemClicked(int)), this, SLOT(slotDisplayDataPage(int)));
    QPoint tp = ((QWidget*)ui->DataMgrBtn->parent())->mapToGlobal(ui->DataMgrBtn->geometry().topLeft());
    mCtrlListWidget->move(0, tp.y() - mCtrlListWidget->size().height());
    mCtrlListWidget->show();
}

void Dialog::slotDisplayDataPage(int val)
{
    QAndroidListWidget* widget = qobject_cast<QAndroidListWidget*>(sender());
    if(!widget || widget != mCtrlListWidget) return;

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
    mCtrlListWidget->hide();
    delete mCtrlListWidget;
    mCtrlListWidget = 0;

}

void Dialog::slotDisplayHqCenterPage(int val)
{
    QAndroidListWidget* widget = qobject_cast<QAndroidListWidget*>(sender());
    if(!widget || widget != mCtrlListWidget) return;
//    mShareTableWidget->resetDisplayRows();
    mCtrlListWidget->hide();
    delete mCtrlListWidget;
    mCtrlListWidget = 0;
}

void Dialog::slotHqCenterBtnClicked()
{
    if(ui->mainStackWidget->currentWidget() == mShareTableWidget)
    {
        if(mCtrlListWidget) delete mCtrlListWidget;
        mCtrlListWidget = new QAndroidListWidget(0 , 0 , this);
        mCtrlListWidget->addItem(QStringLiteral("自选"), MKT_ZXG);
        mCtrlListWidget->addItem(QStringLiteral("沪深"), MKT_ALL);
        mCtrlListWidget->addItem(QStringLiteral("沪市"), MKT_SH);
        mCtrlListWidget->addItem(QStringLiteral("深市"), MKT_SZ);
        mCtrlListWidget->addItem(QStringLiteral("中小板"), MKT_ZXB);
        mCtrlListWidget->addItem(QStringLiteral("创业板"), MKT_CYB);
        mCtrlListWidget->addItem(QStringLiteral("科创板"), MKT_KCB);
        mCtrlListWidget->addItem(QStringLiteral("ETF"), MKT_JJ);
        mCtrlListWidget->addItem(QStringLiteral("陆股通TOP10"), MKT_LGT_TOP10);
        connect(mCtrlListWidget, SIGNAL(signalItemClicked(int)), this, SLOT(slotDisplayHqCenterPage(int)));
        QPoint tp = ((QWidget*)ui->HqCenterButton->parent())->mapToGlobal(ui->DataMgrBtn->geometry().topLeft());
        mCtrlListWidget->move(0, tp.y() - mCtrlListWidget->size().height());
        mCtrlListWidget->show();
    } else
    {
        //默认显示自选
        ui->mainStackWidget->setCurrentWidget(mShareTableWidget);
        slotDisplayHqCenterPage(MKT_ZXG);
    }

}

void Dialog::on_min_clicked()
{
    showMini();
    ui->max->setVisible(true);
    ui->min->setVisible(false);
//    hide();
//    if(systemIcon) systemIcon->setVisible(true);
}

void Dialog::on_max_clicked()
{
    showMax();
    ui->max->setVisible(false);
    ui->min->setVisible(true);
}

void Dialog::on_close_clicked()
{
    hide();
    if(systemIcon) systemIcon->setVisible(true);
}

void Dialog::showMini()
{
    QSize size = QApplication::desktop()->availableGeometry().size();
    this->setFixedSize(200, size.height());
    this->move(size.width() - 200, 1);
    mIsMini = true;
//    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
}

void Dialog::showMax()
{
    this->setFixedSize(QApplication::desktop()->availableGeometry().size());
//    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    this->show();
    this->move(0, 0);
    mIsMini = false;

}
