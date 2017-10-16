#ifndef QBLOCKTABLEWIDGET_H
#define QBLOCKTABLEWIDGET_H

#include "HqTableWidget.h"
#include "block/blockdata.h"

class QBlockTableWidget : public HqTableWidget
{
    Q_OBJECT
public:
    explicit QBlockTableWidget(QWidget *parent = 0);
private:
    void    initMenu();

signals:
    void    signalDisplayBlockDetailCodesList(const QStringList& codes);
    void    signalSetBlockType(int mkt);
public slots:
    void    setDataList(const BlockDataList& list);
    void    slotCustomContextMenuRequested(const QPoint &pos);
    void    setBlockType();
    void    slotCellDoubleClicked(int row, int col);
private:
    QMap<QString, double>   mStockMap;
};

#endif // QBLOCKTABLEWIDGET_H
