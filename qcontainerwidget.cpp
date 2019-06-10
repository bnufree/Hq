#include "qcontainerwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

QContainerWidget::QContainerWidget(LAYOUT_DIRECTION dir, QWidget *parent) : QWidget(parent),mDir(dir)
{
    if(mDir == LAYOUT_HOR)
    {
        setLayout(new QHBoxLayout(this));
    } else
    {
        setLayout(new QVBoxLayout(this));
    }
    this->layout()->setMargin(0);
    this->layout()->setSpacing(0);
    this->layout()->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    this->layout()->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

bool QContainerWidget::appendWidget(QWidget *w)
{
    //计算是否有剩余空间来添加
    int width = this->geometry().width();
    int height = this->geometry().height();
    int count = this->layout()->count();
    int est_len = count > 0 ? count * this->layout()->spacing() : 0;

    for(int i=0; i<count; i++)
    {
        QWidget* o = this->layout()->itemAt(i)->widget();
        if(!o) continue;
        est_len += (mDir == LAYOUT_HOR ? o->width() : o->height());
    }

    est_len += (mDir == LAYOUT_HOR ? w->width() : w->height());
    bool insert = (mDir == LAYOUT_HOR ? est_len <= width : est_len <= height);
    if(insert)
    {
        QBoxLayout *box = qobject_cast<QBoxLayout*> (this->layout());
        if(box)
        {
            box->insertWidget(count-1, w);
            return true;
        }
    }
    return false;
}

bool QContainerWidget::removeWidget(QWidget *w)
{
    int index = this->layout()->indexOf(w);
    if(index >= 0) {
        this->layout()->removeWidget(w);
        return true;
    }
    return false;
}

QWidget* QContainerWidget::getLastWidget()
{
    QWidget* w = NULL;
    int count = this->layout()->count();
    if(count > 2)
    {
        w = this->layout()->itemAt(count - 2)->widget();
    }

    return w;
}

QWidget* QContainerWidget::getFirstWidget()
{
    QWidget* w = NULL;
    int count = this->layout()->count();
    if(count > 2)
    {
        w = this->layout()->itemAt(1)->widget();
    }

    return w;
}

QWidgetList QContainerWidget::getWidgetList()
{
    QWidgetList list;
    QWidget* w = NULL;
    int count = this->layout()->count();
    for(int i=1; i<count - 1; i++)
    {
        QLayoutItem *item = this->layout()->itemAt(i);
        if(!item) continue;
        w = item->widget();
        if(w)
        {
            w->setParent(0);
            list.append(w);
        }
    }

    return list;
}

void QContainerWidget::removeAllWidget()
{
    while (this->layout()->count() > 2) {
        this->layout()->removeItem(this->layout()->itemAt(1));
    }
}

int QContainerWidget::getWidgetCount()
{
    return this->layout()->count() - 2;
}

int QContainerWidget::totalWidgetSize() const
{
    int size = 0;
    int count = this->layout()->count();
    if(count > 2)
    {
        for(int i=1; i<count-1; i++)
        {
            QWidget* w = this->layout()->itemAt(i)->widget();
            if(!w) continue;
            size += (mDir == LAYOUT_HOR ? w->width() : w->height());
        }
    }

    return size;
}
