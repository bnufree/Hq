#ifndef QSHAREGRAPHICWIDGET_H
#define QSHAREGRAPHICWIDGET_H

#include <QWidget>
#include <QDate>
#include "data_structure/sharedata.h"

enum ShareGraphicType
{
    GRAPHIC_HISTOGRAM = 0,
    GRAPHIC_PIECHART,
    GRAPHIC_LINECHART,
};



class QShareGraphicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QShareGraphicWidget(QWidget *parent = 0);
    explicit QShareGraphicWidget(const QString& title , const QColor& = Qt::red, QWidget* parent = 0);
    void    setTitle(const QString& title) {mTitle = title; update();}
    void    setColor(const QColor& color) {mColor = color;update();}
    void    setType(int type) {mType = type;update();}
public slots:
    void    setCode(const QString& code) {mCode = code; updateGraphic();}
public slots:
    void    setValue(const GRAPHIC_DATA_LIST& vals) {mData = vals; update();}
private:
    int    drawSymbol(QPainter *painters, const QString &text, const QColor& text_color, int x, int y, int s_width, int s_height, const QColor &symbol_color, int symbol_text_gap, int symbol_gap );  //返回图例实际的宽度
    void    drawLine(QPainter *painters, const QPoint &start, const QPoint &end, int width, const QColor &color, int type);
    void    drawRect(QPainter *painters, const QRect &rect, int pen_width, int pen_type, const QColor &pen_color, const QColor &brush);
    void    drawText(QPainter *painters, const QString &text, const QColor& text_color, int x, int y, double rotate);
protected:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent * e);
    void wheelEvent(QWheelEvent *e);
private:
    void    updateGraphic();

signals:

public slots:
private:
    QString             mTitle;
    QColor              mColor;
    int                 mType;
    GRAPHIC_DATA_LIST   mData;
    QString             mCode;
    QDate               mDate;
};

#endif // QSHAREGRAPHICWIDGET_H
