#include "northbundwidget.h"
#include "real/qnorthflowinfodisplaywidget.h"
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include "data_structure/hqutils.h"
#include "table/northboundtop10displaywidget.h"

NorthBundWidget::NorthBundWidget(QWidget *parent) : QListWidget(parent)
{
//    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    //添加实时外资流向数据
    this->insertItem(0, QStringLiteral("北向资金实时数据"));
    item(0)->setTextAlignment(Qt::AlignCenter);
    item(0)->setSizeHint(QSize(width(), HqUtils::convertMM2Pixel(10)));
    QFont font = item(0)->font();
    font.setPixelSize(HqUtils::convertMM2Pixel(6));
    item(0)->setFont(font);
    this->insertItem(1, new QListWidgetItem);
    this->setItemWidget(item(1), new QNorthFlowInfoDisplayWidget(this));
    item(1)->setSizeHint(QSize(width(), QApplication::desktop()->availableGeometry().height() *0.50));

    this->insertItem(2, new QListWidgetItem);
    this->setItemWidget(item(2), new NorthBoundTop10DisplayWidget(this));
    item(2)->setSizeHint(QSize(width(), QApplication::desktop()->availableGeometry().height() *0.60));
}

