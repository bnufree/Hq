#ifndef QSHARETABLEWIDGET_H
#define QSHARETABLEWIDGET_H

#include "HqTableWidget.h"
#include "stockdata.h"

class QShareTablewidget : public HqTableWidget
{
    Q_OBJECT
public:
    explicit QShareTablewidget(QWidget *parent = 0);
private:
    void    initMenu();

signals:
    void    signalSetStockMarket(int mkt);
    void    signalSetDisplayMinuteGraph(const QString& code);
    void    signalSetDisplayDayGraph(const QString& code);
    void    signalSetDisplayBlockDetail(const QStringList& blockCodes);
    void    signalSetDisplayHSHK(const QString& code);

public slots:
    void    setDataList(const StockDataList& list);
    void    slotCustomContextMenuRequested(const QPoint &pos);
    void    setStockMarket();
    void    setDisplayMinuteGraph();
    void    setDisplayDayGraph();
    void    setDisplayBlockDetail();
    void    setDisplayHSHK();
    void    slotCellDoubleClicked(int row, int col);
private:
    QMap<QString, double>   mStockMap;
    QList<QAction*>         mCodesActionList;
};

#endif // QSHARETABLEWIDGET_H
