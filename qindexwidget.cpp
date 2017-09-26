#include "qindexwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

QIndexWidget::QIndexWidget(QWidget *parent) : QWidget(parent)
{
    mIndexWidgetMap.clear();
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setMargin(3);
    lay->setSpacing(3);
    this->setLayout(lay);

}

QIndexWidget::~QIndexWidget()
{

}

void QIndexWidget::updateData(const StockDataList &list)
{
    foreach (StockData data, list) {
        QIndexFrame* w = NULL;
        if(mIndexWidgetMap.contains(data.mCode))
        {
            w = mIndexWidgetMap[data.mCode];
        } else
        {
            QHBoxLayout *hlay = 0;
            if(mIndexWidgetMap.count() % 4 == 0)
            {
                hlay = new QHBoxLayout(this);
                ((QVBoxLayout*) this->layout())->addLayout(hlay);
            } else
            {
                QLayoutItem* item = ((QVBoxLayout*) this->layout())->itemAt(this->layout()->count()-1);
                if(item) hlay = (QHBoxLayout*)(item->layout());
            }
            w = new QIndexFrame(data.mName, this);
            //this->layout()->addWidget(w);
            if(hlay)   hlay->addWidget(w);
            mIndexWidgetMap[data.mCode] = w;
        }
        w->updateVal(data.mCur, data.mChg, data.mChgPercent);
    }
}

