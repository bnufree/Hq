#include "qdatamgrwidget.h"
#include "ui_qdatamgrwidget.h"
#include "basic_info/qsharehsgttop10work.h"
#include <QThreadPool>
#include <QMessageBox>
#include "dbservices/dbservices.h"
#include "dbservices/qactivedate.h"

QDataMgrWidget::QDataMgrWidget(QWidget *parent) :
    QWidget(parent),
    mDataType(DATA_NONE),
    ui(new Ui::QDataMgrWidget)
{
    ui->setupUi(this);
    //connect(ui->mCurDayText, SIGNAL(windowIconTextChanged(QString))
    ui->mCurDayText->setText(QActiveDateTime::latestActiveDay().toString(DATE_FORMAT));
    connect(ui->mPreDayBtn, SIGNAL(clicked()), this, SLOT(slotDayChanged()));
    connect(ui->mNextDayBtn, SIGNAL(clicked()), this, SLOT(slotDayChanged()));
    ui->mNextDayBtn->setVisible(false);
    connect(DATA_SERVICE, SIGNAL(signalSendShareHsgtTop10List(ShareHsgtList)),
            this, SLOT(slotUpdateShareHsgtTop10List(ShareHsgtList)));
    connect(ui->mDataTableWidget, SIGNAL(signalDisplayMutualBundHistory(QString)),
            this, SLOT(slotDisplayMutualBundHistory(QString)));
    //updateData();
}

QDataMgrWidget::~QDataMgrWidget()
{
    delete ui;
}

void QDataMgrWidget::slotDayChanged()
{
    QAndroidButton *btn = qobject_cast<QAndroidButton*> (sender());
    qDebug()<<"btn:"<<btn;
    if(!btn) return;
    QDate curDate = QDate::fromString(ui->mCurDayText->text(), DATE_FORMAT);
    qDebug()<<"curDate:"<<curDate;
    if(btn == ui->mPreDayBtn)
    {
        curDate = QActiveDateTime(curDate).preActiveDay();
    } else
    {
        if(curDate != QActiveDateTime::latestActiveDay())
        {
            curDate = QActiveDateTime(curDate).nextActiveDay();
        }
    }
    if(curDate == QActiveDateTime::latestActiveDay())
    {
        ui->mNextDayBtn->setVisible(false);
    } else
    {
        ui->mNextDayBtn->setVisible(true);
    }
    qDebug()<<"new date:"<<curDate;
    ui->mCurDayText->setText(curDate.toString(DATE_FORMAT));
    updateData();
}

void QDataMgrWidget::setDislayDataType(int type)
{
    ui->mCurDayText->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    updateData();
}

void QDataMgrWidget::setDataType(int type)
{
    if(type != mDataType)
    {
        ui->mCurDayText->setText(QActiveDateTime::latestActiveDay().toString(DATE_FORMAT));
        ui->mNextDayBtn->setVisible(false);
    }
    ui->mCurDayText->setText(TradeDateMgr::instance()->currentTradeDay().toString(DATE_FORMAT));
    mDataType = type;
}

void QDataMgrWidget::updateData()
{
    if(mDataType == DATA_MUTUAL_MARKET)
    {
        ui->mDataTableWidget->setDataList(ShareHsgtList());
        emit DATA_SERVICE->signalQueryShareHsgtTop10List("", QDate::fromString(ui->mCurDayText->text(), DATE_FORMAT));
//        QShareHsgtTop10Work *job = new QShareHsgtTop10Work(ui->mCurDayText->text());
//        connect(job, SIGNAL(signalChinaAShareTop10Updated(ShareDataList,QString)), this, SLOT(slotUpdateShareHsgtTop10List(ShareDataList,QString)));
//        QThreadPool::globalInstance()->start(job);
    } else if(mDataType == DATA_LHB)
    {

    }

}

void QDataMgrWidget::slotUpdateShareHsgtTop10List(const ShareHsgtList &list)
{
    if(list.length() == 0)
    {
        QMessageBox::warning(0, QStringLiteral(""), QStringLiteral("数据未找到."));
    }
    ui->mDataTableWidget->setDataList(list);
}

void QDataMgrWidget::slotDisplayMutualBundHistory(const QString &code)
{
    ui->mDataTableWidget->setDataList(ShareHsgtList());
    emit DATA_SERVICE->signalQueryShareHsgtTop10List(code, QDate());
}
