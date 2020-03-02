#ifndef QSHARETABLEWIDGET_H
#define QSHARETABLEWIDGET_H

#include "HqTableWidget.h"
#include "data_structure/sharedata.h"

class QSinaStkResultMergeThread;

class QShareTablewidget : public HqTableWidget
{
    Q_OBJECT
public:
    explicit QShareTablewidget(QWidget *parent = 0);
    virtual void setSortType(int type);
    virtual void displayNextPage();
    virtual void displayPreviousPage();
    virtual void displayFirstPage();
    virtual void displayLastPage();
public slots:
    void    setSelfShareCodesList(const QStringList& list);
private:
    void    initMenu();

signals:
    void    signalSetShareMarket(int mkt);
    void    signalSetDisplayMinuteGraph(const QString& code);
    void    signalSetDisplayDayGraph(const QString& code);
    void    signalSetDisplayBlockDetail(const QStringList& blockCodes);
    void    signalSetDisplayHSHK(const QString& code);
    void    signalSetFavCode(const QString& code);
    void    signalDisplayChinaTop10();
    void    signalSetSpecialConcern(const QString& code);
    void    signalDoubleClickCode(const QString& code);
    void    signalDisplayDetailOfCode(const QString& code);

public slots:
    void    setDataList(int page, int pagesize, const ShareDataList& list, qint64 time);
    void    slotCustomContextMenuRequested(const QPoint &pos);
    void    setShareMarket();
    void    setShareMarketType(int type);
    void    setDisplayMinuteGraph();
    void    setDisplayDayGraph();
    void    setDisplayBlockDetail();
    void    setDisplayHSHK();
    void    slotCellDoubleClicked(int row, int col);
    void    setSpecialConcer();
    void    slotCellClicked(int row, int col);
    void    slotRecvAllShareCodes(const QStringList& list);
    void    slotUpdateTimeOut();
private:
    QMap<QString, double>   mShareMap;
    QList<QAction*>         mCodesActionList;
    QSinaStkResultMergeThread*          mMergeThread;
    QTimer                              *mUpdateTimer;
};

#endif // QSHARETABLEWIDGET_H
