#ifndef QSHAREGRAPHICWIDGET_H
#define QSHAREGRAPHICWIDGET_H

#include <QWidget>
#include <QDate>

enum ShareGraphicType
{
    GRAPHIC_HISTOGRAM = 0,
    GRAPHIC_PIECHART,
    GRAPHIC_LINECHART,
};

struct  GRAPHIC_DATA{
    QDate           mDate;
    double          mClose;
    double          mRzye;
    double          mZjlx;
    double          mForVol;
    double          mForVolChg;
    double          mMoney;
    double          mVol;
};
typedef QList<GRAPHIC_DATA>         GRAPHIC_DATA_LIST;

class QShareGraphicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QShareGraphicWidget(QWidget *parent = 0);
    explicit QShareGraphicWidget(const QString& title = QString(), const QColor& = Qt::red, QWidget* parent = 0);
    void    setTitle(const QString& title) {mTitle = title; update();}
    void    setColor(const QColor& color) {mColor = color;update();}
    void    setType(int type) {mType = type;update();}
    //void    setValue(const QMap<QDate, QVariant>& vals) {mValues = vals; update();}
protected:
    void paintEvent(QPaintEvent *e);

signals:

public slots:
private:
    QString             mTitle;
    QColor              mColor;
    int                 mType;
    GRAPHIC_DATA_LIST   mData;
};

#endif // QSHAREGRAPHICWIDGET_H
