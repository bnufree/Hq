#include "qindexwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include "utils/sharedata.h"


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

void QIndexWidget::insetWidget(const QString &code)
{
    ShareDataList list;
    ShareData data;
    data.setCode(code.right(6));
    list<<data;
    updateData(list);
}

void QIndexWidget::updateData(const ShareDataList &list)
{
    foreach (ShareData data, list) {
        //qDebug()<<"data:"<<data.mCode<<" "<<data.mName<<" "<<data.mChg<<" "<<data.mChgPercent;
        QIndexFrame* w = NULL;
        if(mIndexWidgetMap.contains(data.mCode))
        {
            w = mIndexWidgetMap[data.mCode];
            w->setName(data.mName);
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
        w->updateVal(data.mCur, data.mChg, data.mChgPercent, data.mMoney);
    }
}

