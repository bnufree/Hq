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
    QRect draw_rect = this->rect().marginsAdded(QMargins(-5, -5, -5, -5));
    {
//        double max_close = mData.mMaxClose * 1.1;
//        double close_sub_height = draw_rect.height() / max;
//        double max_foreign_vol = mData.mMaxForeignVol * 1.1;
//        double foreign_sub_height = draw_rect.height() / max_foreign_vol;


//        for(int i=0; i<mData.size(); i++)
//        {

//        }
    }


}
