#include "zchxhqdisplaywidget.h"
#include "ui_zchxhqdisplaywidget.h"
#include <QStackedWidget>

zchxHqDisplayWidget::zchxHqDisplayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::zchxHqDisplayWidget)
{
    ui->setupUi(this);
    ui->comboBox->setMinimumHeight(40);
    ui->verticalLayout->addWidget(mShareWidget = new QShareTablewidget(this));

    QList<struMenu> itemlist;
    itemlist.append(struMenu(QStringLiteral("自选"), MKT_ZXG));
    itemlist.append(struMenu(QStringLiteral("沪深"), MKT_ALL));
    itemlist.append(struMenu(QStringLiteral("沪市"), MKT_SH));
    itemlist.append(struMenu(QStringLiteral("深市"), MKT_SZ));
    itemlist.append(struMenu(QStringLiteral("中小板"), MKT_ZXB));
    itemlist.append(struMenu(QStringLiteral("创业板"), MKT_CYB));
    itemlist.append(struMenu(QStringLiteral("科创板"), MKT_KCB));
    itemlist.append(struMenu(QStringLiteral("沪深基金"), MKT_JJ));
    itemlist.append(struMenu(QStringLiteral("恒指"), MKT_HK_HSZS));
    itemlist.append(struMenu(QStringLiteral("恒生国企"), MKT_HK_HSGQ));
    itemlist.append(struMenu(QStringLiteral("港股通"), MKT_HK_GGT));
    itemlist.append(struMenu(QStringLiteral("陆股通TOP10"), MKT_LGT_TOP10));

    foreach (struMenu item, itemlist) {
        ui->comboBox->addItem(item.mDisplayText, item.mCmd);
    }
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotMarketTypeChanged(int)));
    ui->comboBox->setCurrentIndex(0);

}

void zchxHqDisplayWidget::slotMarketTypeChanged(int index)
{
    mShareWidget->setShareMarketType(ui->comboBox->currentData().toInt());
}

zchxHqDisplayWidget::~zchxHqDisplayWidget()
{
    delete ui;
}
