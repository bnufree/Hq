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
    ui->mDataTypeBox->addItem(QStringLiteral("陆股通"), DATA_MUTUAL_MARKET);
    ui->mDataTypeBox->addItem(QStringLiteral("龙虎榜"), DATA_LHB);    
    ui->mCurDayText->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    ui->mDataTypeBox->setCurrentIndex(0);
    connect(ui->mDataTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDislayDataType(int)));
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

void QDataMgrWidget::updateData()
{
    QDate date = QDate::fromString(ui->mCurDayText->text(), "yyyy-MM-dd");
    int type = ui->mDataTypeBox->currentData().toInt();
    if(type == DATA_MUTUAL_MARKET)
    {
        QThreadPool pool;
        pool.setExpiryTimeout(-1);
        pool.setMaxThreadCount(16);
        pool.start(new QShareHsgtTop10Work(ui->mCurDayText->text(), this));
        pool.waitForDone();
    } else if(type == DATA_LHB)
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
