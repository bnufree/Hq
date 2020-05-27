#include "northboundmoneyflowtablewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include "data_structure/hqutils.h"

NorthBoundMoneyWidget::NorthBoundMoneyWidget(QWidget *parent) : QWidget(parent)
{
    this->setLayout(new QVBoxLayout);
}

void NorthBoundMoneyWidget::appendWidget(NorthBoundMoneyFlowTableWidget *w)
{
    this->layout()->addWidget(w);
}


NorthBoundMoneyFlowTableWidget::NorthBoundMoneyFlowTableWidget(QWidget* child, QWidget *parent) : QWidget(parent)
{
    this->setLayout(new QVBoxLayout);
    QFrame *titleFrame = new QFrame(this);
    titleFrame->setFixedHeight(HqUtils::convertMM2Pixel(8.0));
    this->layout()->addWidget(titleFrame);
    titleFrame->setLayout(new QHBoxLayout);
    mTimeLabel = new QLabel(titleFrame);
    mDetailBtn = new QPushButton(QStringLiteral("详细"), titleFrame);
    mDetailBtn->setFixedWidth(HqUtils::convertMM2Pixel(20.0));
    connect(mDetailBtn, SIGNAL(clicked(bool)), this, SLOT(slotChangeDisplayMode()));
    titleFrame->layout()->addWidget(mTimeLabel);
    titleFrame->layout()->addWidget(mDetailBtn);
    mDisplayWidget = child;
    this->layout()->addWidget(mDisplayWidget);
    setWorkMode(false);
}

void NorthBoundMoneyFlowTableWidget::setChangeModeAvailable(bool sts)
{
    mDetailBtn->setVisible(sts);
}

void NorthBoundMoneyFlowTableWidget::setWorkMode(bool mode)
{
    mWorkDetail = mode;
    if(mWorkDetail == false)
    {
        mDetailBtn->setText(QStringLiteral("详细"));
    } else
    {
        mDetailBtn->setText(QStringLiteral("简要"));
    }
    //开始更新数据
    updateData();
}

void NorthBoundMoneyFlowTableWidget::slotChangeDisplayMode()
{
    setWorkMode(!mWorkDetail);
}
