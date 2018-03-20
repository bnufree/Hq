#include "qsharegraphicwidget.h"
#include <QPainter>

QShareGraphicWidget::QShareGraphicWidget(QWidget *parent) : QWidget(parent)
{

}
QShareGraphicWidget::QShareGraphicWidget(const QString &title, const QColor &, QWidget *parent)
{

}

void QShareGraphicWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

void QShareGraphicWidget::paintEvent(QPaintEvent *e)
{
    if(mData.size() == 0) return;
    //设定当前的区域
    QRect draw_rect = this->rect().marginsAdded(QMargins(-5, -5, -5, -5));
    {
        QPainter painters(this);
        double max_price = mData.mMaxClose * 1.1;
        double height_per_price = draw_rect.height() / max_price;
        double max_foreign_vol = mData.mMaxForeignVol * 1.1;
        double height_per_vol = draw_rect.height() / max_foreign_vol;
        //默认100个数据，中间99间隔
        double unit_width = draw_rect.width() *1.0 / (100.0 -1);
        int size = mData.size();
        if(size > 100)
        {
            unit_width = draw_rect.width() * 1.0 / (size - 1);
        }
        //开始画坐标轴
        double min_x = draw_rect.topLeft().x();
        double max_x = draw_rect.bottomRight().x();
        double min_y = draw_rect.topLeft().y();
        double max_y = draw_rect.bottomRight().y();
        painters.save();
        QPen pen = painters.pen();
        pen.setColor(Qt::black);
        pen.setWidthF(2);
        painters.setPen(pen);
        painters.drawLine(QPointF(min_x, max_y), QPointF(min_x, min_y));
        painters.drawLine(QPointF(min_x, max_y), QPointF(max_x, max_y));
        //开始画价格
        for(int i=0; i<5; i++)
        {
            QString text = QString::number(max_price * (5-i)*1.0/5, 'f', 2);
            double width = painters.fontMetrics().width(text);
            double height = painters.fontMetrics().height();
            painters.drawText(QPointF(min_x+10, min_y + i * draw_rect.height() *1.0 / 5 +height), text);
            text = QString::number(max_foreign_vol*0.0001 * (5-i)*1.0/5, 'f', 2);
            width = painters.fontMetrics().width(text);
            painters.drawText(QPointF(max_x-width - 5, min_y + i * draw_rect.height() *1.0 / 5 + height), text);
        }
        painters.restore();
        QPainterPath price_path;
        QPainterPath vol_path;
        double start_x = min_x;
        if(size < 100)
        {
            start_x = min_x + (100-size) * unit_width;
        }
        for(int i=0; i<size; i++)
        {
            double x = start_x + unit_width * i;
            if(i==0)
            {
                price_path.moveTo(QPointF(x, max_y - mData[i].mClose * height_per_price));
                vol_path.moveTo(QPointF(x, max_y - mData[i].mForVol * height_per_vol));
            } else
            {
                price_path.lineTo(QPointF(x, max_y - mData[i].mClose * height_per_price));
                vol_path.lineTo(QPointF(x, max_y - mData[i].mForVol * height_per_vol));
            }
        }
        painters.save();
        pen = painters.pen();
        pen.setColor(Qt::red);
        pen.setWidthF(1);
        painters.setPen(pen);
        painters.drawPath(price_path);
        pen.setColor(Qt::green);
        painters.setPen(pen);
        painters.drawPath(vol_path);
        painters.restore();
    }


}
