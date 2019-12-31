#ifndef QBLOCKTABLEWIDGET_H
#define QBLOCKTABLEWIDGET_H

#include "HqTableWidget.h"
#include "data_structure/hqblockdata.h"

class QEastMoneyBlockMangagerThread;

class QBlockTableWidget : public HqTableWidget
{
    Q_OBJECT
public:
    explicit QBlockTableWidget(QWidget *parent = 0);
    virtual void setSortType(int type);
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
    void    slotCellClicked(int row, int col);
private:
    QMap<QString, double>   mStockMap;
    QEastMoneyBlockMangagerThread*      mBlockMgr;
};

#endif // QBLOCKTABLEWIDGET_H
