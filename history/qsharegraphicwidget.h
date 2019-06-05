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
    inline GRAPHIC_DATA_LIST()
    {
        mMaxClose = 0.0;
        mMaxRzye = 0.0;
        mMaxZjlx = 0.0;
        mMaxForeignVol = 0.0;
        mMaxMoney = 0.0;
        clear();
    }

    inline GRAPHIC_DATA_LIST(const ShareDataList& list)
    {
        foreach (ShareData data, list) {
            GRAPHIC_DATA graph;
            graph.mDate = data.mTime.date();
            graph.mClose = data.mClose;
            graph.mForVol = data.mHsgtData.mVolTotal;
            graph.mMoney = data.mMoney;
            graph.mRzye = data.mRZRQ;
            graph.mZjlx = data.mZJLX;
            append(graph);
        }
        for(int i=1; i<size(); i++)
        {
            if(this->at(i).mForVol == 0 && this->at(i-1).mForVol != 0)
            {
                (*this)[i].mForVol = (*this)[i-1].mForVol;
            }
        }
    }

    void append(const GRAPHIC_DATA& data)
    {
        QList<GRAPHIC_DATA>::append(data);
        if(data.mClose > mMaxClose)
        {
            mMaxClose = data.mClose;
        }
        if(data.mForVol > mMaxForeignVol)
        {
            mMaxForeignVol = data.mForVol;
        }
        if(data.mRzye> mMaxRzye)
        {
            mMaxRzye = data.mRzye;
        }
        if(data.mZjlx > mMaxZjlx)
        {
            mMaxZjlx = data.mZjlx;
        }
        if(data.mMoney > mMaxMoney)
        {
            mMaxMoney = data.mMoney;
        }
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
    void    setValue(const GRAPHIC_DATA_LIST& vals) {mData = vals; update();}
protected:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent * e);

signals:

public slots:
private:
    QString             mTitle;
    QColor              mColor;
    int                 mType;
    GRAPHIC_DATA_LIST   mData;
};

#endif // QSHAREGRAPHICWIDGET_H
