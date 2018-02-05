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
    double          mMoney;
};
class GRAPHIC_DATA_LIST : public QList<GRAPHIC_DATA>
{
public:
    GRAPHIC_DATA_LIST()
    {
        mMaxClose = 0.0;
        mMaxRzye = 0.0;
        mMaxZjlx = 0.0;
        mMaxForeignVol = 0.0;
        mMaxMoney = 0.0;
        clear();
    }

public:
    double      mMaxClose;
    double      mMaxRzye;
    double      mMaxZjlx;
    double      mMaxForeignVol;
    double      mMaxMoney;
};


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
