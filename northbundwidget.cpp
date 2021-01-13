#include "northbundwidget.h"
#include "real/qnorthflowinfodisplaywidget.h"
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include "data_structure/hqutils.h"
#include "table/northboundtop10displaywidget.h"
#include "qandroidbutton.h"
#include "qandroidlistwidget.h"
#include <QVBoxLayout>
#include <QTimer>
#include "table/qshareforeignvoltablewidget.h"

NorthBundWidget::NorthBundWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vlay = new QVBoxLayout(this);
    this->setLayout(vlay);

    int frameHeight = HqUtils::convertMM2Pixel(10.0);
    mCtrlBtn = new QAndroidButton(this);
    mCtrlBtn->setAlignment(Qt::AlignCenter);
    mCtrlBtn->setFixedHeight(frameHeight);
    vlay->addWidget(mCtrlBtn);
    connect(mCtrlBtn, &QAndroidButton::clicked, this, &NorthBundWidget::slotMarketBtnClicked);

    mWidgetList = new QStackedWidget(this);
    vlay->addWidget(mWidgetList);
    int index = mWidgetList->addWidget(new QNorthFlowInfoDisplayWidget(this));
    mWidgetList->widget(index)->setFixedHeight(QApplication::desktop()->availableGeometry().height() *0.50);
    index = mWidgetList->addWidget(new NorthBoundTop10DisplayWidget(this));
    index = mWidgetList->addWidget(new LGTVolDisplayWidget(this));

    mTypeList = new QAndroidListWidget(0, 0, this);
    mTypeList->addItem(QStringLiteral("北向实时"), North_RealTime);
    mTypeList->addItem(QStringLiteral("北向成交"), North_Top10);
    mTypeList->addItem(QStringLiteral("北向持股"), North_Vol);

    connect(mTypeList, SIGNAL(signalItemClicked(QString,int)), this, SLOT(slotMarketTypeChanged(QString, int)));
    mTypeList->hide();

    QTimer::singleShot(100, mTypeList, SLOT(slotFirstBtnClicked()));

}

void NorthBundWidget::slotMarketBtnClicked()
{
    if(mTypeList)
    {
        mTypeList->setFixedWidth(this->width());
        mTypeList->move(mCtrlBtn->geometry().bottomLeft());
        mTypeList->setVisible(true);
    }
}

void NorthBundWidget::slotMarketTypeChanged(const QString& text, int type)
{
    if(mTypeList) mTypeList->setVisible(false);
    mCtrlBtn->setText(text);
    mWidgetList->setCurrentIndex(type);
}

