#include "qsharegraphicwidget.h"
#include <QPainter>

#define         SYMBOL_WIDTH            100
#define         SYMBOL_HEIGHT           2
#define         SYMBOL_GAP              20

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

int QShareGraphicWidget::drawSymbol(QPainter *painters, const QString &text, const QColor &color, int x, int y, int s_width, int s_height)
{
    drawText(painters, QPoint(x, y), text, Qt::white, false);
    int width = painters->fontMetrics().width(text);
    int height = painters->fontMetrics().height();
    int center_x = x + width + 10 + 0.5 * s_width;
    int center_y = y/* + 0.5 * height*/;
    QRect symbol(0, 0, s_width, s_height);
    symbol.moveCenter(QPoint(center_x, center_y));
    painters->save();
    QPen pen = painters->pen();
    pen.setColor(color);
    painters->setPen(pen);
    painters->setBrush(color);
    painters->drawRect(symbol);
    painters->restore();

    return width + 10 + s_width + SYMBOL_GAP;
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

void QShareGraphicWidget::paintEvent(QPaintEvent *e)
{
    if(mData.size() == 0) return;
    //设定当前的区域
    QRect draw_rect = this->rect().marginsAdded(QMargins(-5, -5, -5, -5));
    QPainter painters(this);
    painters.fillRect(this->rect(), Qt::black);
    double max_price = mData.mMaxClose * 1.1;
    double height_per_price = draw_rect.height() / max_price;
    double max_foreign_vol = mData.mMaxForeignVol * 1.1;
    double height_per_vol = draw_rect.height() / max_foreign_vol;
    //计算左边价格的宽度, 右边数量的宽度
    double price_width = painters.fontMetrics().width(QString::number(max_price, 'f', 2));
    double vol_width = painters.fontMetrics().width(QString::number(max_foreign_vol*0.0001, 'f', 0));
    //设定曲线图例
    QColor price_color(Qt::red);
    QColor vol_color(Qt::green);
    double symbol_height = painters.fontMetrics().height();
    //开始画图例
    QString close_text = tr("收盘价(元)");
    QString vol_text = tr("外资持股量(万股)");
    int total_symbol_area_width = painters.fontMetrics().width(close_text) + painters.fontMetrics().width(vol_text) + SYMBOL_GAP + 2 * SYMBOL_WIDTH;
    int symbol_pos_y = 10;
    int symbol_pos_x = draw_rect.center().x() - 0.5 * total_symbol_area_width;
    symbol_pos_x += drawSymbol(&painters, close_text, price_color, symbol_pos_x, symbol_pos_y, SYMBOL_WIDTH, SYMBOL_HEIGHT);
    symbol_pos_x += drawSymbol(&painters, vol_text, vol_color, symbol_pos_x, symbol_pos_y, SYMBOL_WIDTH, SYMBOL_HEIGHT);
    //重新计算实际的网格区域
    draw_rect.setLeft(draw_rect.left() + price_width);
    draw_rect.setRight(draw_rect.right() - vol_width - 20);
    draw_rect.setTop(draw_rect.top() + 10 + symbol_height + 10);
    draw_rect.setBottom(draw_rect.bottom() - symbol_height - 10); //时间文本使用

    //默认100个数据，中间99间隔
    double unit_width = draw_rect.width() *1.0 / (100.0 -1);
    int size = mData.size();
    if(size > 100)
    {
        unit_width = draw_rect.width() * 1.0 / (size - 1);
    }
    //开始画坐标轴网格
    drawRect(&painters, draw_rect, 2, Qt::SolidLine, Qt::white, Qt::transparent);
    //开始画网格
    double min_x = draw_rect.topLeft().x();
    double max_x = draw_rect.bottomRight().x();
    double min_y = draw_rect.topLeft().y();
    double max_y = draw_rect.bottomRight().y();
    //开始画价格和网格
    for(int i=0; i<5; i++)
    {
        int pos_y = qRound(min_y + i * draw_rect.height() *1.0 / 5);
        if(i > 0 && i<5)
        {
            drawLine(&painters, QPoint(min_x,  pos_y), QPoint(max_x, pos_y), 1, Qt::white, Qt::DotLine);
        }
        QString text = QString::number(max_price * (5-i)*1.0/5, 'f', 2);
        drawText(&painters, QPoint(min_x, pos_y), text, Qt::white, true);
        text = QString::number(max_foreign_vol*0.0001 * (5-i)*1.0/5, 'f', 0);
        drawText(&painters, QPoint(max_x, pos_y), text, Qt::white, false);
    }
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
        if(i==size -1)
        {
            drawDate(&painters, QPoint(x, max_y), mData[i].mDate, Qt::white, 30);
        }
    }

    drawPath(&painters, price_color, price_path);
    drawPath(&painters, vol_color, vol_path);
}

void QShareGraphicWidget::drawText(QPainter *painters, const QPoint &pnt, const QString &text, const QColor &color, bool ref_left)
{
    painters->save();
    painters->setPen(color);
    int width = painters->fontMetrics().width(text);
    int height = painters->fontMetrics().height();
    QPoint pos = pnt;
    if(ref_left)
    {
        pos.setX(pos.x() - width - 2);
    } else
    {
        pos.setX(pos.x() + 10);
    }
    pos.setY(pos.y() + 0.5 * height);

    painters->drawText(pos, text);
    painters->restore();
}

void QShareGraphicWidget::drawPath(QPainter *painters, const QColor &color, const QPainterPath &path)
{
    painters->save();
    painters->setPen(QPen(color, 1));
    painters->drawPath(path);
    painters->restore();
}

void QShareGraphicWidget::drawDate(QPainter *painters, const QPoint &pnt, const QDate &text, const QColor &color, double rotate)
{
    painters->save();
    painters->translate(-pnt.x(), -pnt.y());
    painters->rotate(rotate);
    painters->translate(pnt.x(), pnt.y());
    painters->setPen(color);
    painters->drawText(pnt, text.toString("yyyyMMdd"));
    painters->restore();
}
