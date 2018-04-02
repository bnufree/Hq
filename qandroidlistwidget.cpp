#include "qandroidlistwidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QVBoxLayout>

#define     ITEM_PROPERTY               "UserData"

QAndroidListWidget::QAndroidListWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    this->setLayout(layout);
    this->setStyleSheet("QLabel{border:1px solid black;}");
}

void QAndroidListWidget::addItem(const QString &item, const QVariant& data)
{
    QAndroidButton *btn = new QAndroidButton(item, this);
    btn->setAlignment(Qt::AlignCenter);
    btn->setProperty(ITEM_PROPERTY, data);
    connect(btn, SIGNAL(clicked()), this, SLOT(slotItemClicked()));
    this->layout()->addWidget(btn);

    QSize screen = QApplication::desktop()->availableGeometry().size();
    double item_width = screen.width() * 1.0;
    double item_height = screen.height() / 15.0;
    //btn->resize(item_width, );
    int count = this->layout()->count();
    this->resize(item_width, item_height * count);
}

void QAndroidListWidget::autoAdjustSize()
{
//    QSize screen = QApplication::desktop()->availableGeometry().size();
//    double item_width = screen.width() * 1.0;
//    double item_height = screen.height() / 15.0;
//    for(int i=0; i<this->layout()->count(); i++)
//    {
//        this->item(i)->setSizeHint(QSize(item_width, item_height));
//        this->item(i)->setTextAlignment(Qt::AlignCenter);
//    }
//    this->resize(item_width, item_height * count());
}

void QAndroidListWidget::slotItemClicked()
{
    QAndroidButton *btn = qobject_cast<QAndroidButton*>(sender());
    int val = 0;
    if(btn) val = btn->property(ITEM_PROPERTY).toInt();
    emit signalItemClicked(val);
}

