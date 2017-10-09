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

public slots:
    void    setDataList(const StockDataList& list);
    void    slotCustomContextMenuRequested(const QPoint &point);
private:
    QMap<QString, double>   mStockMap;

};

#endif // QSHARETABLEWIDGET_H
