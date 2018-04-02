#include "qandroidlistwidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>

QAndroidListWidget::QAndroidListWidget(QWidget *parent) : QListWidget(parent)
{
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotItemClicked(QListWidgetItem*)));
}

void QAndroidListWidget::addItem(const QString &item)
{
    QListWidget::addItem(item);
    autoAdjustSize();
}

void QAndroidListWidget::autoAdjustSize()
{
    QSize screen = QApplication::desktop()->availableGeometry().size();
    double item_width = screen.width() * 1.0;
    double item_height = screen.height() / 15.0;
    for(int i=0; i<this->count(); i++)
    {
        this->item(i)->setSizeHint(QSize(item_width, item_height));
        this->item(i)->setTextAlignment(Qt::AlignCenter);
    }
    this->resize(item_width, item_height * count());
}

void QAndroidListWidget::slotItemClicked(QListWidgetItem *item)
{
    qDebug()<<this->count()<<item->text()<<this->size()<<item->sizeHint();
}

