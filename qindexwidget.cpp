#include "qindexwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>


QIndexWidget::QIndexWidget(QWidget *parent) : QStackedWidget(parent)
{
    this->setMinimumHeight(120);
    this->setMaximumHeight(120);
    mIndexWidgetMap.clear();
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setMargin(3);
    lay->setSpacing(3);
    this->setLayout(lay);
    mSwitchTimer = new QTimer(this);
    mSwitchTimer->setInterval(3000);
    connect(mSwitchTimer, SIGNAL(timeout()), this, SLOT(switchWidget()));
    mSwitchTimer->start();

}

QIndexWidget::~QIndexWidget()
{
    if(mSwitchTimer)
    {
        mSwitchTimer->stop();
        mSwitchTimer->deleteLater();
    }
}
void QIndexWidget::resizeEvent(QResizeEvent *e)
{
    QStackedWidget::resizeEvent(e);
}

void QIndexWidget::insetWidget(const QString &code)
{
    ShareDataList list;
    ShareData data;
    data.setCode(code.right(6));
    list<<data;
    updateData(list);
}

void QIndexWidget::updateData(const QList<NS_BOUND_DATA> &list)
{
#if 1
    if(list.length() != 2) return;
    QIndexFrame* w = NULL;
    if(mIndexWidgetMap.contains("BKLGT"))
    {
        w = mIndexWidgetMap["BKLGT"];
    } else
    {
        w = new QIndexFrame("", this);
        this->addWidget(w);
        mIndexWidgetMap["BKLGT"] = w;
    }
    w->updateBound(list[0].mPure, list[0].mName, list[1].mPure, list[1].mName);
 #else
    foreach (NS_BOUND_DATA data, list) {
        //qDebug()<<"data:"<<data.mCode<<" "<<data.mName<<" "<<data.mChg<<" "<<data.mChgPercent;
        QIndexFrame* w = NULL;
        if(mIndexWidgetMap.contains(data.mCode))
        {
            w = mIndexWidgetMap[data.mCode];
            w->setName(data.mName);
        } else
        {
            w = new QIndexFrame(data.mName, this);
            this->addWidget(w);
            mIndexWidgetMap[data.mCode] = w;
        }
        w->updateBound(data.mBuy, data.mSell, data.mPure, data.mTotal);
    }
#endif
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
            w = new QIndexFrame(data.mName, this);
            this->addWidget(w);
            mIndexWidgetMap[data.mCode] = w;
        }
        w->updateVal(data.mCur, data.mChg, data.mChgPercent, data.mMoney);
    }
}

void QIndexWidget::switchWidget()
{
    if(count() == 0) return;
    int index = this->currentIndex() + 1;
    setCurrentIndex(index % count());
}

