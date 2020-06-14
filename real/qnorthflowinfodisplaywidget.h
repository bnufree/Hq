#ifndef QNORTHFLOWINFODISPLAYWIDGET_H
#define QNORTHFLOWINFODISPLAYWIDGET_H

#include <QWidget>
#include "qsinanorthrealinfothread.h"

namespace Ui {
class QNorthFlowInfoDisplayWidget;
}

class NorthFlowCurveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NorthFlowCurveWidget(QWidget* parent = 0);
    void setLineColor(const QColor& sh, const QColor& sz, const QColor& total);
    void setPathWidth(int width);
protected:
    void paintEvent(QPaintEvent *e);
public slots:
    void setNorthRealInfo(const QList<NorthBoundData>& list, int mMax, int mMin);
public:
    QColor mSH;
    QColor mSZ;
    QColor mTotal;
    QList<NorthBoundData> mDataList;
    int     mMax;
    int     mMin;
    int     mPathWidth;
};

class QNorthFlowInfoDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QNorthFlowInfoDisplayWidget(QWidget *parent = 0);
    ~QNorthFlowInfoDisplayWidget();
    void  setDate(const QString& date);

public slots:
    void setNorthRealInfo(const QList<NorthBoundData>& list, int mMax, int mMin, const QDate& date);

private:
    Ui::QNorthFlowInfoDisplayWidget *ui;
    QSinaNorthRealInfoThread * mRealInfoThread;
    NorthFlowCurveWidget* mDisplayWidget;
};

#endif // QNORTHFLOWINFODISPLAYWIDGET_H
