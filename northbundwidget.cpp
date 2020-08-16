#include "northbundwidget.h"
#include "real/qnorthflowinfodisplaywidget.h"
#include <QHeaderView>

NorthBundWidget::NorthBundWidget(QWidget *parent) : QListWidget(parent)
{
//    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    //添加实时外资流向数据
    this->insertItem(0, QStringLiteral("北向资金实时数据"));
    this->insertItem(1, new QListWidgetItem);
    this->setItemWidget(item(1), new QNorthFlowInfoDisplayWidget(this));
    item(1)->setSizeHint(QSize(width(), 600));
}

