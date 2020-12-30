#include "qsharegraphicwidget.h"
#include <QPainter>
#include "sharehistoryreadthread.h"
#include <QWheelEvent>
#include <QPainterPath>

QShareGraphicWidget::QShareGraphicWidget(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<GRAPHIC_DATA>("const GRAPHIC_DATA&");
    qRegisterMetaType<GRAPHIC_DATA_LIST>("const GRAPHIC_DATA_LIST&");
    mDate = QDate::currentDate().addMonths(-1);
    qDebug()<<"init date:"<<mDate;
}
QShareGraphicWidget::QShareGraphicWidget(const QString &title, const QColor &, QWidget *parent)
{

}

void QShareGraphicWidget::wheelEvent(QWheelEvent *e)
{
    qDebug()<<"wheel history:";
    if(e->delta() > 0)
        mDate = mDate.addDays(10);
    else
        mDate = mDate.addDays(-10);
    updateGraphic();
}

void QShareGraphicWidget::updateGraphic()
{
    qDebug()<<"mDate:"<<mDate;
    ShareHistoryReadThread *thread = new ShareHistoryReadThread(mCode, mDate);
    connect(thread, SIGNAL(signalSendGraphicDataList(GRAPHIC_DATA_LIST)), this, SLOT(setValue(GRAPHIC_DATA_LIST)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void QShareGraphicWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

int QShareGraphicWidget::drawSymbol(QPainter *painters, const QString &text, const QColor& text_color, int x, int y, int s_width, int s_height, const QColor &symbol_color, int symbol_text_gap, int symbol_gap)
{
    int width = painters->fontMetrics().width(text);
    int height = painters->fontMetrics().height();
    int text_pos_y = y + 0.5 * height;

    painters->save();
    painters->setPen(text_color);
    painters->drawText(x, text_pos_y, text);
    painters->restore();
    int center_x = x + width + symbol_text_gap + 0.5 * s_width;
    int center_y = y;
    QRect symbol(0, 0, s_width, s_height);
    symbol.moveCenter(QPoint(center_x, center_y));
    painters->save();
    QPen pen = painters->pen();
    pen.setColor(symbol_color);
    painters->setPen(pen);
    painters->setBrush(symbol_color);
    painters->drawRect(symbol);
    painters->restore();

    return width + symbol_text_gap + s_width + symbol_gap;
}

void QShareGraphicWidget::drawLine(QPainter *painters, const QPoint &start, const QPoint &end, int width, const QColor &color, int type)
{
    painters->save();
    QPen pen = painters->pen();
    pen.setColor(color);
    pen.setWidth(width);
    pen.setStyle(Qt::PenStyle(type));
    painters->setPen(pen);
    painters->drawLine(start, end);
    painters->restore();
}

void QShareGraphicWidget::drawRect(QPainter *painters, const QRect &rect, int pen_width, int pen_type, const QColor &pen_color, const QColor &brush)
{
    painters->save();
    QPen pen = painters->pen();
    pen.setColor(pen_color);
    pen.setWidth(pen_width);
    pen.setStyle(Qt::PenStyle(pen_type));
    painters->setPen(pen);
    painters->setBrush(brush);
    painters->drawRect(rect);
    painters->restore();
}

void QShareGraphicWidget::drawText(QPainter *painters, const QString &text, const QColor &text_color, int x, int y, double rotate)
{
    painters->save();
    QPen pen = painters->pen();
    pen.setColor(text_color);
    painters->setPen(pen);
    if(rotate != 0)
    {
        painters->translate(x, y);
        painters->rotate(rotate);
        painters->translate(-x, -y);
    }
    painters->drawText(QPoint(x, y), text);
    painters->restore();
}

void QShareGraphicWidget::paintEvent(QPaintEvent *e)
{
    if(mData.size() == 0) return;
    //设定当前的区域
    int hor_gap = HqUtils::convertMM2Pixel(5);
    QRect draw_rect = this->rect().marginsAdded(QMargins(-hor_gap, -5, -hor_gap, -5));
    QPainter painters(this);
    painters.fillRect(this->rect(), Qt::transparent);

    //计算左边价格的宽度, 右边数量的宽度
    double max_price = mData.mMaxClose * 1.2;
    double max_foreign_vol = mData.mMaxForeignVol * 1.2;
    double price_width = painters.fontMetrics().width(QString::number(max_price, 'f', 2));
    double vol_width = painters.fontMetrics().width(QString::number(max_foreign_vol*0.0001, 'f', 0));
    //设定曲线图例
    QColor price_color(Qt::red);
    QColor vol_color(Qt::green);
    //开始画图例
    QString close_price_text = tr("收盘价");
    QString foreign_vol_text = tr("外资持股量(万股)");
    int     symbol_width = HqUtils::convertMM2Pixel(20);
    int     symbol_height = HqUtils::convertMM2Pixel(1);
    int     text_symbol_gap =  HqUtils::convertMM2Pixel(5);
    int     symbol_symbol_gap = HqUtils::convertMM2Pixel(10);
    int     total_symbol_width = painters.fontMetrics().width(close_price_text);
    total_symbol_width += painters.fontMetrics().width(foreign_vol_text);
    total_symbol_width += text_symbol_gap * 2;
    total_symbol_width += symbol_symbol_gap;
    total_symbol_width += symbol_width * 2;

    int symbol_pos_y = 10;
    int symbol_pos_x = draw_rect.width() * 0.5 - 0.5 * total_symbol_width;
    symbol_pos_x += drawSymbol(&painters, close_price_text, Qt::white, symbol_pos_x, symbol_pos_y, symbol_width, symbol_height, price_color, text_symbol_gap, symbol_symbol_gap);
    symbol_pos_x += drawSymbol(&painters, foreign_vol_text, Qt::white, symbol_pos_x, symbol_pos_y, symbol_width, symbol_height, vol_color, text_symbol_gap, symbol_symbol_gap);
    //重新计算实际的网格区域
    symbol_height = qMax(symbol_height, painters.fontMetrics().height());
    draw_rect.setLeft(draw_rect.left() + price_width);
    draw_rect.setRight(draw_rect.right() - vol_width);
    draw_rect.setTop(draw_rect.top() + 10 + symbol_height + 10);
    draw_rect.setBottom(draw_rect.bottom() - symbol_height - 10); //时间文本使用
    int date_text_y = draw_rect.bottom() + 10;

    //默认100个数据，中间99间隔
    double unit_width = draw_rect.width() *1.0 / (100.0 -1);
    int size = mData.size();
//    if(size > 100)
    {
        unit_width = draw_rect.width() * 1.0 /
                size;
    }
    //开始画坐标轴网格
    drawRect(&painters, draw_rect, 2, Qt::SolidLine, Qt::white, Qt::transparent);
    //开始画网格    
    double height_per_price = draw_rect.height() / max_price;
    double height_per_vol = draw_rect.height() / max_foreign_vol;

    double min_x = draw_rect.topLeft().x();
    double max_x = draw_rect.bottomRight().x();
    double min_y = draw_rect.topLeft().y();
    double max_y = draw_rect.bottomRight().y();
    //开始画价格和网格
    painters.save();
    for(int i=0; i<5; i++)
    {
        //网格的坐标点
        if( i > 0 && i <= 4)
        {
            QPointF start(min_x, min_y + i * draw_rect.height() / 5.0);
            QPointF end(max_x, min_y + i * draw_rect.height() / 5.0);
            this->drawLine(&painters, start.toPoint(), end.toPoint(), 1, Qt::white, Qt::DotLine);
        }
        painters.setPen(Qt::white);
        QString text = QString::number(max_price * (5-i)*1.0/5, 'f', 2);
        double width = painters.fontMetrics().width(text);
        double height = painters.fontMetrics().height();
        painters.drawText(QPointF(min_x - width - 5, min_y + i * draw_rect.height() *1.0 / 5 + 0.5*height), text);
        text = QString::number(max_foreign_vol*0.0001 * (5-i)*1.0/5, 'f', 0);
        width = painters.fontMetrics().width(text);
        painters.drawText(QPointF(max_x + 5, min_y + i * draw_rect.height() *1.0 / 5 + 0.5*height), text);
    }
    painters.restore();
    QPainterPath price_path;
    QPainterPath vol_path;
    double start_x = min_x;
//    if(size < 100)
//    {
//        start_x = min_x + (100-size) * unit_width;
//    }
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
            //开始画柱状图形
            double  subVol = mData[i].mForVol - mData[i-1].mForVol;
            QRectF rect(0, 0, unit_width, qAbs(subVol * height_per_vol) );
            rect.moveBottomRight(QPointF(x+ 0.5*unit_width, max_y));
            {
                painters.save();
                Qt::GlobalColor color = (subVol >= 0? Qt::red : Qt::green);
                painters.setPen(color);
                painters.setBrush(color);
                painters.drawRect(rect);
                painters.restore();
            }
        }

        if(i==0 ||i==size-1)
        {
//            qDebug()<<"price:"<<mData[i].mClose;
            drawText(&painters, mData[i].mDate.toString("yyyyMMdd"), Qt::white, x, date_text_y, 0);
        }
    }
    painters.save();
    QPen pen = painters.pen();
    pen.setColor(price_color);
    pen.setWidthF(HqUtils::convertMM2Pixel(0.5));
    painters.setPen(pen);
    painters.drawPath(price_path);
    pen.setColor(vol_color);
    painters.setPen(pen);
    painters.drawPath(vol_path);
    painters.restore();

}
