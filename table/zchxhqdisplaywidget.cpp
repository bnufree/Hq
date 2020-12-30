#include "zchxhqdisplaywidget.h"
#include "ui_zchxhqdisplaywidget.h"
#include <QStackedWidget>
#include "qindexwidget.h"
#include "qandroidlistwidget.h"

zchxHqDisplayWidget::zchxHqDisplayWidget(QWidget *parent) :
    QWidget(parent),
    mMarketBtn(0),
    mMarketList(0),
    ui(new Ui::zchxHqDisplayWidget)
{
    ui->setupUi(this);
    int frameHeight = HqUtils::convertMM2Pixel(10.0);
//    ui->top_frame->setFixedHeight(frameHeight);
    mMarketBtn = new QAndroidButton(this);
    mMarketBtn->setAlignment(Qt::AlignCenter);
    mMarketBtn->setFixedWidth(HqUtils::convertMM2Pixel(15.0));
    ui->horizontalLayout->addWidget(mMarketBtn);
    ui->horizontalLayout->addWidget(new QIndexWidget(this));
    ui->verticalLayout->addWidget(mShareWidget = new QShareTablewidget(this));

    int item_width = HqUtils::convertMM2Pixel(15.0);
    int item_height = HqUtils::convertMM2Pixel(6.0);
    mMarketList = new QAndroidListWidget(item_width, item_height, this);

    mMarketList->addItem(QStringLiteral("自选"), MKT_ZXG);
    mMarketList->addItem(QStringLiteral("沪深"), MKT_ALL);
    mMarketList->addItem(QStringLiteral("沪市"), MKT_SH);
    mMarketList->addItem(QStringLiteral("深市"), MKT_SZ);
    mMarketList->addItem(QStringLiteral("中小板"), MKT_ZXB);
    mMarketList->addItem(QStringLiteral("创业板"), MKT_CYB);
    mMarketList->addItem(QStringLiteral("科创板"), MKT_KCB);
    mMarketList->addItem(QStringLiteral("沪深基金"), MKT_JJ);
    mMarketList->addItem(QStringLiteral("恒指"), MKT_HK_HSZS);
    mMarketList->addItem(QStringLiteral("恒生国企"), MKT_HK_HSGQ);
    mMarketList->addItem(QStringLiteral("港股通"), MKT_HK_GGT);
    mMarketList->addItem(QStringLiteral("陆股通TOP10"), MKT_LGT_TOP10);

    connect(mMarketList, SIGNAL(signalItemClicked(QString,int)), this, SLOT(slotMarketTypeChanged(QString, int)));
    mMarketList->hide();

    //设定初始状态
    mMarketBtn->setText(QStringLiteral("自选"));
    mMarketBtn->setSelected(true);
    mShareWidget->setShareMarketType(MKT_ZXG);

    connect(mMarketBtn, SIGNAL(clicked()), this, SLOT(slotMarketBtnClicked()));

}

void zchxHqDisplayWidget::slotMarketBtnClicked()
{
    if(mMarketList)
    {
        mMarketList->move(mMarketBtn->geometry().bottomLeft());
        mMarketList->setVisible(true);
    }
}

void zchxHqDisplayWidget::slotMarketTypeChanged(const QString& text, int type)
{
    if(mMarketList) mMarketList->setVisible(false);
    mMarketBtn->setText(text);
    mShareWidget->setShareMarketType(type);
}

zchxHqDisplayWidget::~zchxHqDisplayWidget()
{
    delete ui;
}
