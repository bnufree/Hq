#include "qsharegraphicwidget.h"

QShareGraphicWidget::QShareGraphicWidget(QWidget *parent) : QWidget(parent)
{

}
QShareGraphicWidget::QShareGraphicWidget(const QString &title, const QColor &, QWidget *parent)
{

}

void QShareGraphicWidget::paintEvent(QPaintEvent *e)
{
    if(mData.size() == 0) return;
    //设定当前的区域


}
