#include "qindexwidget.h"
#include <QHBoxLayout>

QIndexWidget::QIndexWidget(QWidget *parent) : QWidget(parent)
{
    mIndexWidgetMap.clear();
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setMargin(0);
    this->setLayout(lay);

}

QIndexWidget::~QIndexWidget()
{

}

void QIndexWidget::updateData(const StockDataList &list)
{
    foreach (StockData data, list) {
        QIndexFrame* w = NULL;
        if(mIndexWidgetMap.contains(data.code))
        {
            w = mIndexWidgetMap[data.code];
        } else
        {
            w = new QIndexFrame(data.name, this);
            this->layout()->addWidget(w);
            mIndexWidgetMap[data.code] = w;
        }
        w->updateVal(data.cur, data.chg, data.per);
    }
}

