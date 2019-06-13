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
protected:
    void paintEvent(QPaintEvent *e);
public slots:
    void setNorthRealInfo(const QList<NorthBoundData>& list, int mMax);
public:
    QColor mSH;
    QColor mSZ;
    QColor mTotal;
    QList<NorthBoundData> mDataList;
    int     mMax;
};

class QNorthFlowInfoDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QNorthFlowInfoDisplayWidget(QWidget *parent = 0);
    ~QNorthFlowInfoDisplayWidget();

private:
    Ui::QNorthFlowInfoDisplayWidget *ui;
    QSinaNorthRealInfoThread * mRealInfoThread;
    NorthFlowCurveWidget* mDisplayWidget;
};

#endif // QNORTHFLOWINFODISPLAYWIDGET_H
