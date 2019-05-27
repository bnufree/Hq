#include "qindexwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include "qcontainerwidget.h"


QIndexWidget::QIndexWidget(QWidget *parent) : QStackedWidget(parent)
{
    mIndexWidgetMap.clear();
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
    //检查indexMapWidget的数量,进行重新布局
    if(count() == 0) return;
    //检查widget里边包含的indexframe的数量
    QWidgetList list;
    while(count())
    {
        QContainerWidget *w = qobject_cast<QContainerWidget*> (this->widget(0));
        if(!w) continue;
        list.append(w->getWidgetList());
        this->removeWidget(w);
        delete w;
    }
    qDebug()<<"stacked widget count:"<<count();
    insertWidget(list);
}

void QIndexWidget::insertWidget(const QWidgetList &list)
{
    qDebug()<<"list:"<<list.count();
    foreach (QWidget* w, list) {
        qDebug()<<"w:"<<w;
        insertWidget(w);
    }
}

void QIndexWidget::insertWidget(QWidget *w)
{
    QContainerWidget *target = 0;
    bool insert_new = true;
    qDebug()<<"stacked widget count:"<<count();
    if(count())
    {
        target = qobject_cast<QContainerWidget*> (this->widget(count() - 1));
        if( target && target->appendWidget(w)) insert_new = false;
    }
    if(insert_new)
    {
        target = new QContainerWidget();
        target->resize(this->geometry().size());
        target->appendWidget(w);
        this->addWidget(target);
    }

}

void QIndexWidget::insertWidget(const QString &code)
{
    ShareDataList list;
    ShareData data;
    data.mCode = code.right(6);
    list<<data;
    updateData(list);
}

void QIndexWidget::updateData(const ShareHsgtList &list)
{
#if 0
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
    foreach (ShareHsgt data, list) {
        //qDebug()<<"data:"<<data.mCode<<" "<<data.mName<<" "<<data.mChg<<" "<<data.mChgPercent;
        QIndexFrame* w = NULL;
        if(mIndexWidgetMap.contains(data.mCode))
        {
            w = (QIndexFrame*)(mIndexWidgetMap[data.mCode]);
            w->setName(data.mName);
        } else
        {
            w = new QIndexFrame(data.mName, this);
            mIndexWidgetMap[data.mCode] = w;
            insertWidget(w);
        }
        w->updateBound(data.mPure, data.mName);
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
            mIndexWidgetMap[data.mCode] = w;
            insertWidget(w);
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

