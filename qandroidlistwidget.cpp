#include "qandroidlistwidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QVBoxLayout>
#include "data_structure/hqutils.h"


#define     ITEM_PROPERTY               "UserData"

QAndroidListWidget::QAndroidListWidget(int w, int h, QWidget *parent) : QWidget(parent)
{
    mItemWidth = w;
    mItemHeight = h;
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    this->setLayout(layout);
    this->setStyleSheet("background-color:gray;QLabel{border:1px solid white;}");
}

void QAndroidListWidget::addItem(const QString &item, const QVariant& data)
{
    QAndroidButton *btn = new QAndroidButton(item, this);
    btn->setAlignment(Qt::AlignCenter);
    btn->setProperty(ITEM_PROPERTY, data);
    connect(btn, SIGNAL(clicked()), this, SLOT(slotItemClicked()));
    this->layout()->addWidget(btn);

    QSize screen = QApplication::desktop()->availableGeometry().size();
    if(mItemWidth == 0) mItemWidth = screen.width() * 1.0;
    if(mItemHeight == 0) mItemHeight = HqUtils::convertMM2Pixel(8);
    //btn->resize(item_width, );
    int count = this->layout()->count();
    this->resize(mItemWidth, mItemHeight * count);
    mBtnList.append(btn);
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
    emit signalItemClicked(btn->text(),  val);
}

void QAndroidListWidget::slotFirstBtnClicked()
{
    if(mBtnList.size() == 0) return;
    emit mBtnList.first()->clicked();
}

