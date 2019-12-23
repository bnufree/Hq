#include "qindexwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include "qcontainerwidget.h"
#include <QThread>
#include "real/qhqindexthread.h"
#include "utils/profiles.h"

extern int frameWidth;

#if 0
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
    qDebug()<<"QIndexWidget"<<__func__<<e->size()<<QThread::currentThread();
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
    qDebug()<<"list:"<<list.count()<<QThread::currentThread();
    foreach (QWidget* w, list) {
        qDebug()<<"w:"<<w;
        insertWidget(w);
    }
}

void QIndexWidget::insertWidget(QWidget *w)
{
    QContainerWidget *target = 0;
    bool insert_new = true;
    qDebug()<<"stacked widget count:"<<count()<<QThread::currentThread();
    if(count())
    {
        target = qobject_cast<QContainerWidget*> (this->widget(count() - 1));
        if( target && target->appendWidget(w)) insert_new = false;
    }
    if(insert_new)
    {
        target = new QContainerWidget();
        target->resize(this->geometry().size());
        addWidget(target);        
        target->appendWidget(w);
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
            w = new QIndexFrame(data.mName);
            mIndexWidgetMap[data.mCode] = w;
            insertWidget(w);
        }
        w->updateBound(data.mPure, data.mName);
    }
#endif
}

void QIndexWidget::updateData(const ShareDataList &list)
{
    qDebug()<<"update index:"<<QThread::currentThread();
    foreach (ShareData data, list) {
        //qDebug()<<"data:"<<data.mCode<<" "<<data.mName<<" "<<data.mChg<<" "<<data.mChgPercent;
        QIndexFrame* w = NULL;
        if(mIndexWidgetMap.contains(data.mCode))
        {
            w = (QIndexFrame*)(mIndexWidgetMap[data.mCode]);
            w->setName(data.mName);
        } else
        {
            w = new QIndexFrame(data.mName);
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

#else

QIndexWidget::QIndexWidget(QWidget *parent) : QWidget(parent), mSwitchTimer(0)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    this->setLayout(layout);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->setSpacing(6);
    mMaxDisplayFrameCount = 1;
    //开始实时指数更新
    mIndexThread = new QHqIndexThread(this);
//    connect(thread, SIGNAL(signalSendIndexDataList(ShareDataList, qint64)), this, SLOT(updateData(ShareDataList, qint64)));
    mIndexThread->start();


    mSwitchTimer = new QTimer(this);
    mSwitchTimer->setInterval(2000);
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
    QWidget::resizeEvent(e);
    //设定当前可以显示的index frame的数量
    if(frameWidth > 0)
    {
        mMaxDisplayFrameCount = e->size().width() / frameWidth;
    }
    if(mMaxDisplayFrameCount < 1) mMaxDisplayFrameCount = 1;

    qDebug()<<"start delete widget:"<<frameWidth;
    while (this->layout()->count() > 0)
    {
        QLayoutItem *item = this->layout()->takeAt(0);
        qDebug()<<"remove item:"<<item;
        if(item)
        {
            QWidget* w = item->widget();
            if(w)
            {
                w->setParent(0);
                delete w;

            }
            delete item;
        }
    }

    qDebug()<<"end delete widget:";

    while (this->layout()->count() < mMaxDisplayFrameCount) {
        this->layout()->addWidget(new QIndexFrame(""));
    }
    qDebug()<<"add widget end";
}



void QIndexWidget::insertWidget(const QString &code)
{
    ShareDataList list;
    ShareData data;
    data.mCode = code.right(6);
    list<<data;
    updateData(list, QDateTime::currentMSecsSinceEpoch());
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
    foreach (ShareHsgt temp, list) {
        //qDebug()<<"data:"<<data.mCode<<" "<<data.mName<<" "<<data.mChg<<" "<<data.mChgPercent;
        IndexFrameData data;
        data.mCode = temp.mCode;
        data.mCur = temp.mPure;
        data.mName = temp.mName;
        data.mTotal = temp.mTotal;
        data.mType = 0;
        int index = mDataList.indexOf(data);
        if(index < 0)
        {
            mDataList.append(data);
        } else
        {
            mDataList[index] = data;
        }
    }
#endif
}

void QIndexWidget::updateData(const ShareDataList &list, qint64 time)
{
//    qDebug()<<"update index:"<<QDateTime::fromMSecsSinceEpoch(time).toString("hh:mm:ss")<<QDateTime::currentDateTime().toString("hh:mm:ss");
    QTime t;
    t.start();
    foreach (ShareData temp, list) {
        IndexFrameData data;
        data.mCode = temp.mCode;
        data.mCur = temp.mCur;
        data.mChg = temp.mChg;
        data.mChgPer = temp.mChgPercent;
        data.mName = temp.mName;
        data.mTotal = temp.mMoney;
        data.mType = 1;
        int index = mDataList.indexOf(data);
        if(index < 0)
        {
            mDataList.append(data);
        } else
        {
            mDataList[index] = data;
        }
    }
}

void QIndexWidget::switchWidget()
{

    QTime t;
    t.start();
    static int row = 0;
    updateData(mIndexThread->getDataList(), QDateTime::currentMSecsSinceEpoch());

    for(int i = 0; i<this->layout()->count(); i++)
    {
        QIndexFrame* frame = qobject_cast<QIndexFrame*>(this->layout()->itemAt(i)->widget());
        if(frame && row < mDataList.size())
        {
            IndexFrameData data = mDataList[row];
            frame->updateData(data);
            row++;
            row = row % (mDataList.size());
        }
    }
}

#endif

