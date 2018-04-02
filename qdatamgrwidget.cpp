#include "qdatamgrwidget.h"
#include "ui_qdatamgrwidget.h"
#include "basic_info/qsharehsgttop10work.h"
#include <QThreadPool>
#include <QMessageBox>

QDataMgrWidget::QDataMgrWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QDataMgrWidget)
{
    ui->setupUi(this);
    ui->mCurDayText->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    connect(ui->mPreDayBtn, SIGNAL(clicked()), this, SLOT(slotDayChanged()));
    connect(ui->mNextDayBtn, SIGNAL(clicked()), this, SLOT(slotDayChanged()));
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
    QDate curDate = QDate::fromString(ui->mCurDayText->text(), "yyyy-MM-dd");
    if(btn == ui->mPreDayBtn)
    {
        curDate = curDate.addDays(-1);
    } else
    {
        if(curDate == QDate::currentDate()) return;
        curDate = curDate.addDays(1);
    }
    ui->mCurDayText->setText(curDate.toString("yyyy-MM-dd"));
    updateData();
}

void QDataMgrWidget::setDislayDataType(int type)
{
    ui->mCurDayText->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    updateData();
}

void QDataMgrWidget::setDataType(int type)
{
    ui->mCurDayText->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    mDataType = type;
}

void QDataMgrWidget::updateData()
{
    if(mDataType == DATA_MUTUAL_MARKET)
    {
        QThreadPool pool;
        pool.setExpiryTimeout(-1);
        pool.setMaxThreadCount(16);
        pool.start(new QShareHsgtTop10Work(ui->mCurDayText->text(), this));
        pool.waitForDone();
    } else if(mDataType == DATA_LHB)
    {

    }

}

void QDataMgrWidget::slotUpdateShareHsgtTop10List(const ShareBaseDataList &list)
{
    if(list.length() == 0)
    {
        QMessageBox::warning(0, QStringLiteral(""), QStringLiteral("数据未找到."));
    }
    ui->mDataTableWidget->setDataList(list);
}
