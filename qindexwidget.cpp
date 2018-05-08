#include "qindexwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>


QIndexWidget::QIndexWidget(QWidget *parent) : QStackedWidget(parent)
{
//    this->setMinimumHeight(120);
//    this->setMaximumHeight(120);
    mIndexWidgetMap.clear();
//    if(layout() == 0)
//    {
//        QVBoxLayout *lay = new QVBoxLayout(this);
//        this->setLayout(lay);
//    }
//    QLayout *lay = layout();
//    lay->setMargin(30);
//    lay->setSpacing(3);
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
    qDebug()<<"QIndexWidget"<<__func__<<e->size();
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
    qDebug()<<list.length();
    if(list.length() != 2) return;
    QWidget* w = NULL;
    if(mIndexWidgetMap.contains("BKLGT"))
    {
        w = mIndexWidgetMap["BKLGT"];
    } else
    {
        w = new QWidget(this);
        w->setLayout(new QHBoxLayout(this));
        this->addWidget(w);
        mIndexWidgetMap["BKLGT"] = w;
        w->layout()->addWidget(new QIndexFrame(list[0].mName, this));
        w->layout()->addWidget(new QIndexFrame(list[1].mName, this));
    }
    qDebug()<<list.length()<<w->layout()->count();
    if(list.length() != w->layout()->count()) return;
    for(int i=0; i<w->layout()->count(); i++)
    {
        qDebug()<<i+1<<list[i].mPure<<list[i].mName;
        QIndexFrame* frame = static_cast<QIndexFrame*>(w->layout()->itemAt(i)->widget());
        frame->updateBound(list[i].mPure, list[i].mName);
    }
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
            w = (QIndexFrame*)(mIndexWidgetMap[data.mCode]);
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

